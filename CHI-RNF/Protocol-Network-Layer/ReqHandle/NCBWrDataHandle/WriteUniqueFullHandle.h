#ifndef CHI_RNF_WRITEUNIQUEFULLHANDLE_H_
#define CHI_RNF_WRITEUNIQUEFULLHANDLE_H_

#include "INCBWrDataHandle.h"

namespace CHI_RNF
{

class WriteUniqueFullHandle : public INCBWrDataHandle
{
public:
    WriteUniqueFullHandle(ACCFLIT *accreq_flit, REQFLIT *txreq_flit, CacheLine *cache_line);
};


WriteUniqueFullHandle::WriteUniqueFullHandle(ACCFLIT *accreq_flit, REQFLIT *txreq_flit, CacheLine *cache_line)
{
    InitNCBWrDataHandle(accreq_flit, txreq_flit, cache_line);
}



}
#endif
