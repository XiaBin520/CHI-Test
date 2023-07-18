#ifndef CHI_HNF_SNOOPREADHANDLE_H_
#define CHI_HNF_SNOOPREADHANDLE_H_

#include "IAccHandle.h"

namespace CHI_HNF
{



class SnoopReadHandle : public IAccHandle
{
private:
    virtual uint32_t SetSnpOpcode();

public:
    SnoopReadHandle(ACCFLIT *accreq_flit, MemLine *mem_line);
    virtual void DriverAccRSP(queue<ACCFLIT*>& accrsp_flit_queue);
};





SnoopReadHandle::SnoopReadHandle(ACCFLIT *accreq_flit, MemLine *mem_line)
{
    AccInit(accreq_flit, mem_line);
}




void SnoopReadHandle::DriverAccRSP(queue<ACCFLIT*>& accrsp_flit_queue)
{
    if(mem_line->snp_is_pending) return;
    if(set_ACCRSP) return;

    if(set_CHITxSNP & (!mem_line->snp_is_pending))
    {
        set_ACCRSP = true;
        mem_line->ReadData(accreq_flit->data);
        accrsp_flit_queue.push(accreq_flit);
    }
}




uint32_t SnoopReadHandle::SetSnpOpcode()
{
    uint32_t snp_opcode = SnpOpcodeEncoding::SnpCleanInvalid;
    return snp_opcode;
}



}
#endif