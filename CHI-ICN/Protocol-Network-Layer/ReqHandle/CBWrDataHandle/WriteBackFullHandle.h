#ifndef CHI_HNF_WRITEBACKFULLHANDLE_H_
#define CHI_HNF_WRITEBACKFULLHANDLE_H_

#include "ICBWrDataHandle.h"


namespace CHI_HNF
{

class WriteBackFullHandle : public ICBWrDataHandle
{
public:
    WriteBackFullHandle(uint32_t dbid, REQFLIT *rxreq_flit, MemLine *mem_line);
};


WriteBackFullHandle::WriteBackFullHandle(uint32_t dbid, REQFLIT *rxreq_flit, MemLine *mem_line)
{
    CBWrDataInit(dbid, rxreq_flit, mem_line);
}


}
#endif