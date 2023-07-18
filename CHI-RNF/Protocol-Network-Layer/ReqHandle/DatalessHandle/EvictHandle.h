#ifndef CHI_RNF_EVICTHANDLE_H_
#define CHI_RNF_EVICTHANDLE_H_

#include "IDatalessHandle.h"

namespace CHI_RNF
{



class EvictHandle : public IDatalessHandle
{
private:
    virtual void UpdateCacheLine();

public:
    EvictHandle(ACCFLIT *accreq_flit, REQFLIT *txreq_flit, CacheLine *cache_line);
};






EvictHandle::EvictHandle(ACCFLIT *accreq_flit, REQFLIT *txreq_flit, CacheLine *cache_line)
{
    InitDatalessHandle(accreq_flit, txreq_flit, cache_line);

    switch(cache_line->ReadState())
    {
        case CacheStateEncoding::UC:
        case CacheStateEncoding::SC:
        case CacheStateEncoding::UCE:
            this->cache_line->WriteState(CacheStateEncoding::Invalid);
            break;
        default:
            assert(0);
            break;
    }
}







void EvictHandle::UpdateCacheLine()
{
    // NOP
    return;
}


}
#endif