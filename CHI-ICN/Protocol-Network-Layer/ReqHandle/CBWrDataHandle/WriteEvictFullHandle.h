#ifndef CHI_HNF_WRITEEVICTFULLHANDLE_H_
#define CHI_HNF_WRITEEVICTFULLHANDLE_H_

#include "ICBWrDataHandle.h"

namespace CHI_HNF
{

class WriteEvictFullHandle : public ICBWrDataHandle
{
public:
    WriteEvictFullHandle(uint32_t dbid, REQFLIT *rxreq_flit, MemLine *mem_line);
};


WriteEvictFullHandle::WriteEvictFullHandle(uint32_t dbid, REQFLIT *rxreq_flit, MemLine *mem_line)
{
    CBWrDataInit(dbid, rxreq_flit, mem_line);
}


}
#endif