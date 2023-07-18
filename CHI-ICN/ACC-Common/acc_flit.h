#ifndef CHI_HNF_ACC_FLIT_H_
#define CHI_HNF_ACC_FLIT_H_


#include "../CHI-Common/chi_flit.h"
#include "acc_fields_encoding.h"

namespace CHI_HNF
{

class ACCFLIT
{
public:
    uint32_t    acc_id;
    uint32_t    type;
    uint32_t    snp_opcode;
    uint64_t    addr;
    uint8_t     data[64];
    uint64_t    write_enable;
};



}
#endif