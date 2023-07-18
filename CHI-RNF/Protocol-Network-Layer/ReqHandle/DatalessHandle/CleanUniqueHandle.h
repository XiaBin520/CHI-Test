#ifndef CHI_RNF_CLEANUNIQUEHANDLE_H_
#define CHI_RNF_CLEANUNIQUEHANDLE_H_

#include "IDatalessHandle.h"


namespace CHI_RNF
{

class CleanUniqueHandle : public IDatalessHandle
{
private:
    virtual void UpdateCacheLine();

public:
    CleanUniqueHandle(ACCFLIT *accreq_flit, REQFLIT *txreq_flit, CacheLine *cache_line);
};






CleanUniqueHandle::CleanUniqueHandle(ACCFLIT *accreq_flit, REQFLIT *txreq_flit, CacheLine *cache_line)
{
    InitDatalessHandle(accreq_flit, txreq_flit, cache_line);
}







#define CleanUniqueUpdateCacheState(Init_State, Resp_State, Final_State)   \
            else if((cache_line->ReadState() == CacheStateEncoding::Init_State) & \
                    (rxrsp_flit->Resp == RespEncoding::CompResp::Resp_State)) \
            { \
                cache_line->WriteState(CacheStateEncoding::Final_State); \
            }

void CleanUniqueHandle::UpdateCacheLine()
{
    RSPFLIT *rxrsp_flit = got_rxrsp_flit_vec.back();
    if(false);
    //                         (Init State, Resp State) --> (Final State)
    CleanUniqueUpdateCacheState(Invalid, UC,         UCE)
    CleanUniqueUpdateCacheState(UC,      UC,         UC)
    CleanUniqueUpdateCacheState(UD,      UC,         UD)
    CleanUniqueUpdateCacheState(SC,      UC,         UC)
    CleanUniqueUpdateCacheState(UCE,     UC,         UC)
    else assert(0);
}


}
#endif