#ifndef CHI_RNF_READCLEANHANDLE_H_
#define CHI_RNF_READCLEANHANDLE_H_

#include "IReadHandle.h"

namespace CHI_RNF
{


class ReadCleanHandle : public IReadHandle
{
private:
    virtual void UpdateCacheLine();

public:
    ReadCleanHandle(ACCFLIT *accreq_flit, REQFLIT *txreq_flit, CacheLine *cache_line);
};




ReadCleanHandle::ReadCleanHandle(ACCFLIT *accreq_flit, REQFLIT *txreq_flit, CacheLine *cache_line)
{
    InitReadHandle(accreq_flit, txreq_flit, cache_line);
}










#define ReadCleanUpdateCacheState(Init_State, Resp_State, Final_State) \
            else if((cache_line->ReadState() == CacheStateEncoding::Init_State) & \
                    (rxdat_flit->Resp == RespEncoding::CompResp::Resp_State)) \
            { \
                cache_line->WriteState(CacheStateEncoding::Final_State); \
            }

void ReadCleanHandle::UpdateCacheLine()
{
    // Update Cache Line State
    DATFLIT *rxdat_flit = got_rxdat_flit_vec.back();
    if(false);
    //                       (Init State, Resp State) --> (Final State)
    ReadCleanUpdateCacheState(Invalid, SC,      SC)
    ReadCleanUpdateCacheState(Invalid, UC,      UC)
    ReadCleanUpdateCacheState(UCE,     SC,      UC)
    ReadCleanUpdateCacheState(UCE,     UC,      UC)
    else assert(0);

    // Update Cache Line Data
    uint8_t data_write[64];
    uint32_t num_wrap = got_rxdat_flit_vec.size();
    for(uint32_t i = 0; i < num_wrap; i++)
    {
        DATFLIT *temp_rxdat = got_rxdat_flit_vec[i];
        uint32_t offset = temp_rxdat->DataID * (Data_Width/8);
        memmove(&data_write[offset], temp_rxdat->Data, (Data_Width/8));
    }
    cache_line->WriteData(data_write, -1);
}





}
#endif
