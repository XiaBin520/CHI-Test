#ifndef CHI_RNF_ACC_FIELDS_ENCODING_H_
#define CHI_RNF_ACC_FIELDS_ENCODING_H_


namespace CHI_RNF
{


namespace AccTypeEncoding
{
    enum {
        None = 0, 
        AccRead, 
        AccWrite_Allocate, 
        AccWrite_NoAllocate,
        AccFlush,
    };
};

}
#endif