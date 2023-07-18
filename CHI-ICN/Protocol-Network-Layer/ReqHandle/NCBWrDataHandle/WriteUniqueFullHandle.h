#ifndef CHI_HNF_WRITEUNIQUEFULLHANDLE_H_
#define CHI_HNF_WRITEUNIQUEFULLHANDLE_H_

#include "INCBWrDataHandle.h"


namespace CHI_HNF
{

class WriteUniqueFullHandle : public INCBWrDataHandle
{
public:
    WriteUniqueFullHandle(uint32_t dbid, REQFLIT *rxreq_flit, MemLine *mem_line);
};


WriteUniqueFullHandle::WriteUniqueFullHandle(uint32_t dbid, REQFLIT *rxreq_flit, MemLine *mem_line)
{
    NCBWrDataInit(dbid, rxreq_flit, mem_line);
}


}
#endif