#ifndef CHI_HNF_WRITEBACKPTLHANDLE_H_
#define CHI_HNF_WRITEBACKPTLHANDLE_H_

#include "ICBWrDataHandle.h"


namespace CHI_HNF
{

class WriteBackPtlHandle : public ICBWrDataHandle
{
public:
    WriteBackPtlHandle(uint32_t dbid, REQFLIT *rxreq_flit, MemLine *mem_line);
};


WriteBackPtlHandle::WriteBackPtlHandle(uint32_t dbid, REQFLIT *rxreq_flit, MemLine *mem_line)
{
    CBWrDataInit(dbid, rxreq_flit, mem_line);
}


}
#endif