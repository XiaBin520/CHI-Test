#ifndef CHI_RNF_ACCWRITENOALLOCATEHANDLE_H_
#define CHI_RNF_ACCWRITENOALLOCATEHANDLE_H_

#include "IAccHandle.h"


namespace CHI_RNF
{



class AccWrNoAllocHandle : public IAccHandle
{
private:
    bool set_ACCRSP;
    bool set_CHITxREQ;
    bool CHITxREQ_is_WriteUnique;
private:
    bool handle_begin;


public:
    AccWrNoAllocHandle(CacheLine *cache_line, ACCFLIT *accreq_flit);

    virtual void DriverTxREQ(queue<REQFLIT*>& txreq_flit_queue, queue<ACCFLIT*>& accreq_flit_queue);
    virtual void DriverAccRSP(queue<ACCFLIT*>& accrsp_flit_queue);
    virtual bool IsDone();
};


/******************************************************/
/*  AccWrNoAllocHandle Constructor              */
/******************************************************/
AccWrNoAllocHandle::AccWrNoAllocHandle(CacheLine *cache_line, ACCFLIT *accreq_flit)
{
    this->cache_line = cache_line;
    this->accreq_flit = accreq_flit;

    this->age = 0;
    this->handle_begin = false;
    this->cache_line->acc_is_pending = true;

    set_ACCRSP   = false;
    set_CHITxREQ = false;
    CHITxREQ_is_WriteUnique = false;
}





/******************************************************/
/*  AccWrNoAllocHandle Driver CHI TxREQ         */
/******************************************************/
void AccWrNoAllocHandle::DriverTxREQ(queue<REQFLIT*>& txreq_flit_queue, queue<ACCFLIT*>& accreq_flit_queue)
{
    if(cache_line->req_is_pending) return;
    if(set_CHITxREQ) return;

    uint32_t cache_state = cache_line->ReadState();
    switch(cache_state)
    {
        case CacheStateEncoding::Invalid:
        case CacheStateEncoding::UC:
        case CacheStateEncoding::UD:
        case CacheStateEncoding::SC:
        case CacheStateEncoding::UCE:
            set_CHITxREQ = true;
            break;
        default:
            assert(0);
            break;
    }

    if(set_CHITxREQ)
    {
        REQFLIT *txreq_flit = new REQFLIT();
        switch(cache_state)
        {   case CacheStateEncoding::Invalid:
                if(accreq_flit->write_enable == -1)
                    SetCHITxREQ_WriteUniqueFull(txreq_flit);
                else
                    SetCHITxREQ_WriteUniquePtl(txreq_flit);
                CHITxREQ_is_WriteUnique = true;
                break;
            case CacheStateEncoding::UC:
            case CacheStateEncoding::SC:
            case CacheStateEncoding::UCE:
                SetCHITxREQ_Evict(txreq_flit);
                break;
            case CacheStateEncoding::UD:
                SetCHITxREQ_WriteBackFull(txreq_flit);
                break;
            default:
                assert(0);
                break;
        }
        txreq_flit_queue.push(txreq_flit);
        accreq_flit_queue.push(accreq_flit);

        handle_begin = true;
    }
}





/******************************************************/
/*  AccWrNoAllocHandle Driver ACC RSP           */
/******************************************************/
void AccWrNoAllocHandle::DriverAccRSP(queue<ACCFLIT*>& accrsp_flit_queue)
{
    if(cache_line->req_is_pending) return;
    if(cache_line->snp_is_pending) return;
    if(!cache_line->acc_is_pending) return;

    if(set_ACCRSP) return;

    if(set_CHITxREQ)
    {
        if(CHITxREQ_is_WriteUnique)
        {
            set_ACCRSP = true;
        }
        else
        {
            set_CHITxREQ = false;
        }
    }

    if(set_ACCRSP)
    {
        accrsp_flit_queue.push(accreq_flit);
    }
}





/******************************************************/
/*  AccWrNoAllocHandle Done                     */
/******************************************************/
bool AccWrNoAllocHandle::IsDone()
{
    if(handle_begin)
    {
        age++;
        if(age >= 2000) assert(0);
    }
    
    bool done = set_ACCRSP;
    if(done)
    {
        cache_line->acc_is_pending = false;
    }
    return done;
}



}
#endif