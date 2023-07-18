#ifndef CHI_HNF_INCBWRDATAHANDLE_H_
#define CHI_HNF_INCBWRDATAHANDLE_H_

#include "../IReqHandle.h"

namespace CHI_HNF
{

class INCBWrDataHandle : public IReqHandle
{
protected:
    bool set_Comp;
    bool set_DBIDResp;
    bool set_CompDBIDResp;
protected:
    bool got_Comp;
    bool got_DBIDResp;
    bool got_CompDBIDResp;
    bool got_NCBWrData;
    uint32_t got_NCBWrData_num;


protected:
    virtual void NCBWrDataInit(uint32_t dbid, REQFLIT *rxreq_flit, MemLine *mem_line);
private:
    void SetComp(queue<RSPFLIT*>& txrsp_flit_queue);
    void SetDBIDResp(queue<RSPFLIT*>& txrsp_flit_queue);
    void SetCompDBIDResp(queue<RSPFLIT*>& txrsp_flit_queue);
private:
    void WriteData();
public:
    virtual void DriverTxRSP(queue<RSPFLIT*>& txrsp_flit_queue);
    virtual bool MonitorTxRSP(RSPFLIT *txrsp_flit);
    virtual bool MonitorRxDAT(DATFLIT *rxdat_flit);

    virtual bool IsDone();
};



void INCBWrDataHandle::NCBWrDataInit(uint32_t dbid, REQFLIT *rxreq_flit, MemLine *mem_line)
{
    this->dbid = dbid;
    this->got_rxreq_flit = rxreq_flit;
    this->mem_line = mem_line;

    mem_line->req_is_pending = true;

    set_Comp          = false;
    set_DBIDResp      = false;
    set_CompDBIDResp  = false;
    got_Comp          = false;
    got_DBIDResp      = false;
    got_CompDBIDResp  = false;
    got_NCBWrData     = false;
    got_NCBWrData_num = 0;
}







void INCBWrDataHandle::SetComp(queue<RSPFLIT*>& txrsp_flit_queue)
{
    RSPFLIT *txrsp_flit = new RSPFLIT();
    txrsp_flit->QoS         = 0;
    txrsp_flit->TgtID       = got_rxreq_flit->SrcID;
    txrsp_flit->SrcID       = got_rxreq_flit->TgtID;
    txrsp_flit->TxnID       = got_rxreq_flit->TxnID;
    txrsp_flit->Opcode      = RspOpcodeEncoding::Comp;
    txrsp_flit->RespErr     = RespErrEncoding::OK;
    txrsp_flit->Resp        = 0;
    txrsp_flit->CF1         = 0;
    txrsp_flit->CBusy       = 0;
    txrsp_flit->CF2         = dbid;
    txrsp_flit->PCrdType    = 0;
    txrsp_flit->TagOp       = 0;
    txrsp_flit->TraceTag    = 0;

    txrsp_flit_queue.push(txrsp_flit);
}

void INCBWrDataHandle::SetDBIDResp(queue<RSPFLIT*>& txrsp_flit_queue)
{
    RSPFLIT *txrsp_flit = new RSPFLIT();
    txrsp_flit->QoS         = 0;
    txrsp_flit->TgtID       = got_rxreq_flit->SrcID;
    txrsp_flit->SrcID       = got_rxreq_flit->TgtID;
    txrsp_flit->TxnID       = got_rxreq_flit->TxnID;
    txrsp_flit->Opcode      = RspOpcodeEncoding::DBIDResp;
    txrsp_flit->RespErr     = 0;
    txrsp_flit->Resp        = 0;
    txrsp_flit->CF1         = 0;
    txrsp_flit->CBusy       = 0;
    txrsp_flit->CF2         = dbid;
    txrsp_flit->PCrdType    = 0;
    txrsp_flit->TagOp       = 0;
    txrsp_flit->TraceTag    = 0;

    txrsp_flit_queue.push(txrsp_flit);
}

void INCBWrDataHandle::SetCompDBIDResp(queue<RSPFLIT*>& txrsp_flit_queue)
{
    RSPFLIT *txrsp_flit = new RSPFLIT();
    txrsp_flit->QoS         = 0;
    txrsp_flit->TgtID       = got_rxreq_flit->SrcID;
    txrsp_flit->SrcID       = got_rxreq_flit->TgtID;
    txrsp_flit->TxnID       = got_rxreq_flit->TxnID;
    txrsp_flit->Opcode      = RspOpcodeEncoding::CompDBIDResp;
    txrsp_flit->RespErr     = RespErrEncoding::OK;
    txrsp_flit->Resp        = 0;
    txrsp_flit->CF1         = 0;
    txrsp_flit->CBusy       = 0;
    txrsp_flit->CF2         = dbid;
    txrsp_flit->PCrdType    = 0;
    txrsp_flit->TagOp       = 0;
    txrsp_flit->TraceTag    = 0;

    txrsp_flit_queue.push(txrsp_flit);
}


void INCBWrDataHandle::DriverTxRSP(queue<RSPFLIT*>& txrsp_flit_queue)
{
    if(set_Comp & set_DBIDResp) return;
    if(set_CompDBIDResp) return;

    if(set_Comp & (!set_DBIDResp))
    {
        set_DBIDResp = true;
        SetDBIDResp(txrsp_flit_queue);
    }
    else if(set_DBIDResp & (!set_Comp))
    {
        if(mem_line->snp_is_pending) return;

        set_Comp = true;
        mem_line->req_completion_is_sent = true;
        SetComp(txrsp_flit_queue);
    }
    else
    {
        if(mem_line->snp_is_pending)
        {
            set_DBIDResp = true;
            SetDBIDResp(txrsp_flit_queue);
        }
        else
        {
            set_Comp = (rand()%2 == 0);
            set_CompDBIDResp = (!set_Comp);

            if(set_Comp) SetComp(txrsp_flit_queue);
            else SetCompDBIDResp(txrsp_flit_queue);
            mem_line->req_completion_is_sent = true;
        }
    }



}








bool INCBWrDataHandle::MonitorTxRSP(RSPFLIT *txrsp_flit)
{
    if(txrsp_flit == NULL) return false;
    bool is_my_txrsp = (txrsp_flit->CF2 == dbid) &
                       ((txrsp_flit->Opcode == RspOpcodeEncoding::Comp) |
                        (txrsp_flit->Opcode == RspOpcodeEncoding::DBIDResp) |
                        (txrsp_flit->Opcode == RspOpcodeEncoding::CompDBIDResp));

    if(is_my_txrsp)
    {
        switch(txrsp_flit->Opcode)
        {
            case RspOpcodeEncoding::Comp:
                got_Comp = true;
                break;
            case RspOpcodeEncoding::DBIDResp:
                got_DBIDResp = true;
                break;
            case RspOpcodeEncoding::CompDBIDResp:
                got_CompDBIDResp = true;
                break;
            default:
                assert(0);
                break;
        }
        got_txrsp_flit_vec.push_back(txrsp_flit);
    }
    return is_my_txrsp;
}


bool INCBWrDataHandle::MonitorRxDAT(DATFLIT *rxdat_flit)
{
    if(rxdat_flit == NULL) return false;
    bool is_my_rxdat = (rxdat_flit->TxnID == dbid) &
                       (rxdat_flit->Opcode == DatOpcodeEncoding::NonCopyBackWrData);

    if(is_my_rxdat)
    {
        got_NCBWrData_num++;
        got_rxdat_flit_vec.push_back(rxdat_flit);

        uint32_t ncb_wr_size = -1;
        switch(got_rxreq_flit->Size)
        {
            case SizeEncoding::Size_16Bytes:
                ncb_wr_size = 16;
                break;
            case SizeEncoding::Size_32Bytes:
                ncb_wr_size = 32;
                break;
            case SizeEncoding::Size_64Bytes:
                ncb_wr_size = 64;
                break;
            default:
                assert(0);
                break;
        }

        uint32_t exp_ncb_wr_num = ceil(ncb_wr_size / (Data_Width/8.0));
        got_NCBWrData = (got_NCBWrData_num == exp_ncb_wr_num);
    }
    return is_my_rxdat;
}






void INCBWrDataHandle::WriteData()
{
    uint32_t ncb_wr_data_num = got_rxdat_flit_vec.size();
    for(uint32_t i = 0; i < ncb_wr_data_num; i++)
    {
        DATFLIT *rxdat_flit = got_rxdat_flit_vec[i];

        uint8_t write_data[64] = {0};
        uint64_t write_enables = 0;

        uint32_t line_offset = rxdat_flit->DataID * (Data_Width/8);
        uint64_t be_mask = (Data_Width == 128) ? 0xffff :
                           (Data_Width == 256) ? 0xffffffff :
                           (Data_Width == 512) ? 0xffffffffffffffff :
                                                 0;
        memmove(&write_data[line_offset], rxdat_flit->Data, (Data_Width/8));
        write_enables = (rxdat_flit->BE & be_mask) << line_offset;
        mem_line->WriteData(write_data, write_enables);
    }
}



bool INCBWrDataHandle::IsDone()
{
    bool tx_done = (got_Comp & got_DBIDResp) | got_CompDBIDResp;
    bool rx_done = got_NCBWrData;
    bool done = tx_done & rx_done;

    if(done)
    {
        mem_line->req_is_pending = false;
        mem_line->req_completion_is_sent = false;
        WriteData();
    }
    return done;
}



}
#endif