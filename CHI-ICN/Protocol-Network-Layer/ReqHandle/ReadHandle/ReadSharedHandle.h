#ifndef CHI_HNF_READSHAREDHANDLE_H_
#define CHI_HNF_READSHAREDHANDLE_H_

#include "IReadHandle.h"

namespace CHI_HNF
{


class ReadSharedHandle : public IReadHandle
{
private:
    uint32_t SetResp();

public:
    ReadSharedHandle(uint32_t dbid, REQFLIT *rxreq_flit, MemLine *mem_line);
};




ReadSharedHandle::ReadSharedHandle(uint32_t dbid, REQFLIT *rxreq_flit, MemLine *mem_line)
{
    ReadInit(dbid, rxreq_flit, mem_line);
}




uint32_t ReadSharedHandle::SetResp()
{
    uint32_t resp = (rand()%2 == 0) ? RespEncoding::CompResp::UC :
                    (rand()%2 == 0) ? RespEncoding::CompResp::SC :
                                      RespEncoding::CompResp::UD_PD;
    return resp;
}

}
#endif