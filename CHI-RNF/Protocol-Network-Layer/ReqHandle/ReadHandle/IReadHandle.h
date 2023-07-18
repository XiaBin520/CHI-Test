#ifndef CHI_RNF_IREADHANDLE_H_
#define CHI_RNF_IREADHANDLE_H_

#include "../IReqHandle.h"

namespace CHI_RNF
{



class IReadHandle : public IReqHandle
{
protected:
    bool set_CompAck;
    bool got_CompAck;
protected:
    bool got_RespSepData;
    bool got_DataSepResp;
    bool got_CompData;
    uint32_t got_DataSepResp_num;
    uint32_t got_CompData_num;



protected:
    virtual void InitReadHandle(ACCFLIT *accreq_flit, REQFLIT *txreq_flit, CacheLine *cache_line);
    virtual void UpdateCacheLine() = 0;

public:
    virtual void DriverTxRSP(queue<RSPFLIT*>& txrsp_flit_queue);
    virtual bool MonitorRxRSP(RSPFLIT *rxrsp_flit);
    virtual bool MonitorRxDAT(DATFLIT *rxdat_flit);
    virtual bool MonitorTxRSP(RSPFLIT *txrsp_flit);
    virtual bool IsDone();
};






void IReadHandle::InitReadHandle(ACCFLIT *accreq_flit, REQFLIT *txreq_flit, CacheLine *cache_line)
{
    this->accreq_flit = accreq_flit;
    this->cache_line = cache_line;
    this->got_txreq_flit = txreq_flit;
    
    this->age = 0;
    this->cache_line->req_is_pending = true;

    set_CompAck = (txreq_flit->ExpCompAck) ? false : true;
    got_CompAck = (txreq_flit->ExpCompAck) ? false : true;

    got_RespSepData = false;
    got_DataSepResp = false;
    got_CompData    = false;
    got_DataSepResp_num = 0;
    got_CompData_num    = 0;
}










/******************************************************/
/*  IReadHandle Driver                                */
/******************************************************/
void IReadHandle::DriverTxRSP(queue<RSPFLIT*>& txrsp_flit_queue)
{
    if(set_CompAck) return;

    if(got_RespSepData | got_CompData) set_CompAck = true;

    if(set_CompAck)
    {
        uint32_t tgtid = 0;
        uint32_t txnid = 0;
        if(got_RespSepData)
        {
            RSPFLIT *rxrsp_flit = got_rxrsp_flit_vec.back();
            tgtid = rxrsp_flit->SrcID;
            txnid = rxrsp_flit->CF2;
        }
        else if(got_CompData)
        {
            DATFLIT *rxdat_flit = got_rxdat_flit_vec.back();
            tgtid = rxdat_flit->HomeNID;
            txnid = rxdat_flit->DBID; 
        }
        else
        {
            assert(0);
        }

        RSPFLIT *txrsp_flit = new RSPFLIT();
        txrsp_flit->QoS         = 0;
        txrsp_flit->TgtID       = tgtid;
        txrsp_flit->SrcID       = 1234; // TODO
        txrsp_flit->TxnID       = txnid;
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
        // 不能处理 Snoop Transaction
        if(got_RespSepData & set_CompAck & (!cache_line->req_all_data_response))
            cache_line->snp_response_is_locked = true;
    }
}







/******************************************************/
/*  IReadHandle Monitor                               */
/******************************************************/
bool IReadHandle::MonitorRxRSP(RSPFLIT *rxrsp_flit)
{
    bool is_my_rxrsp = false;
    is_my_rxrsp = (rxrsp_flit->Opcode == RspOpcodeEncoding::RespSepData) &
                  (rxrsp_flit->TxnID  == got_txreq_flit->TxnID);

    if(is_my_rxrsp)
    {
        got_RespSepData = true;
        got_rxrsp_flit_vec.push_back(rxrsp_flit);
    }
    return is_my_rxrsp;
}



bool IReadHandle::MonitorRxDAT(DATFLIT *rxdat_flit)
{
    bool is_my_rxdat = false;
    is_my_rxdat = ((rxdat_flit->Opcode == DatOpcodeEncoding::DataSepResp) | 
                   (rxdat_flit->Opcode == DatOpcodeEncoding::CompData)) &
                  (rxdat_flit->TxnID   == got_txreq_flit->TxnID);
    
    if(is_my_rxdat)
    {
        if(rxdat_flit->Opcode == DatOpcodeEncoding::DataSepResp)
            got_DataSepResp_num++;
        if(rxdat_flit->Opcode == DatOpcodeEncoding::CompData)
            got_CompData_num++;
        got_rxdat_flit_vec.push_back(rxdat_flit);
        cache_line->req_one_data_response  = true;
        cache_line->snp_response_is_locked = true;

        uint32_t exp_num = (64 * 8) / Data_Width;
        got_DataSepResp = (got_DataSepResp_num == exp_num);
        got_CompData    = (got_CompData_num    == exp_num);

        if(got_DataSepResp | got_CompData)
        {
            cache_line->req_one_data_response  = false;
            cache_line->req_all_data_response  = true;
            cache_line->snp_response_is_locked = false;
            UpdateCacheLine();
        }
    }
    return is_my_rxdat;
}



bool IReadHandle::MonitorTxRSP(RSPFLIT *txrsp_flit)
{
    if(got_CompAck) return false;

    bool is_my_txrsp = false;
    if(got_RespSepData)
    {
        RSPFLIT *rxrsp_flit = got_rxrsp_flit_vec.back();
        is_my_txrsp = (txrsp_flit->Opcode == RspOpcodeEncoding::CompAck) &
                      (txrsp_flit->TxnID  == rxrsp_flit->CF2);
    }
    else if(got_DataSepResp | got_CompData)
    {
        DATFLIT *rxdat_flit = got_rxdat_flit_vec.back();
        is_my_txrsp = (txrsp_flit->Opcode == RspOpcodeEncoding::CompAck) &
                      (txrsp_flit->TxnID  == rxdat_flit->DBID);
    }

    if(is_my_txrsp)
    {
        got_CompAck = true;
        got_txrsp_flit_vec.push_back(txrsp_flit);
    }
    return is_my_txrsp;
}








/******************************************************/
/*  IReadHandle Done                                  */
/******************************************************/
bool IReadHandle::IsDone()
{
    age++;
    if(age >= 1000) assert(0);

    bool tx_done = got_CompAck;
    bool rx_done = got_CompData | (got_RespSepData & got_DataSepResp);
    bool done = (tx_done & rx_done);

    if(done)
    {
        cache_line->req_is_pending = false;
        cache_line->req_one_data_response  = false;
        cache_line->req_all_data_response  = false;
        cache_line->snp_response_is_locked = false;
    }
    return done;
}




}
#endif