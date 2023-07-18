#ifndef CHI_HNF_EVICTHANDLE_H_
#define CHI_HNF_EVICTHANDLE_H_

#include "IDatalessHandle.h"


namespace CHI_HNF
{



class EvictHandle : public IDatalessHandle
{
private:
    uint32_t SetResp();

public:
    EvictHandle(uint32_t dbid, REQFLIT *rxreq_flit, MemLine *mem_line);
};





EvictHandle::EvictHandle(uint32_t dbid, REQFLIT *rxreq_flit, MemLine *mem_line)
{
    DatalessInit(dbid, rxreq_flit, mem_line);
}



uint32_t EvictHandle::SetResp()
{
    uint32_t resp = RespEncoding::CompResp::I;
    return resp;
}



}
#endif