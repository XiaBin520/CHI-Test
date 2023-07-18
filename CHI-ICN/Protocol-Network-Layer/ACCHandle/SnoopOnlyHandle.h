#ifndef CHI_HNF_SNOOPONLYHANDLE_H_
#define CHI_HNF_SNOOPONLYHANDLE_H_

#include "IAccHandle.h"


namespace CHI_HNF
{



class SnoopOnlyHandle : public IAccHandle
{
private:
    virtual uint32_t SetSnpOpcode();

public:
    SnoopOnlyHandle(ACCFLIT *accreq_flit, MemLine *mem_line);
    virtual void DriverAccRSP(queue<ACCFLIT*>& accrsp_flit_queue);
};




uint32_t SnoopOnlyHandle::SetSnpOpcode()
{
    uint32_t snp_opcode = accreq_flit->snp_opcode;
    switch(snp_opcode)
    {
        case SnpOpcodeEncoding::SnpClean:
        case SnpOpcodeEncoding::SnpShared:
        case SnpOpcodeEncoding::SnpUnique:
        case SnpOpcodeEncoding::SnpCleanInvalid:
        case SnpOpcodeEncoding::SnpCleanShared:
        case SnpOpcodeEncoding::SnpMakeInvalid:
        case SnpOpcodeEncoding::SnpQuery:
            break;
        default:
            assert(0);
            break;
    }
    return snp_opcode;
}




SnoopOnlyHandle::SnoopOnlyHandle(ACCFLIT *accreq_flit, MemLine *mem_line)
{
    AccInit(accreq_flit, mem_line);
}



void SnoopOnlyHandle::DriverAccRSP(queue<ACCFLIT*>& accrsp_flit_queue)
{
    if(mem_line->snp_is_pending) return;
    if(set_ACCRSP) return;

    if(set_CHITxSNP & (!mem_line->snp_is_pending))
    {
        set_ACCRSP = true;
        accrsp_flit_queue.push(accreq_flit);
    }
}




}
#endif