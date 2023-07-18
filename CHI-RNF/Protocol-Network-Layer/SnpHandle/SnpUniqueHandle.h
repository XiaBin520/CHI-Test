#ifndef CHI_RNF_SNPUNIQUEHANDLE_H_
#define CHI_RNF_SNPUNIQUEHANDLE_H_

#include "SnoopHandle.h"

namespace CHI_RNF
{

class SnpUniqueHandle : public SnoopHandle
{
private:
    virtual void SetNeedResponse();


public:
    SnpUniqueHandle(SNPFLIT *rxsnp_flit, CacheLine *cache_line);
};


SnpUniqueHandle::SnpUniqueHandle(SNPFLIT *rxsnp_flit, CacheLine *cache_line)
{
    SnoopInit(rxsnp_flit, cache_line);
    SetNeedResponse();
}



void SnpUniqueHandle::SetNeedResponse()
{
    bool ret_to_src = got_rxsnp_flit->RetToSrc;

    switch(cache_line->ReadState())
    {
        case CacheStateEncoding::Invalid:
            need_SnpResp = true;
            need_Resp    = RespEncoding::SnoopResp::I;
            break;

        case CacheStateEncoding::UC:
            if(ret_to_src) need_SnpRespData = true;
            else need_SnpResp = true;
            need_Resp = RespEncoding::SnoopResp::I;
            break;

        case CacheStateEncoding::UD:
            need_SnpRespData = true;
            need_Resp        = RespEncoding::SnoopResp::I_PD;
            break;

        case CacheStateEncoding::SC:
            if(ret_to_src) need_SnpRespData = true;
            else need_SnpResp = true;
            need_Resp = RespEncoding::SnoopResp::I;
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
