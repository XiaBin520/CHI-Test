#ifndef CHI_HNF_READCLEANHANDLE_H_
#define CHI_HNF_READCLEANHANDLE_H_

#include "IReadHandle.h"

namespace CHI_HNF
{

class ReadCleanHandle : public IReadHandle
{
private:
    uint32_t SetResp();

public:
    ReadCleanHandle(uint32_t dbid, REQFLIT *rxreq_flit, MemLine *mem_line);
};




ReadCleanHandle::ReadCleanHandle(uint32_t dbid, REQFLIT *rxreq_flit, MemLine *mem_line)
{
    ReadInit(dbid, rxreq_flit, mem_line);
}



uint32_t ReadCleanHandle::SetResp()
{
    uint32_t resp = (rand()%2 == 0) ? RespEncoding::CompResp::UC :
                                      RespEncoding::CompResp::SC;
    return resp;
}



}
#endif