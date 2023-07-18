#ifndef CHI_RNF_MAKEUNIQUEHANDLE_H_
#define CHI_RNF_MAKEUNIQUEHANDLE_H_

#include "IDatalessHandle.h"


namespace CHI_RNF
{



class MakeUniqueHandle : public IDatalessHandle
{
private:
    virtual void UpdateCacheLine();

public:
    MakeUniqueHandle(ACCFLIT *accreq_flit, REQFLIT *txreq_flit, CacheLine *cache_line);
};






MakeUniqueHandle::MakeUniqueHandle(ACCFLIT *accreq_flit, REQFLIT *txreq_flit, CacheLine *cache_line)
{
    InitDatalessHandle(accreq_flit, txreq_flit, cache_line);
}







#define MakeUniqueUpdateCacheState(Init_State, Resp_State, Final_State)   \
            else if((cache_line->ReadState() == CacheStateEncoding::Init_State) & \
                    (rxrsp_flit->Resp == RespEncoding::CompResp::Resp_State)) \
            { \
                cache_line->WriteState(CacheStateEncoding::Final_State); \
            }

void MakeUniqueHandle::UpdateCacheLine()
{
    RSPFLIT *rxrsp_flit = got_rxrsp_flit_vec.back();
    if(false);
    //                        (Init State, Resp State) --> (Final State)
    MakeUniqueUpdateCacheState(Invalid, UC,         UC)
    MakeUniqueUpdateCacheState(UC,      UC,         UC)
    MakeUniqueUpdateCacheState(SC,      UC,         UC)
    MakeUniqueUpdateCacheState(UCE,     UC,         UC)
    else assert(0);
}


}
#endif