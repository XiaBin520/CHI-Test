#ifndef CHI_HNF_WRITEUNIQUEPTLHANDLE_H_
#define CHI_HNF_WRITEUNIQUEPTLHANDLE_H_

#include "INCBWrDataHandle.h"

namespace CHI_HNF
{

class WriteUniquePtlHandle : public INCBWrDataHandle
{
public:
    WriteUniquePtlHandle(uint32_t dbid, REQFLIT *rxreq_flit, MemLine *mem_line);
};


WriteUniquePtlHandle::WriteUniquePtlHandle(uint32_t dbid, REQFLIT *rxreq_flit, MemLine *mem_line)
{
    NCBWrDataInit(dbid, rxreq_flit, mem_line);
}


}
#endif