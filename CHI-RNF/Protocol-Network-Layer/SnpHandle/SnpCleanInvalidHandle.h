#ifndef CHI_RNF_SNPCLEANINVALIDHANDLE_H_
#define CHI_RNF_SNPCLEANINVALIDHANDLE_H_

#include "SnoopHandle.h"

namespace CHI_RNF
{


class SnpCleanInvalidHandle : public SnoopHandle
{
private:
    virtual void SetNeedResponse();


public:
    SnpCleanInvalidHandle(SNPFLIT *rxsnp_flit, CacheLine *cache_line);
};




SnpCleanInvalidHandle::SnpCleanInvalidHandle(SNPFLIT *rxsnp_flit, CacheLine *cache_line)
{
    SnoopInit(rxsnp_flit, cache_line);
    SetNeedResponse();
}



void SnpCleanInvalidHandle::SetNeedResponse()
{
    bool ret_to_src = got_rxsnp_flit->RetToSrc;

    switch(cache_line->ReadState())
    {
        case CacheStateEncoding::Invalid:
        case CacheStateEncoding::UC:
        case CacheStateEncoding::SC:
        case CacheStateEncoding::UCE:
            need_SnpResp = true;
            need_Resp    = RespEncoding::SnoopResp::I;
            break;

        case CacheStateEncoding::UD:
            need_SnpRespData = true;
            need_Resp        = RespEncoding::SnoopResp::I_PD;
            break;
        
        default:
            assert(0);
            break;
    }
}





}
#endif
