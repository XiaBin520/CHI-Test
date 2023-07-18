#ifndef CHI_HNF_WRITECLEANFULLHANDLE_H_
#define CHI_HNF_WRITECLEANFULLHANDLE_H_

#include "ICBWrDataHandle.h"


namespace CHI_HNF
{

class WriteCleanFullHandle : public ICBWrDataHandle
{
public:
    WriteCleanFullHandle(uint32_t dbid, REQFLIT *rxreq_flit, MemLine *mem_line);
};


WriteCleanFullHandle::WriteCleanFullHandle(uint32_t dbid, REQFLIT *rxreq_flit, MemLine *mem_line)
{
    CBWrDataInit(dbid, rxreq_flit, mem_line);
}


}
#endif