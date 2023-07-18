#ifndef CHI_RNF_WRITEEVICTFULLHANDLE_H_
#define CHI_RNF_WRITEEVICTFULLHANDLE_H_

#include "ICBWrDataHandle.h"

namespace CHI_RNF
{


class WriteEvictFullHandle : public ICBWrDataHandle
{
private:
    virtual uint32_t SetCBWrDataResp();
    virtual void UpdateCacheLine();

public:
    WriteEvictFullHandle(ACCFLIT *accreq_flit, REQFLIT *txreq_flit, CacheLine *cache_line);
};







WriteEvictFullHandle::WriteEvictFullHandle(ACCFLIT *accreq_flit, REQFLIT *txreq_flit, CacheLine *cache_line)
{
    InitCBWrDataHandle(accreq_flit, txreq_flit, cache_line);
}






uint32_t WriteEvictFullHandle::SetCBWrDataResp()
{
    uint32_t resp = -1;
    switch(cache_line->ReadState())
    {
        case CacheStateEncoding::Invalid:
            resp = RespEncoding::WriteDataResp::I;
            break;
        case CacheStateEncoding::UC:
            resp = RespEncoding::WriteDataResp::UC;
            break;
        case CacheStateEncoding::SC:
            resp = RespEncoding::WriteDataResp::SC;
            break;
        default:
            assert(0);
            break;
    }
    return resp;
}



void WriteEvictFullHandle::UpdateCacheLine()
{
    cache_line->WriteState(CacheStateEncoding::Invalid);
}




}
#endif