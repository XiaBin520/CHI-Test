#ifndef CHI_RNF_INCBWRDATAHANDLE_H_
#define CHI_RNF_INCBWRDATAHANDLE_H_

#include "../IReqHandle.h"


namespace CHI_RNF
{

class INCBWrDataHandle : public IReqHandle
{
protected:
    bool set_NonCopyBackWrData;
    bool got_NonCopyBackWrData;
    uint32_t got_NonCopyBackWrData_num;
protected:
    bool got_DBIDResp;
    bool got_Comp;
    bool got_CompDBIDResp;



protected:
    virtual void InitNCBWrDataHandle(ACCFLIT *accreq_flit, REQFLIT *txreq_flit, CacheLine *cache_line);

public:
    virtual void DriverTxDAT(queue<DATFLIT*>& txdat_flit_queue);
    virtual bool MonitorTxDAT(DATFLIT *txdat_flit);
    virtual bool MonitorRxRSP(RSPFLIT *rxrsp_flit);
    virtual bool IsDone();
};




void INCBWrDataHandle::InitNCBWrDataHandle(ACCFLIT *accreq_flit, REQFLIT *txreq_flit, CacheLine *cache_line)
{
    this->accreq_flit = accreq_flit;
    this->cache_line = cache_line;
    this->got_txreq_flit = txreq_flit;
    
    this->age = 0;
    this->cache_line->req_is_pending = true;

    set_NonCopyBackWrData = false;
    got_NonCopyBackWrData = false;
    got_NonCopyBackWrData_num = 0;
    got_DBIDResp     = false;
    got_Comp         = false;
    got_CompDBIDResp = false;
}










/******************************************************/
/*  NCBWrReqData Driver                               */
/******************************************************/
void INCBWrDataHandle::DriverTxDAT(queue<DATFLIT*>& txdat_flit_queue)
{
    if(set_NonCopyBackWrData) return;

    if(got_DBIDResp | got_CompDBIDResp)  set_NonCopyBackWrData = true;

    if(set_NonCopyBackWrData)
    {
        RSPFLIT *rxrsp_flit = NULL;
        for(uint32_t i = 0; i < got_rxrsp_flit_vec.size(); i++)
        {
            rxrsp_flit = got_rxrsp_flit_vec[i];
            if((rxrsp_flit->Opcode == RspOpcodeEncoding::DBIDResp) |
               (rxrsp_flit->Opcode == RspOpcodeEncoding::CompDBIDResp))
               break;
        }


        // set aligned address
        uint64_t start_address = got_txreq_flit->Addr;
        uint32_t number_bytes = 0;
        switch(got_txreq_flit->Size)
        {
            case SizeEncoding::Size_16Bytes:
                number_bytes = 16;
                break;
            case SizeEncoding::Size_32Bytes:
                number_bytes = 32;
                break;
            case SizeEncoding::Size_64Bytes:
                number_bytes = 64;
                break;
            default:
                assert(0);
                break;
        }
        uint64_t aligned_address = (start_address/number_bytes) * number_bytes;

        // set ccid
        uint32_t ccid = (start_address >> 4) & 0b11;
        // set write enables
        uint64_t write_enables = accreq_flit->write_enable;
        // set write data
        uint8_t write_data[64];
        for(uint32_t i = 0; i < 64; i++)
        {
            bool write_en = (write_enables >> i) & 0b1;
            if(write_en) write_data[i] = accreq_flit->data[i];
            else write_data[i] = 0;
        }
        // set the number of wrap
        uint32_t num_wrap = ceil(number_bytes / (Data_Width/8.0));

        deque<DATFLIT*> set_txdat_flit_deque;
        for(uint32_t i = 0; i < num_wrap; i++)
        {
            uint32_t addr_offset = i * (Data_Width/8);
            uint64_t wrap_addr = aligned_address + addr_offset;
            uint32_t dataid = (wrap_addr >> 4) & 0b11;

            uint32_t line_offset = dataid * 16;
            uint64_t be_mask = (Data_Width == 128) ? 0xffff :
                               (Data_Width == 256) ? 0xffffffff :
                               (Data_Width == 512) ? 0xffffffffffffffff :
                                                     0;

            DATFLIT *txdat_flit = new DATFLIT();
            txdat_flit->QoS         = 0;
            txdat_flit->TgtID       = rxrsp_flit->SrcID;
            txdat_flit->SrcID       = 1234; // TODO
            txdat_flit->TxnID       = rxrsp_flit->CF2;
            txdat_flit->HomeNID     = 0;
            txdat_flit->Opcode      = DatOpcodeEncoding::NonCopyBackWrData;
            txdat_flit->RespErr     = RespErrEncoding::OK;
            txdat_flit->Resp        = 0; // TODO
            txdat_flit->CF1         = 0;
            txdat_flit->CBusy       = 0;
            txdat_flit->DBID        = 0;
            txdat_flit->CCID        = ccid;
            txdat_flit->DataID      = dataid;
            txdat_flit->TagOp       = 0;
            txdat_flit->Tag         = 0;
            txdat_flit->TU          = 0;
            txdat_flit->TraceTag    = 0;
            txdat_flit->BE          = (write_enables >> line_offset) & be_mask;
            memmove(txdat_flit->Data, &write_data[line_offset], (Data_Width/8));
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
/*  NCBWrReqData Monitor                              */
/******************************************************/
bool INCBWrDataHandle::MonitorRxRSP(RSPFLIT *rxrsp_flit)
{
    bool is_my_rxrsp = false;
    is_my_rxrsp = ((rxrsp_flit->Opcode == RspOpcodeEncoding::Comp) |
                   (rxrsp_flit->Opcode == RspOpcodeEncoding::DBIDResp) |
                   (rxrsp_flit->Opcode == RspOpcodeEncoding::CompDBIDResp)) &
                  (rxrsp_flit->TxnID == got_txreq_flit->TxnID);

    if(is_my_rxrsp)
    {
        if(rxrsp_flit->Opcode == RspOpcodeEncoding::Comp)
            got_Comp = true;
        if(rxrsp_flit->Opcode == RspOpcodeEncoding::DBIDResp)
            got_DBIDResp = true;
        if(rxrsp_flit->Opcode == RspOpcodeEncoding::CompDBIDResp)
            got_CompDBIDResp = true;
        got_rxrsp_flit_vec.push_back(rxrsp_flit);
    }
    return is_my_rxrsp;
}


bool INCBWrDataHandle::MonitorTxDAT(DATFLIT *txdat_flit)
{
    bool is_my_txdat = false;
    if(got_DBIDResp | got_CompDBIDResp)
    {
        RSPFLIT *rxrsp_flit = got_rxrsp_flit_vec.back();
        is_my_txdat = (txdat_flit->Opcode == DatOpcodeEncoding::NonCopyBackWrData) &
                      (txdat_flit->TxnID  == rxrsp_flit->CF2);
    }

    if(is_my_txdat)
    {
        got_NonCopyBackWrData_num++;
        got_txdat_flit_vec.push_back(txdat_flit);

        uint32_t write_size = (got_txreq_flit->Size == SizeEncoding::Size_16Bytes) ? 16 :
                              (got_txreq_flit->Size == SizeEncoding::Size_32Bytes) ? 32 :
                              (got_txreq_flit->Size == SizeEncoding::Size_64Bytes) ? 64 :
                                                                                     0;
        uint32_t exp_num = ceil(write_size / (Data_Width/8.0));
        got_NonCopyBackWrData = (got_NonCopyBackWrData_num == exp_num);
    }
    return is_my_txdat;
}





/******************************************************/
/*  NCBWrReqData Done                                 */
/******************************************************/
bool INCBWrDataHandle::IsDone()
{
    age++;
    if(age >= 1000) assert(0);

    bool tx_done = got_NonCopyBackWrData;
    bool rx_done = got_CompDBIDResp | (got_Comp & got_DBIDResp);
    bool done = (tx_done & rx_done);

    if(done)
    {
        cache_line->req_is_pending = false;
    }
    return done;
}




}
#endif
