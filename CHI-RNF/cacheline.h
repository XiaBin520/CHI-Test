#ifndef CHI_RNF_CACHELINE_H_
#define CHI_RNF_CACHELINE_H_

#include "common.h"
#include "cache_field_encoding.h"


namespace CHI_RNF
{

class CacheLine
{
private:
    uint32_t    state;
    uint64_t    tag;
    uint8_t     data[64];

public:
    bool acc_is_pending;
    bool req_is_pending;
    bool snp_is_pending;
    bool req_one_data_response;
    bool req_all_data_response;
    bool snp_response_is_locked;

public:
    CacheLine(uint64_t tag);

    uint32_t    ReadState();
    uint64_t    ReadTag();
    void        ReadData(uint8_t data_read[64]);

    void WriteState(uint32_t state_write);
    void WriteTag(uint64_t tag_write);
    void WriteData(uint8_t data_write[64], uint64_t write_enable);

    bool InvalidCacheLine();
};




/******************************************************/
/*  Constructor                                       */
/******************************************************/
CacheLine::CacheLine(uint64_t tag)
{
    this->state = CacheStateEncoding::Invalid;
    this->tag   = tag;
    memset(data, 0, 64);

    this->acc_is_pending = false;
    this->req_is_pending = false;
    this->snp_is_pending = false;
    this->req_one_data_response  = false;
    this->req_all_data_response  = false;
    this->snp_response_is_locked = false;
}





/******************************************************/
/*  Read Functions                                    */
/******************************************************/
uint32_t CacheLine::ReadState() {return state;}
uint64_t CacheLine::ReadTag() {return tag;}
void CacheLine::ReadData(uint8_t data_read[64]) {memmove(data_read, data, 64);}





/******************************************************/
/*  Write Functions                                   */
/******************************************************/
void CacheLine::WriteState(uint32_t state_write) {state = state_write;}
void CacheLine::WriteTag(uint64_t tag_write) {tag = tag_write;}

void CacheLine::WriteData(uint8_t data_write[64], uint64_t write_enable)
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





/******************************************************/
/*  Check if cache line is invalid                    */
/******************************************************/
bool CacheLine::InvalidCacheLine()
{
    bool invalid_state = (state == CacheStateEncoding::Invalid);
    bool invalid_ctrl  = (!acc_is_pending) &
                         (!req_is_pending) &
                         (!snp_is_pending) &
                         (!req_one_data_response) &
                         (!req_all_data_response) &
                         (!snp_response_is_locked);

    bool invalid_line = (invalid_state & invalid_ctrl);
    return invalid_line;
}


}
#endif