#ifndef CHI_RNF_SNPMAKEINVALIDHANDLE_H_
#define CHI_RNF_SNPMAKEINVALIDHANDLE_H_

#include "SnoopHandle.h"

namespace CHI_RNF
{


class SnpMakeInvalidHandle : public SnoopHandle
{
private:
    virtual void SetNeedResponse();


public:
    SnpMakeInvalidHandle(SNPFLIT *rxsnp_flit, CacheLine *cache_line);
};




SnpMakeInvalidHandle::SnpMakeInvalidHandle(SNPFLIT *rxsnp_flit, CacheLine *cache_line)
{
    SnoopInit(rxsnp_flit, cache_line);
    SetNeedResponse();
}



void SnpMakeInvalidHandle::SetNeedResponse()
{
    bool ret_to_src = got_rxsnp_flit->RetToSrc;

    switch(cache_line->ReadState())
    {
        case CacheStateEncoding::Invalid:
        case CacheStateEncoding::UC:
        case CacheStateEncoding::UD:
        case CacheStateEncoding::SC:
        case CacheStateEncoding::UCE:
            need_SnpResp = true;
            need_Resp    = RespEncoding::SnoopResp::I;
            break;
        
        default:
            assert(0);
            break;
    }
}





}
#endif
