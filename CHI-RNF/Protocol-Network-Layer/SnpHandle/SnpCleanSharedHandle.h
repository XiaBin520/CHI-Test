#ifndef CHI_RNF_SNPCLEANSHAREDHANDLE_H_
#define CHI_RNF_SNPCLEANSHAREDHANDLE_H_

#include "SnoopHandle.h"

namespace CHI_RNF
{


class SnpCleanSharedHandle : public SnoopHandle
{
private:
    virtual void SetNeedResponse();


public:
    SnpCleanSharedHandle(SNPFLIT *rxsnp_flit, CacheLine *cache_line);
};




SnpCleanSharedHandle::SnpCleanSharedHandle(SNPFLIT *rxsnp_flit, CacheLine *cache_line)
{
    SnoopInit(rxsnp_flit, cache_line);
    SetNeedResponse();
}



void SnpCleanSharedHandle::SetNeedResponse()
{
    bool ret_to_src = got_rxsnp_flit->RetToSrc;

    switch(cache_line->ReadState())
    {
        case CacheStateEncoding::Invalid:
            need_SnpResp = true;
            need_Resp    = RespEncoding::SnoopResp::I;
            break;

        case CacheStateEncoding::UC:
            need_SnpResp = true;
            need_Resp    = RespEncoding::SnoopResp::I;
            break;

        case CacheStateEncoding::UD:
            need_SnpRespData = true;
            need_Resp        = RespEncoding::SnoopResp::UC_PD;
            break;

        case CacheStateEncoding::SC:
            need_SnpResp = true;
            need_Resp    = RespEncoding::SnoopResp::I;
            break;

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
