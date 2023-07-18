#ifndef CHI_RNF_WRITECLEANFULLHANDLE_H_
#define CHI_RNF_WRITECLEANFULLHANDLE_H_

#include "ICBWrDataHandle.h"

namespace CHI_RNF
{


class WriteCleanFullHandle : public ICBWrDataHandle
{
private:
    virtual uint32_t SetCBWrDataResp();
    virtual void UpdateCacheLine();

public:
    WriteCleanFullHandle(ACCFLIT *accreq_flit, REQFLIT *txreq_flit, CacheLine *cache_line);
};







WriteCleanFullHandle::WriteCleanFullHandle(ACCFLIT *accreq_flit, REQFLIT *txreq_flit, CacheLine *cache_line)
{
    InitCBWrDataHandle(accreq_flit, txreq_flit, cache_line);
}






uint32_t WriteCleanFullHandle::SetCBWrDataResp()
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



void WriteCleanFullHandle::UpdateCacheLine()
{
    if(got_txdat_flit_vec.empty()) assert(0);

    DATFLIT *txdat_flit = got_txdat_flit_vec.back();
    switch(txdat_flit->Resp)
    {
        case RespEncoding::WriteDataResp::I:
            cache_line->WriteState(CacheStateEncoding::Invalid);
            break;
        case RespEncoding::WriteDataResp::UC:
            cache_line->WriteState(CacheStateEncoding::UC);
            break;
        case RespEncoding::WriteDataResp::UD_PD:
            cache_line->WriteState(CacheStateEncoding::UC);
            break;
        case RespEncoding::WriteDataResp::SC:
            cache_line->WriteState(CacheStateEncoding::SC);
            break;
        default:
            assert(0);
            break;
    }
}




}
#endif