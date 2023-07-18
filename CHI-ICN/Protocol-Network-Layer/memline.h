#ifndef CHI_HNF_MEMLINE_H_
#define CHI_HNF_MEMLINE_H_

#include "../common.h"

namespace CHI_HNF
{



class MemLine
{
private:
    uint64_t    tag;
    uint8_t     data[64];


public:
    bool acc_is_pending;
    bool req_is_pending;
    bool snp_is_pending;
    bool req_completion_is_sent;

public:
    MemLine(uint64_t tag);

    uint64_t ReadTag();
    void     ReadData(uint8_t data_read[64]);

    void WriteTag(uint64_t tag_write);
    void WriteData(uint8_t data_write[64], uint64_t write_enable);
};




/******************************************************/
/*  Constructor                                       */
/******************************************************/
MemLine::MemLine(uint64_t tag)
{
    this->tag   = tag;
    memset(data, 0, 64);

    acc_is_pending = false;
    req_is_pending = false;
    snp_is_pending = false;
    req_completion_is_sent = false;
}





/******************************************************/
/*  Read Functions                                    */
/******************************************************/
uint64_t MemLine::ReadTag() {return tag;}
void MemLine::ReadData(uint8_t data_read[64]) {memmove(data_read, data, 64);}





/******************************************************/
/*  Write Functions                                   */
/******************************************************/
void MemLine::WriteTag(uint64_t tag_write) {tag = tag_write;}

void MemLine::WriteData(uint8_t data_write[64], uint64_t write_enable)
{
    if(write_enable == 0) return;
    
    if(write_enable == -1)
    {
        memmove(data, data_write, 64);
        return;
    }

    for(uint32_t i = 0; i < 64; i++)
    {
        bool write_en = (write_enable >> i) & 0b1;
        if(write_en)
        {
            data[i] = data_write[i];
        }
    }
}


}
#endif