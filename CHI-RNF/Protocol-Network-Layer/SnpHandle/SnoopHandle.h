#ifndef CHI_RNF_SNOOPHANDLE_H_
#define CHI_RNF_SNOOPHANDLE_H_

#include "ISnpHandle.h"

namespace CHI_RNF
{


class SnoopHandle : public ISnpHandle
{
protected:
    bool need_SnpResp;
    bool need_SnpRespData;
    uint32_t need_Resp;
protected:
    bool set_SnpResp;
    bool got_SnpResp;
    bool set_SnpRespData;
    bool got_SnpRespData;
    uint32_t got_SnpRespData_num;



protected:
    void SnoopInit(SNPFLIT *rxsnp_flit, CacheLine *cache_line);
    virtual void SetNeedResponse() = 0;
    virtual void UpdateCacheLine(); // Note: SnpQuery Can't Update CacheLine

public:
    virtual void DriverTxRSP(queue<RSPFLIT*>& txrsp_flit_queue);
    virtual void DriverTxDAT(queue<DATFLIT*>& txdat_flit_queue);
    virtual bool MonitorTxRSP(RSPFLIT *txrsp_flit);
    virtual bool MonitorTxDAT(DATFLIT *txdat_flit);
    virtual bool IsDone();
};









void SnoopHandle::SnoopInit(SNPFLIT *rxsnp_flit, CacheLine *cache_line)
{
    this->cache_line = cache_line;
    this->got_rxsnp_flit = rxsnp_flit;

    this->cache_line->snp_is_pending = true;

    need_SnpResp     = false;
    need_SnpRespData = false;
    need_Resp        = -1;

    set_SnpResp         = false;
    got_SnpResp         = false;
    set_SnpRespData     = false;
    got_SnpRespData     = false;
    got_SnpRespData_num = 0;
}






/******************************************************/
/*  SnoopHandle Driver                                */
/******************************************************/
void SnoopHandle::DriverTxRSP(queue<RSPFLIT*>& txrsp_flit_queue)
{
    if(set_SnpResp | set_SnpRespData) return;

    if(need_SnpResp) set_SnpResp = true;

    if(set_SnpResp)
    {
        RSPFLIT *txrsp_flit = new RSPFLIT();
        txrsp_flit->QoS         = 0;
        txrsp_flit->TgtID       = got_rxsnp_flit->SrcID;
        txrsp_flit->SrcID       = 1234; // TODO
        txrsp_flit->TxnID       = got_rxsnp_flit->TxnID;
        txrsp_flit->Opcode      = RspOpcodeEncoding::SnpResp;
        txrsp_flit->RespErr     = RespErrEncoding::OK;
        txrsp_flit->Resp        = need_Resp;
        txrsp_flit->CF1         = 0;
        txrsp_flit->CBusy       = 0;
        txrsp_flit->CF2         = 0;
        txrsp_flit->PCrdType    = 0;
        txrsp_flit->TagOp       = 0;
        txrsp_flit->TraceTag    = 0;
        txrsp_flit_queue.push(txrsp_flit);
    }
}



void SnoopHandle::DriverTxDAT(queue<DATFLIT*>& txdat_flit_queue)
{
    if(set_SnpResp | set_SnpRespData) return;

    if(need_SnpRespData) set_SnpRespData = true;

    if(set_SnpRespData)
    {
        // set the number of wrap for snoop response
        uint32_t num_wrap = Cache_Line_Width / Data_Width;
        // set ccid
        uint64_t start_addr = got_rxsnp_flit->Addr;
        uint32_t ccid = (start_addr >> 4) & 0b11;
        // set aligned address by 64 Bytes
        uint64_t aligned_addr = (start_addr / 64) * 64;

        deque<DATFLIT*> set_txdat_flit_deque;
        uint8_t data_read[64];
        cache_line->ReadData(data_read);

        for(uint32_t i = 0; i < num_wrap; i++)
        {   
            uint32_t addr_offset = i * (Data_Width/8);
            uint64_t wrap_addr   = aligned_addr + addr_offset;
            uint32_t dataid      = (wrap_addr >> 4) & 0b11;

            uint32_t line_offset = (dataid * 16);

            DATFLIT *txdat_flit = new DATFLIT();
            txdat_flit->QoS         = 0;
            txdat_flit->TgtID       = got_rxsnp_flit->SrcID;
            txdat_flit->SrcID       = 1234; // TODO
            txdat_flit->TxnID       = got_rxsnp_flit->TxnID;
            txdat_flit->HomeNID     = 0;
            txdat_flit->Opcode      = DatOpcodeEncoding::SnpRespData;
            txdat_flit->RespErr     = RespErrEncoding::OK;
            txdat_flit->Resp        = need_Resp;
            txdat_flit->CF1         = 0;
            txdat_flit->CBusy       = 0;
            txdat_flit->DBID        = 0;
            txdat_flit->CCID        = ccid;
            txdat_flit->DataID      = dataid;
            txdat_flit->TagOp       = 0;
            txdat_flit->Tag         = 0;
            txdat_flit->TU          = 0;
            txdat_flit->TraceTag    = 0;
            txdat_flit->BE          = (Data_Width == 128) ? 0xffff :
                                      (Data_Width == 256) ? 0xffffffff :
                                      (Data_Width == 512) ? 0xffffffffffffffff :
                                                            0;
            memmove(txdat_flit->Data, &data_read[line_offset], (Data_Width/8));
            set_txdat_flit_deque.push_back(txdat_flit);
        }


        // reorder set txdat_flit deque
        uint32_t dataid_mask = (Data_Width == 128) ? 0b11 :
                               (Data_Width == 256) ? 0b10 :
                               (Data_Width == 512) ? 0b00 :
                                                     0b00;
        uint32_t need_first_dataid = ccid & dataid_mask;
        while(1)
        {
            DATFLIT *reorder_txdat = set_txdat_flit_deque.front();
            if(reorder_txdat->DataID == need_first_dataid)
            {
                break;
            }
            else
            {
                set_txdat_flit_deque.pop_front();
                set_txdat_flit_deque.push_back(reorder_txdat);
            }
        }

        while(!set_txdat_flit_deque.empty())
        {
            DATFLIT *temp_txdat = set_txdat_flit_deque.front();
            set_txdat_flit_deque.pop_front();
            txdat_flit_queue.push(temp_txdat);
        }
    }
}








/******************************************************/
/*  SnoopHandle Monitor                               */
/******************************************************/
bool SnoopHandle::MonitorTxRSP(RSPFLIT *txrsp_flit)
{
    if(!set_SnpResp) return false;

    bool is_my_txrsp = false;
    is_my_txrsp = (txrsp_flit->Opcode == RspOpcodeEncoding::SnpResp) & 
                  (txrsp_flit->TxnID  == got_rxsnp_flit->TxnID);
    
    if(is_my_txrsp)
    {
        got_SnpResp = true;
        got_txrsp_flit_vec.push_back(txrsp_flit);
    }
    return is_my_txrsp;
}


bool SnoopHandle::MonitorTxDAT(DATFLIT *txdat_flit)
{
    if(!set_SnpRespData) return false;

    bool is_my_txdat = false;
    is_my_txdat = (txdat_flit->Opcode == DatOpcodeEncoding::SnpRespData) &
                  (txdat_flit->TxnID  == got_rxsnp_flit->TxnID);
    
    if(is_my_txdat)
    {
        got_SnpRespData_num++;
        got_txdat_flit_vec.push_back(txdat_flit);
        got_SnpRespData = (got_SnpRespData_num == (Cache_Line_Width/Data_Width));
    }
    return is_my_txdat;
}








/******************************************************/
/*  SnoopHandle Done                                  */
/******************************************************/
void SnoopHandle::UpdateCacheLine()
{
    uint32_t resp = -1;
    if(got_SnpResp)
    {
        RSPFLIT *txrsp_flit = got_txrsp_flit_vec.back();
        resp = txrsp_flit->Resp;
    }
    else if(got_SnpRespData)
    {
        DATFLIT *txdat_flit = got_txdat_flit_vec.back();
        resp = txdat_flit->Resp;
    }
    else
    {
        assert(0);
    }


    uint32_t final_state = -1;
    switch(resp)
    {
        case RespEncoding::SnoopResp::I:
        case RespEncoding::SnoopResp::I_PD:
            final_state = CacheStateEncoding::Invalid;
            break;
        case RespEncoding::SnoopResp::UC:
        case RespEncoding::SnoopResp::UC_PD:
            final_state = CacheStateEncoding::UC;
            break;
        case RespEncoding::SnoopResp::SC:
        case RespEncoding::SnoopResp::SC_PD:
            final_state = CacheStateEncoding::SC;
            break;
        default:
            assert(0);
            break;
    }
    cache_line->WriteState(final_state);
}



bool SnoopHandle::IsDone()
{
    bool tx_done = got_SnpResp | got_SnpRespData;
    bool rx_done = true;
    bool done = (tx_done & rx_done);

    if(done)
    {
        UpdateCacheLine();
        cache_line->snp_is_pending = false;
    }
    return done;
}



}
#endif
