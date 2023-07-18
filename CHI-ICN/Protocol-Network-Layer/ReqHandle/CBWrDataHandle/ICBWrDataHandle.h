#ifndef CHI_HNF_ICBWRDATAHANDLE_H_
#define CHI_HNF_ICBWRDATAHANDLE_H_

#include "../IReqHandle.h"

namespace CHI_HNF
{



class ICBWrDataHandle : public IReqHandle
{
protected:
    bool set_CompDBIDResp;
protected:
    bool got_CompDBIDResp;
    bool got_CBWrData;
    uint32_t got_CBWrData_num;



protected:
    virtual void CBWrDataInit(uint32_t dbid, REQFLIT *rxreq_flit, MemLine *mem_line);
private:
    void WriteData();
public:
    virtual void DriverTxRSP(queue<RSPFLIT*>& txrsp_flit_queue);
    virtual bool MonitorTxRSP(RSPFLIT *txrsp_flit);
    virtual bool MonitorRxDAT(DATFLIT *rxdat_flit);

    virtual bool IsDone();
};



void ICBWrDataHandle::CBWrDataInit(uint32_t dbid, REQFLIT *rxreq_flit, MemLine *mem_line)
{
    this->dbid = dbid;
    this->got_rxreq_flit = rxreq_flit;
    this->mem_line = mem_line;

    mem_line->req_is_pending = true;

    set_CompDBIDResp = false;
    got_CompDBIDResp = false;
    got_CBWrData     = false;
    got_CBWrData_num = 0;
}







void ICBWrDataHandle::DriverTxRSP(queue<RSPFLIT*>& txrsp_flit_queue)
{
    if(mem_line->snp_is_pending) return;
    if(set_CompDBIDResp) return;

    set_CompDBIDResp = true;
    if(set_CompDBIDResp)
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

        mem_line->req_completion_is_sent = true;
    }

}








bool ICBWrDataHandle::MonitorTxRSP(RSPFLIT *txrsp_flit)
{
    if(txrsp_flit == NULL) return false;
    bool is_my_txrsp = (txrsp_flit->CF2 == dbid) &
                       (txrsp_flit->Opcode == RspOpcodeEncoding::CompDBIDResp);

    if(is_my_txrsp)
    {
        got_CompDBIDResp = true;
        got_txrsp_flit_vec.push_back(txrsp_flit);
    }
    return is_my_txrsp;
}


bool ICBWrDataHandle::MonitorRxDAT(DATFLIT *rxdat_flit)
{
    if(rxdat_flit == NULL) return false;
    bool is_my_rxdat = (rxdat_flit->TxnID == dbid) &
                       (rxdat_flit->Opcode == DatOpcodeEncoding::CopyBackWrData);

    if(is_my_rxdat)
    {
        got_CBWrData_num++;
        got_rxdat_flit_vec.push_back(rxdat_flit);

        uint32_t exp_num = (64 * 8) / Data_Width;
        got_CBWrData = (got_CBWrData_num == exp_num);
    }
    return is_my_rxdat;
}









void ICBWrDataHandle::WriteData()
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


bool ICBWrDataHandle::IsDone()
{
    bool tx_done = got_CompDBIDResp;
    bool rx_done = got_CBWrData;
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