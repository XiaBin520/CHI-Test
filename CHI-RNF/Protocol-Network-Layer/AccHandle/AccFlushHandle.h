#ifndef CHI_RNF_ACCFLUSHHANDLE_H_
#define CHI_RNF_ACCFLUSHHANDLE_H_

#include "IAccHandle.h"



namespace CHI_RNF
{


class AccFlushHandle : public IAccHandle
{
private:
    bool set_CHITxREQ;
    bool set_ACCRSP;
private:
    bool handle_begin;


public:
    AccFlushHandle(CacheLine *cache_line, ACCFLIT *accreq_flit);

    virtual void DriverTxREQ(queue<REQFLIT*>& txreq_flit_queue, queue<ACCFLIT*>& accreq_flit_queue);
    virtual void DriverAccRSP(queue<ACCFLIT*>& accrsp_flit_queue);
    virtual bool IsDone();
};




/******************************************************/
/*  AccFlushHandle Constructor                        */
/******************************************************/
AccFlushHandle::AccFlushHandle(CacheLine *cache_line, ACCFLIT *accreq_flit)
{
    this->cache_line = cache_line;
    this->accreq_flit = accreq_flit;

    this->age = 0;
    this->handle_begin = false;
    this->cache_line->acc_is_pending = true;

    set_CHITxREQ = false;
    set_ACCRSP   = false;
}





/******************************************************/
/*  AccFlushHandle Driver CHI TxREQ                   */
/******************************************************/
void AccFlushHandle::DriverTxREQ(queue<REQFLIT*>& txreq_flit_queue, queue<ACCFLIT*>& accreq_flit_queue)
{
    if(cache_line->req_is_pending) return;
    if(set_CHITxREQ) return;

    uint32_t cache_state = cache_line->ReadState();
    switch(cache_state)
    {
        case CacheStateEncoding::UC:
        case CacheStateEncoding::UD:
        case CacheStateEncoding::SC:
        case CacheStateEncoding::UCE:
            set_CHITxREQ = true;
            break;
        default:
            set_CHITxREQ = false;
            break;
    }

    if(set_CHITxREQ)
    {
        REQFLIT *txreq_flit = new REQFLIT();
        switch(cache_state)
        {
            case CacheStateEncoding::UC:
                if(rand()%2 == 0) SetCHITxREQ_Evict(txreq_flit);
                else SetCHITxREQ_WriteEvictFull(txreq_flit);
                break;
            case CacheStateEncoding::UD:
                if(rand()%2 == 0) SetCHITxREQ_WriteBackFull(txreq_flit);
                else SetCHITxREQ_WriteCleanFull(txreq_flit);
                break;
            case CacheStateEncoding::SC:
            case CacheStateEncoding::UCE:
                SetCHITxREQ_Evict(txreq_flit);
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
/*  AccFlushHandle Driver ACC RSP                     */
/******************************************************/
void AccFlushHandle::DriverAccRSP(queue<ACCFLIT*>& acc_rsp_queue)
{
    if(cache_line->req_is_pending) return;
    if(cache_line->snp_is_pending) return;
    if(!cache_line->acc_is_pending) return;

    if(set_ACCRSP) return;

    uint32_t cache_state = cache_line->ReadState();
    switch(cache_state)
    {
        case CacheStateEncoding::Invalid:
            set_ACCRSP = true;
            break;
        default:
            set_CHITxREQ = false;
            set_ACCRSP = false;
            break;
    }

    if(set_ACCRSP)
    {
        acc_rsp_queue.push(accreq_flit);
    }
}







/******************************************************/
/*  AccFlushHandle Done                                */
/******************************************************/
bool AccFlushHandle::IsDone()
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