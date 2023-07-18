#ifndef CHI_HNF_IREADHANDLE_H_
#define CHI_HNF_IREADHANDLE_H_

#include "../IReqHandle.h"


namespace CHI_HNF
{

class IReadHandle : public IReqHandle
{
protected:
    bool set_RespSepData;
    bool set_DataSepResp;
    bool set_CompData;

protected:
    bool got_RespSepData;
    bool got_DataSepResp;
    bool got_CompData;
    uint32_t got_DataSepResp_num;
    uint32_t got_CompData_num;
    bool got_CompAck;


protected:
    virtual uint32_t SetResp() = 0;
    virtual void ReadInit(uint32_t dbid, REQFLIT *rxreq_flit, MemLine *mem_line);

public:
    virtual void DriverTxRSP(queue<RSPFLIT*>& txrsp_flit_queue);
    virtual void DriverTxDAT(queue<DATFLIT*>& txdat_flit_queue);
    virtual bool MonitorTxRSP(RSPFLIT *txrsp_flit);
    virtual bool MonitorTxDAT(DATFLIT *txdat_flit);
    virtual bool MonitorRxRSP(RSPFLIT *rxrsp_flit);
    virtual bool IsDone();
};





void IReadHandle::ReadInit(uint32_t dbid, REQFLIT *rxreq_flit, MemLine *mem_line)
{
    this->dbid = dbid;
    this->got_rxreq_flit = rxreq_flit;
    this->mem_line = mem_line;

    mem_line->req_is_pending = true;


    set_RespSepData = false;
    set_DataSepResp = false;
    set_CompData    = false;

    got_RespSepData     = false;
    got_DataSepResp     = false;
    got_CompData        = false;
    got_DataSepResp_num = 0;
    got_CompData_num    = 0;
    got_CompAck         = false;
}








void IReadHandle::DriverTxRSP(queue<RSPFLIT*>& txrsp_flit_queue)
{
    if(mem_line->snp_is_pending) return;
    if(set_RespSepData) return;

    if(!set_CompData) set_RespSepData = true;

    if(set_RespSepData)
    {
        RSPFLIT *txrsp_flit = new RSPFLIT();
        txrsp_flit->QoS      = 0;
        txrsp_flit->TgtID    = got_rxreq_flit->SrcID;
        txrsp_flit->SrcID    = got_rxreq_flit->TgtID;
        txrsp_flit->TxnID    = got_rxreq_flit->TxnID;
        txrsp_flit->Opcode   = RspOpcodeEncoding::RespSepData;
        txrsp_flit->RespErr  = RespErrEncoding::OK;
        txrsp_flit->Resp     = SetResp();
        txrsp_flit->CF1      = 0;
        txrsp_flit->CBusy    = 0;
        txrsp_flit->CF2      = dbid;
        txrsp_flit->PCrdType = 0;
        txrsp_flit->TagOp    = 0;
        txrsp_flit->TraceTag = 0;
        txrsp_flit_queue.push(txrsp_flit);

        mem_line->req_completion_is_sent = true;
    }
}

void IReadHandle::DriverTxDAT(queue<DATFLIT*>& txdat_flit_queue)
{
    if(mem_line->snp_is_pending) return;
    if(set_DataSepResp | set_CompData) return;

    if(set_RespSepData) set_DataSepResp = true;
    else set_CompData = true;

    if(set_DataSepResp | set_CompData)
    {
        // set field opcode, resp, ccid
        uint32_t opcode = set_CompData ? DatOpcodeEncoding::CompData : 
                                         DatOpcodeEncoding::DataSepResp;
        uint32_t ccid = (got_rxreq_flit->Addr >> 4) & 0b11;

        // decide the number of wrap, aligned address
        uint32_t exp_num_wrap = Cache_Line_Width / Data_Width;
        uint64_t aligned_addr = (got_rxreq_flit->Addr / 64) * 64;

        uint8_t data_read[64];
        mem_line->ReadData(data_read);
        deque<DATFLIT*> set_txdat_flit_deque;
        for(uint32_t i = 0; i < exp_num_wrap; i++)
        {
            uint32_t addr_offset = i * (Data_Width/8);
            uint64_t wrap_addr = aligned_addr + addr_offset;
            uint32_t dataid    = (wrap_addr >> 4) & 0b11;

            uint32_t line_offset = dataid * 16;

            DATFLIT *txdat_flit = new DATFLIT();
            txdat_flit->QoS         = 0;
            txdat_flit->TgtID       = got_rxreq_flit->SrcID;
            txdat_flit->SrcID       = got_rxreq_flit->TgtID;
            txdat_flit->TxnID       = got_rxreq_flit->TxnID;
            txdat_flit->HomeNID     = got_rxreq_flit->TgtID;
            txdat_flit->Opcode      = opcode;
            txdat_flit->RespErr     = RespErrEncoding::OK;
            txdat_flit->Resp        = SetResp();
            txdat_flit->CF1         = 0;
            txdat_flit->CBusy       = 0;
            txdat_flit->DBID        = dbid;
            txdat_flit->CCID        = ccid;
            txdat_flit->DataID      = dataid;
            txdat_flit->TagOp       = TagOpEncoding::Invalid;
            txdat_flit->Tag         = 0;
            txdat_flit->TU          = 0;
            txdat_flit->TraceTag    = 0;
            txdat_flit->BE          = 0;
            memmove(txdat_flit->Data, &data_read[line_offset], (Data_Width/8));

            set_txdat_flit_deque.push_back(txdat_flit);
        }

        mem_line->req_completion_is_sent = true;

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




bool IReadHandle::MonitorTxRSP(RSPFLIT *txrsp_flit)
{
    if(txrsp_flit == NULL) return false;

    bool is_my_txrsp = (txrsp_flit->CF2 == dbid) &
                       (txrsp_flit->Opcode == RspOpcodeEncoding::RespSepData);
    if(is_my_txrsp)
    {
        got_RespSepData = true;
        got_txrsp_flit_vec.push_back(txrsp_flit);
    }
    return is_my_txrsp;
}

bool IReadHandle::MonitorTxDAT(DATFLIT *txdat_flit)
{
    if(txdat_flit == NULL) return false;

    bool is_my_txdat = (txdat_flit->DBID == dbid) &
                       ((txdat_flit->Opcode == DatOpcodeEncoding::DataSepResp) | 
                        (txdat_flit->Opcode == DatOpcodeEncoding::CompData));
    if(is_my_txdat)
    {
        if(txdat_flit->Opcode == DatOpcodeEncoding::DataSepResp)
            got_DataSepResp_num++;
        else if(txdat_flit->Opcode == DatOpcodeEncoding::CompData)
            got_CompData_num++;
        got_txdat_flit_vec.push_back(txdat_flit);

        uint32_t exp_num_wrap = Cache_Line_Width / Data_Width;
        got_DataSepResp = (got_DataSepResp_num == exp_num_wrap);
        got_CompData    = (got_CompData_num    == exp_num_wrap);
    }
    return is_my_txdat;
}

bool IReadHandle::MonitorRxRSP(RSPFLIT *rxrsp_flit)
{
    if(rxrsp_flit == NULL) return false;
    bool is_my_rxrsp = (rxrsp_flit->TxnID == dbid) &
                       (rxrsp_flit->Opcode == RspOpcodeEncoding::CompAck);
    if(is_my_rxrsp)
    {
        got_CompAck = true;
        got_rxrsp_flit_vec.push_back(rxrsp_flit);
    }
    return is_my_rxrsp;
}







bool IReadHandle::IsDone()
{
    bool tx_done = (got_RespSepData & got_DataSepResp) | got_CompData;
    bool rx_done = got_CompAck;
    bool done = tx_done & rx_done;

    if(done)
    {
        mem_line->req_is_pending = false;
        mem_line->req_completion_is_sent = false;
    }
    return done;
}









}
#endif