#ifndef CHI_RNF_WRITEUNIQUEPTLHANDLE_H_
#define CHI_RNF_WRITEUNIQUEPTLHANDLE_H_

#include "INCBWrDataHandle.h"

namespace CHI_RNF
{

class WriteUniquePtlHandle : public INCBWrDataHandle
{
public:
    WriteUniquePtlHandle(ACCFLIT *accreq_flit, REQFLIT *txreq_flit, CacheLine *cache_line);
};


WriteUniquePtlHandle::WriteUniquePtlHandle(ACCFLIT *accreq_flit, REQFLIT *txreq_flit, CacheLine *cache_line)
{
    InitNCBWrDataHandle(accreq_flit, txreq_flit, cache_line);
}



}
#endif
