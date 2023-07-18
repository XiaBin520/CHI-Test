#ifndef CHI_RNF_WRITEBACKFULLHANDLE_H_
#define CHI_RNF_WRITEBACKFULLHANDLE_H_

#include "ICBWrDataHandle.h"

namespace CHI_RNF
{


class WriteBackFullHandle : public ICBWrDataHandle
{
private:
    virtual uint32_t SetCBWrDataResp();
    virtual void UpdateCacheLine();

public:
    WriteBackFullHandle(ACCFLIT *accreq_flit, REQFLIT *txreq_flit, CacheLine *cache_line);
};







WriteBackFullHandle::WriteBackFullHandle(ACCFLIT *accreq_flit, REQFLIT *txreq_flit, CacheLine *cache_line)
{
    InitCBWrDataHandle(accreq_flit, txreq_flit, cache_line);
}






uint32_t WriteBackFullHandle::SetCBWrDataResp()
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
        case CacheStateEncoding::UD:
            resp = RespEncoding::WriteDataResp::UD_PD;
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



void WriteBackFullHandle::UpdateCacheLine()
{
    cache_line->WriteState(CacheStateEncoding::Invalid);
}



}
#endif