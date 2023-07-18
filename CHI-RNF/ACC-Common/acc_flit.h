#ifndef CHI_RNF_ACC_FLIT_H_
#define CHI_RNF_ACC_FLIT_H_


#include "../CHI-Common/chi_flit.h"
#include "acc_fields_encoding.h"


namespace CHI_RNF
{


class ACCFLIT
{
public:
    uint32_t    acc_id;
    uint32_t    type;
    uint64_t    addr;
    uint8_t     data[64];
    uint64_t    write_enable;
};



}
#endif