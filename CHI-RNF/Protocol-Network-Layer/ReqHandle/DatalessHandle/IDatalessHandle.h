#ifndef CHI_RNF_IDATALESSHANDLE_H_
#define CHI_RNF_IDATALESSHANDLE_H_

#include "../IReqHandle.h"


namespace CHI_RNF
{


class IDatalessHandle : public IReqHandle
{
protected:
    bool set_CompAck;
    bool got_CompAck;
protected:
    bool got_Comp;


protected:
    virtual void InitDatalessHandle(ACCFLIT *accreq_flit, REQFLIT *txreq_flit, CacheLine *cache_line);
    virtual void UpdateCacheLine() = 0;

public:
    virtual void DriverTxRSP(queue<RSPFLIT*>& txrsp_flit_queue);
    virtual bool MonitorTxRSP(RSPFLIT *txrsp_flit);
    virtual bool MonitorRxRSP(RSPFLIT *rxrsp_flit);
    virtual bool IsDone();
};






void IDatalessHandle::InitDatalessHandle(ACCFLIT *accreq_flit, REQFLIT *txreq_flit, CacheLine *cache_line)
{
    this->accreq_flit = accreq_flit;
    this->cache_line = cache_line;
    this->got_txreq_flit = txreq_flit;

    this->age = 0;
    this->cache_line->req_is_pending = true;

    set_CompAck = (txreq_flit->ExpCompAck) ? false : true;
    got_CompAck = (txreq_flit->ExpCompAck) ? false : true;
    got_Comp    = false;
}









/******************************************************/
/*  Driver TxRSP                                      */
/******************************************************/
void IDatalessHandle::DriverTxRSP(queue<RSPFLIT*>& txrsp_flit_queue)
{
    if(set_CompAck) return;

    if(got_Comp) set_CompAck = true;

    if(set_CompAck)
    {
        RSPFLIT *rxrsp_flit = NULL;
        if(got_Comp)
        {
            rxrsp_flit = got_rxrsp_flit_vec.back();
        }
        else
        {
            assert(0);
        }

        RSPFLIT *txrsp_flit = new RSPFLIT();
        txrsp_flit->QoS         = 0;
        txrsp_flit->TgtID       = rxrsp_flit->SrcID;
        txrsp_flit->SrcID       = 1234; // TODO
        txrsp_flit->TxnID       = rxrsp_flit->CF2;
        txrsp_flit->Opcode      = RspOpcodeEncoding::CompAck;
        txrsp_flit->RespErr     = 0;
        txrsp_flit->Resp        = 0;
        txrsp_flit->CF1         = 0;
        txrsp_flit->CBusy       = 0;
        txrsp_flit->CF2         = 0;
        txrsp_flit->PCrdType    = 0;
        txrsp_flit->TagOp       = 0;
        txrsp_flit->TraceTag    = 0;
        txrsp_flit_queue.push(txrsp_flit);
    }
}






/******************************************************/
/*  Monitor RxRSP, TxRSP                              */
/******************************************************/
bool IDatalessHandle::MonitorRxRSP(RSPFLIT *rxrsp_flit)
{
    bool is_my_rxrsp = false;
    is_my_rxrsp = (rxrsp_flit->Opcode == RspOpcodeEncoding::Comp) &
                  (rxrsp_flit->TxnID  == got_txreq_flit->TxnID);
    
    if(is_my_rxrsp)
    {
        got_Comp = true;
        got_rxrsp_flit_vec.push_back(rxrsp_flit);
        UpdateCacheLine();
    }
    return is_my_rxrsp;
}


bool IDatalessHandle::MonitorTxRSP(RSPFLIT *txrsp_flit)
{
    if(got_CompAck) return false;

    bool is_my_txrsp = false;
    if(got_Comp)
    {
        RSPFLIT *rxrsp_flit = got_rxrsp_flit_vec.back();
        is_my_txrsp = (txrsp_flit->Opcode == RspOpcodeEncoding::CompAck) &
                      (txrsp_flit->TxnID  == rxrsp_flit->CF2);
    }

    if(is_my_txrsp)
    {
        got_CompAck = true;
        got_txrsp_flit_vec.push_back(txrsp_flit);
    }
    return is_my_txrsp;
}





/******************************************************/
/*  Done                                              */
/******************************************************/
bool IDatalessHandle::IsDone()
{
    age++;
    if(age >= 1000) assert(0);

    bool tx_done = got_CompAck;
    bool rx_done = got_Comp;
    bool done = (tx_done & rx_done);
    
    if(done)
    {
        cache_line->req_is_pending = false;
    }
    return done;
}




}
#endif