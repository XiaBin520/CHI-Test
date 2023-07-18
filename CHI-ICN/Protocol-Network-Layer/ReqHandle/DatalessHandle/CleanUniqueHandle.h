#ifndef CHI_HNF_CLEANUNIQUEHANDLE_H_
#define CHI_HNF_CLEANUNIQUEHANDLE_H_

#include "IDatalessHandle.h"

namespace CHI_HNF
{


class CleanUniqueHandle : public IDatalessHandle
{
private:
    uint32_t SetResp();

public:
    CleanUniqueHandle(uint32_t dbid, REQFLIT *rxreq_flit, MemLine *mem_line);
};





CleanUniqueHandle::CleanUniqueHandle(uint32_t dbid, REQFLIT *rxreq_flit, MemLine *mem_line)
{
    DatalessInit(dbid, rxreq_flit, mem_line);
}



uint32_t CleanUniqueHandle::SetResp()
{
    uint32_t resp = RespEncoding::CompResp::UC;
    return resp;
}



}
#endif