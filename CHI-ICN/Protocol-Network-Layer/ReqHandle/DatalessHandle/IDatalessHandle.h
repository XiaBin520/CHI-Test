#ifndef CHI_HNF_IDATALESSHANDLE_H_
#define CHI_HNF_IDATALESSHANDLE_H_

#include "../IReqHandle.h"

namespace CHI_HNF
{


class IDatalessHandle : public IReqHandle
{
protected:
    bool set_Comp;
protected:
    bool got_Comp;
    bool got_CompAck;


protected:
    virtual uint32_t SetResp() = 0;
    virtual void DatalessInit(uint32_t dbid, REQFLIT *rxreq_flit, MemLine *mem_line);

public:
    virtual void DriverTxRSP(queue<RSPFLIT*>& txrsp_flit_queue);
    virtual bool MonitorTxRSP(RSPFLIT *txrsp_flit);
    virtual bool MonitorRxRSP(RSPFLIT *rxrsp_flit);
    virtual bool IsDone();
};




void IDatalessHandle::DatalessInit(uint32_t dbid, REQFLIT *rxreq_flit, MemLine *mem_line)
{
    this->dbid = dbid;
    this->got_rxreq_flit = rxreq_flit;
    this->mem_line = mem_line;

    mem_line->req_is_pending = true;

    set_Comp = false;
    got_Comp = false;
    got_CompAck = (rxreq_flit->ExpCompAck) ? false : true;
}





void IDatalessHandle::DriverTxRSP(queue<RSPFLIT*>& txrsp_flit_queue)
{
    if(mem_line->snp_is_pending) return;
    if(set_Comp) return;

    set_Comp = true;
    if(set_Comp)
    {
        RSPFLIT *txrsp_flit = new RSPFLIT();
        txrsp_flit->QoS         = 0;
        txrsp_flit->TgtID       = got_rxreq_flit->SrcID;
        txrsp_flit->SrcID       = got_rxreq_flit->TgtID;
        txrsp_flit->TxnID       = got_rxreq_flit->TxnID;
        txrsp_flit->Opcode      = RspOpcodeEncoding::Comp;
        txrsp_flit->RespErr     = RespErrEncoding::OK;
        txrsp_flit->Resp        = SetResp();
        txrsp_flit->CF1         = 0;
        txrsp_flit->CBusy       = 0;
        txrsp_flit->CF2         = dbid;
        txrsp_flit->PCrdType    = 0;
        txrsp_flit->TagOp       = 0;
        txrsp_flit->TraceTag    = 0;
        txrsp_flit_queue.push(txrsp_flit);

        mem_line->req_completion_is_sent = true;
    }
}






bool IDatalessHandle::MonitorTxRSP(RSPFLIT *txrsp_flit)
{
    if(txrsp_flit == NULL) return false;
    bool is_my_txrsp = (txrsp_flit->CF2 == dbid) &
                       (txrsp_flit->Opcode == RspOpcodeEncoding::Comp);
    if(is_my_txrsp)
    {
        got_Comp = true;
        got_txrsp_flit_vec.push_back(txrsp_flit);
    }
    return is_my_txrsp;
}

bool IDatalessHandle::MonitorRxRSP(RSPFLIT *rxrsp_flit)
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






bool IDatalessHandle::IsDone()
{
    bool tx_done = got_Comp;
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