#ifndef CHI_HNF_READUNIQUEHANDLE_H_
#define CHI_HNF_READUNIQUEHANDLE_H_

#include "IReadHandle.h"



namespace CHI_HNF
{



class ReadUniqueHandle : public IReadHandle
{
private:
    uint32_t SetResp();

public:
    ReadUniqueHandle(uint32_t dbid, REQFLIT *rxreq_flit, MemLine *mem_line);
};




ReadUniqueHandle::ReadUniqueHandle(uint32_t dbid, REQFLIT *rxreq_flit, MemLine *mem_line)
{
    ReadInit(dbid, rxreq_flit, mem_line);
}




uint32_t ReadUniqueHandle::SetResp()
{
    uint32_t resp = (rand()%2 == 0) ? RespEncoding::CompResp::UC :
                                      RespEncoding::CompResp::UD_PD;
    return resp;
}


}

#endif