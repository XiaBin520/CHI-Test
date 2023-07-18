#ifndef CHI_RNF_ACCREADHANDLE_H_
#define CHI_RNF_ACCREADHANDLE_H_


#include "IAccHandle.h"


namespace CHI_RNF
{


class AccReadHandle : public IAccHandle
{
private:
    bool set_CHITxREQ;
    bool set_ACCRSP;
private:
    bool handle_begin;

public:
    AccReadHandle(CacheLine *cache_line, ACCFLIT *accreq_flit);

    virtual void DriverTxREQ(queue<REQFLIT*>& txreq_flit_queue, queue<ACCFLIT*>& accreq_flit_queue);
    virtual void DriverAccRSP(queue<ACCFLIT*>& accrsp_flit_queue);
    virtual bool IsDone();
};




/******************************************************/
/*  AccReadHandle Constructor                         */
/******************************************************/
AccReadHandle::AccReadHandle(CacheLine *cache_line, ACCFLIT *accreq_flit)
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
/*  AccReadHandle Driver CHI TxREQ                    */
/******************************************************/
void AccReadHandle::DriverTxREQ(queue<REQFLIT*>& txreq_flit_queue, queue<ACCFLIT*>& accreq_flit_queue)
{
    if(cache_line->req_is_pending) return;
    if(set_CHITxREQ) return;

    uint32_t cache_state = cache_line->ReadState();
    switch(cache_state)
    {
        case CacheStateEncoding::Invalid:
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

        uint32_t rand_sel = rand() % 3;
        if(rand_sel == 0)      SetCHITxREQ_ReadClean(txreq_flit);
        else if(rand_sel == 1) SetCHITxREQ_ReadShared(txreq_flit);
        else                   SetCHITxREQ_ReadUnique(txreq_flit);

        txreq_flit_queue.push(txreq_flit);
        accreq_flit_queue.push(accreq_flit);

        handle_begin = true;
    }

}








/******************************************************/
/*  AccReadHandle Driver ACC RSP                      */
/******************************************************/
void AccReadHandle::DriverAccRSP(queue<ACCFLIT*>& acc_rsp_queue)
{
    if(cache_line->req_is_pending) return;
    if(cache_line->snp_is_pending) return;
    if(!cache_line->acc_is_pending) return;

    if(set_ACCRSP) return;


    uint32_t cache_state = cache_line->ReadState();
    switch(cache_state)
    {
        case CacheStateEncoding::UC:
        case CacheStateEncoding::UD:
        case CacheStateEncoding::SC:
            set_ACCRSP = true;
            break;
        default:
            set_CHITxREQ = false;
            set_ACCRSP = false;
            break;
    }

    if(set_ACCRSP)
    {
        cache_line->ReadData(accreq_flit->data);
        acc_rsp_queue.push(accreq_flit);
    }
}






/******************************************************/
/*  AccReadHandle Done                                */
/******************************************************/
bool AccReadHandle::IsDone()
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