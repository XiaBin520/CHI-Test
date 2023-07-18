#ifndef CHI_HNF_SNOOPWRITEHANDLE_H_
#define CHI_HNF_SNOOPWRITEHANDLE_H_

#include "IAccHandle.h"


namespace CHI_HNF
{


class SnoopWriteHandle : public IAccHandle
{
private:
    virtual uint32_t SetSnpOpcode();

public:
    SnoopWriteHandle(ACCFLIT *accreq_flit, MemLine *mem_line);
    virtual void DriverAccRSP(queue<ACCFLIT*>& accrsp_flit_queue);
};



uint32_t SnoopWriteHandle::SetSnpOpcode()
{
    uint32_t snp_opcode = -1;
    if(accreq_flit->write_enable == -1)
        snp_opcode = SnpOpcodeEncoding::SnpMakeInvalid;
    else
        snp_opcode = SnpOpcodeEncoding::SnpCleanInvalid;
    
    return snp_opcode;
}




SnoopWriteHandle::SnoopWriteHandle(ACCFLIT *accreq_flit, MemLine *mem_line)
{
    AccInit(accreq_flit, mem_line);
}





void SnoopWriteHandle::DriverAccRSP(queue<ACCFLIT*>& accrsp_flit_queue)
{
    if(mem_line->snp_is_pending) return;
    if(set_ACCRSP) return;

    if(set_CHITxSNP & (!mem_line->snp_is_pending))
    {
        set_ACCRSP = true;
        mem_line->WriteData(accreq_flit->data, accreq_flit->write_enable);
        accrsp_flit_queue.push(accreq_flit);
    }
}


}
#endif