#ifndef CHI_HNF_IACCHANDLE_H_
#define CHI_HNF_IACCHANDLE_H_

#include "../../ACC-Common/acc_flit.h"
#include "../../CHI-Common/chi_flit.h"
#include "../memory.h"


namespace CHI_HNF
{


class IAccHandle
{
protected:
    ACCFLIT *accreq_flit;
    MemLine *mem_line;
protected:
    bool set_CHITxSNP;
    bool set_ACCRSP;


protected:
    virtual void AccInit(ACCFLIT *accreq_flit, MemLine *mem_line);
    virtual uint32_t SetSnpOpcode() = 0;
public:
    virtual void DriverTxSNP(queue<SNPFLIT*>& txsnp_flit_queue);
    virtual void DriverAccRSP(queue<ACCFLIT*>& accrsp_flit_queue) {return;}
    virtual bool IsDone();
};






void IAccHandle::AccInit(ACCFLIT *accreq_flit, MemLine *mem_line)
{
    this->accreq_flit = accreq_flit;
    this->mem_line = mem_line;

    this->mem_line->acc_is_pending = true;

    set_CHITxSNP = false;
    set_ACCRSP = false;
}




void IAccHandle::DriverTxSNP(queue<SNPFLIT*>& txsnp_flit_queue)
{
    if(mem_line->snp_is_pending) return;
    if(mem_line->req_completion_is_sent) return;
    if(set_CHITxSNP) return;

    set_CHITxSNP = true;
    if(set_CHITxSNP)
    {
        SNPFLIT *txsnp_flit = new SNPFLIT();
        txsnp_flit->QoS         = 0;
        txsnp_flit->SrcID       = 456; //TODO
        txsnp_flit->TxnID       = 0;
        txsnp_flit->FwdNID      = 0;
        txsnp_flit->CF1         = 0;
        txsnp_flit->Opcode      = SetSnpOpcode();
        txsnp_flit->Addr        = accreq_flit->addr;
        txsnp_flit->NS          = 0;
        txsnp_flit->DoNotGoToSD = 1;
        txsnp_flit->RetToSrc    = 0;
        txsnp_flit->TraceTag    = 0;
        txsnp_flit_queue.push(txsnp_flit);
    }
}




bool IAccHandle::IsDone()
{
    bool done = set_CHITxSNP & set_ACCRSP;
    if(done)
    {
        mem_line->acc_is_pending = false;
    }
    return done;
}



}
#endif