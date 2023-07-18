#ifndef CHI_RNF_SNPQUERYHANDLE_H_
#define CHI_RNF_SNPQUERYHANDLE_H_

#include "SnoopHandle.h"

namespace CHI_RNF
{

class SnpQueryHandle : public SnoopHandle
{
private:
    virtual void SetNeedResponse();
    virtual void UpdateCacheLine(); // Note: SnpQuery Can't Update CacheLine


public:
    SnpQueryHandle(SNPFLIT *rxsnp_flit, CacheLine *cache_line);
};



SnpQueryHandle::SnpQueryHandle(SNPFLIT *rxsnp_flit, CacheLine *cache_line)
{
    SnoopInit(rxsnp_flit, cache_line);
    SetNeedResponse();
}






void SnpQueryHandle::UpdateCacheLine()
{
    //NOP
    return;
}




void SnpQueryHandle::SetNeedResponse()
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
            need_Resp    = RespEncoding::SnoopResp::UC;
            break;

        case CacheStateEncoding::UD:
            need_SnpResp = true;
            need_Resp    = RespEncoding::SnoopResp::UD;
            break;

        case CacheStateEncoding::SC:
            need_SnpResp = true;
            need_Resp    = RespEncoding::SnoopResp::SC;
            break;

        case CacheStateEncoding::UCE:
            need_SnpResp = true;
            need_Resp    = RespEncoding::SnoopResp::UC;
            break;
        
        default:
            assert(0);
            break;
    }
}





}
#endif
