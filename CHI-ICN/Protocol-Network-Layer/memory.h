#ifndef CHI_HNF_MEMORY_H_
#define CHI_HNF_MEMORY_H_

#include "memline.h"

namespace CHI_HNF
{



class Memory
{
private:
    MemLine *mem_line_arr[1024]; // 1024 * 64B = 64KB

public:
    Memory()
    {
        for(uint32_t i = 0; i < 1024; i++)
        {
            MemLine *mem_line = new MemLine(i);
            mem_line_arr[i] = mem_line;
        }
    }


    MemLine* FindMemLine(uint64_t addr)
    {
        uint64_t index = addr >> 6;
        return mem_line_arr[index];
    }


    void PrintMemory(uint8_t mem_data[512][64])
    {
        for(uint32_t i = 0; i < 512; i++)
        {
            MemLine *mem_line = mem_line_arr[i];
            mem_line->ReadData(mem_data[i]);
        }
    }
};




}
#endif