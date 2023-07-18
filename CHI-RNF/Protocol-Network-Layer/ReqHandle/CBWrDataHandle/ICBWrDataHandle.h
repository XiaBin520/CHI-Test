#ifndef CHI_RNF_ICBWRDATAHANDLE_H_
#define CHI_RNF_ICBWRDATAHANDLE_H_

#include "../IReqHandle.h"


namespace CHI_RNF
{

class ICBWrDataHandle : public IReqHandle
{
protected:
    bool set_CopyBackWrData;
    bool got_CopyBackWrData;
    uint32_t got_CopyBackWrData_num;
protected:
    bool got_CompDBIDResp;


protected:
    virtual void InitCBWrDataHandle(ACCFLIT *accreq_flit, REQFLIT *txreq_flit, CacheLine *cache_line);
    virtual uint32_t SetCBWrDataResp() = 0;
    virtual void UpdateCacheLine() = 0;

public:
    virtual void DriverTxDAT(queue<DATFLIT*>& txdat_flit_queue);
    virtual bool MonitorTxDAT(DATFLIT *txdat_flit);
    virtual bool MonitorRxRSP(RSPFLIT *rxrsp_flit);
    virtual bool IsDone();
};





void ICBWrDataHandle::InitCBWrDataHandle(ACCFLIT *accreq_flit, REQFLIT *txreq_flit, CacheLine *cache_line)
{
    this->accreq_flit = accreq_flit;
    this->cache_line = cache_line;
    this->got_txreq_flit = txreq_flit;
    
    this->age = 0;
    this->cache_line->req_is_pending = true;

    set_CopyBackWrData = false;
    got_CopyBackWrData = false;
    got_CopyBackWrData_num = 0;
    got_CompDBIDResp = false;
}









/******************************************************/
/*  Driver TxDAT                                      */
/******************************************************/
void ICBWrDataHandle::DriverTxDAT(queue<DATFLIT*>& txdat_flit_queue)
{
    if(set_CopyBackWrData) return;

    if(got_CompDBIDResp) set_CopyBackWrData = true;

    if(set_CopyBackWrData)
    {
        REQFLIT *txreq_flit = got_txreq_flit;
        RSPFLIT *rxrsp_flit = got_rxrsp_flit_vec.back();

        uint64_t start_address = txreq_flit->Addr;
        uint64_t aligned_address = (start_address/64) * 64;


        // set ccid
        uint32_t ccid = (start_address >> 4) & 0b11;
        // set resp
        uint32_t resp = SetCBWrDataResp();
        // set the number of wrap
        uint32_t num_wrap = Cache_Line_Width / Data_Width;
        // read data from cacheline
        uint8_t data_read[64];
        cache_line->ReadData(data_read);

        deque<DATFLIT*> set_txdat_flit_deque;
        for(uint32_t i = 0; i < num_wrap; i++)
        {
            uint32_t addr_offset = i * (Data_Width/8);
            uint64_t wrap_addr = aligned_address + addr_offset;
            uint32_t dataid    = (wrap_addr >> 4) & 0b11;

            uint32_t line_offset = dataid * 16;

            DATFLIT *txdat_flit = new DATFLIT();
            txdat_flit->QoS         = 0;
            txdat_flit->TgtID       = rxrsp_flit->SrcID;
            txdat_flit->SrcID       = 1234; // TODO
            txdat_flit->TxnID       = rxrsp_flit->CF2;
            txdat_flit->HomeNID     = 0;
            txdat_flit->Opcode      = DatOpcodeEncoding::CopyBackWrData;
            txdat_flit->RespErr     = RespErrEncoding::OK;
            txdat_flit->Resp        = resp;
            txdat_flit->CF1         = 0;
            txdat_flit->CBusy       = 0;
            txdat_flit->DBID        = 0;
            txdat_flit->CCID        = ccid;
            txdat_flit->DataID      = dataid;
            txdat_flit->TagOp       = 0;
            txdat_flit->Tag         = 0;
            txdat_flit->TU          = 0;
            txdat_flit->TraceTag    = 0;

            if(txdat_flit->Resp == RespEncoding::WriteDataResp::I)
            {
                txdat_flit->BE = 0;
                memset(txdat_flit->Data, 0, (Data_Width/8));
            }
            else
            {
                txdat_flit->BE = (Data_Width == 128) ? 0xffff :
                                 (Data_Width == 256) ? 0xffffffff :
                                 (Data_Width == 512) ? 0xffffffffffffffff :
                                                       0;
                memmove(txdat_flit->Data, &data_read[line_offset], (Data_Width/8));
            }
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
/*  CopyBack Write Data Monitor                       */
/******************************************************/
bool ICBWrDataHandle::MonitorRxRSP(RSPFLIT *rxrsp_flit)
{
    bool is_my_rxrsp = false;
    is_my_rxrsp = (rxrsp_flit->Opcode == RspOpcodeEncoding::CompDBIDResp) &
                  (rxrsp_flit->TxnID  == got_txreq_flit->TxnID);
    
    if(is_my_rxrsp)
    {
        got_CompDBIDResp = true;
        got_rxrsp_flit_vec.push_back(rxrsp_flit);
    }
    return is_my_rxrsp;
}


bool ICBWrDataHandle::MonitorTxDAT(DATFLIT *txdat_flit)
{
    bool is_my_txdat = false;
    RSPFLIT *rxrsp_flit = NULL;
    if(got_CompDBIDResp)
    {
        rxrsp_flit = got_rxrsp_flit_vec.back();
        is_my_txdat = (txdat_flit->Opcode == DatOpcodeEncoding::CopyBackWrData) &
                      (txdat_flit->TxnID  == rxrsp_flit->CF2);
    }
        
    if(is_my_txdat)
    {
        got_CopyBackWrData_num++;
        got_txdat_flit_vec.push_back(txdat_flit);
        uint32_t exp_num = (64 * 8) / Data_Width;
        if(got_CopyBackWrData_num == exp_num)
        {
            got_CopyBackWrData = true;
            UpdateCacheLine();
        }
    }
    return is_my_txdat;
}





/******************************************************/
/*  CBWrDataReq Done                                  */
/******************************************************/
bool ICBWrDataHandle::IsDone()
{
    age++;
    if(age >= 1000) assert(0);

    bool tx_done = got_CopyBackWrData;
    bool rx_done = got_CompDBIDResp;
    bool done = (tx_done & rx_done);
    
    if(done)
    {
        cache_line->req_is_pending = false;
    }
    return done;
}

}
#endif