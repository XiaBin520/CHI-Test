#ifndef CHI_HNF_SNOOPHANDLE_H_
#define CHI_HNF_SNOOPHANDLE_H_

#include "ISnpHandle.h"

namespace CHI_HNF
{



class SnoopHandle : public ISnpHandle
{
private:
    bool got_SnpResp;
    bool got_SnpRespData;
    uint32_t got_SnpRespData_num;


private:
    void UpdateMemLine();
public:
    SnoopHandle(SNPFLIT *txsnp_flit, MemLine *mem_line);

    virtual bool MonitorRxRSP(RSPFLIT *rxrsp_flit);
    virtual bool MonitorRxDAT(DATFLIT *rxdat_flit);
    virtual bool IsDone();
};






SnoopHandle::SnoopHandle(SNPFLIT *txsnp_flit, MemLine *mem_line)
{
    this->mem_line = mem_line;
    this->got_txsnp_flit = txsnp_flit;

    mem_line->snp_is_pending = true;

    got_SnpResp         = false;
    got_SnpRespData     = false;
    got_SnpRespData_num = 0;
}







bool SnoopHandle::MonitorRxRSP(RSPFLIT *rxrsp_flit)
{
    if(rxrsp_flit == NULL)  return false;
    bool is_my_rxrsp = (rxrsp_flit->TxnID == got_txsnp_flit->TxnID) & 
                       (rxrsp_flit->Opcode == RspOpcodeEncoding::SnpResp);

    if(is_my_rxrsp)
    {
        got_SnpResp = true;
        got_rxrsp_flit_vec.push_back(rxrsp_flit);
    }
    return is_my_rxrsp;
}


bool SnoopHandle::MonitorRxDAT(DATFLIT *rxdat_flit)
{
    if(rxdat_flit == NULL) return false;
    bool is_my_rxdat = (rxdat_flit->TxnID == got_txsnp_flit->TxnID) &
                       (rxdat_flit->Opcode == DatOpcodeEncoding::SnpRespData);
    
    if(is_my_rxdat)
    {
        got_SnpRespData_num++;
        got_rxdat_flit_vec.push_back(rxdat_flit);

        uint32_t exp_num_wrap = Cache_Line_Width / Data_Width;
        got_SnpRespData = (got_SnpRespData_num == exp_num_wrap);
    }
    return is_my_rxdat;
}








void SnoopHandle::UpdateMemLine()
{
    if(got_SnpResp) return;
    if(got_SnpRespData)
    {
        uint32_t wr_data_num = got_rxdat_flit_vec.size();
        for(uint32_t i = 0; i < wr_data_num; i++)
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
}




bool SnoopHandle::IsDone()
{
    bool done = (got_SnpResp | got_SnpRespData);
    if(done)
    {
        UpdateMemLine();
        mem_line->snp_is_pending = false;
    }
    return done;
}



}
#endif