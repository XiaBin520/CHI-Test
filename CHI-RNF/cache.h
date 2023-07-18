#ifndef CHI_RNF_CACHE_H_
#define CHI_RNF_CACHE_H_

#include "cacheline.h"

namespace CHI_RNF
{

class Cache
{
private:
    vector<CacheLine*> cache_line_vec;


    uint32_t FindTag(uint64_t tag)
    {
        uint32_t num_line = cache_line_vec.size();
        for(uint32_t i = 0; i < num_line; i++)
        {
            CacheLine *line = cache_line_vec[i];
            if(line->ReadTag() == tag) return i;
        }
        return -1;
    }




public:
    CacheLine* FindCacheLine(uint64_t addr)
    {
        uint64_t tag = addr >> 6;
        uint32_t index = FindTag(tag);
        if(index == -1) return NULL;

        return cache_line_vec[index]; 
    }


    CacheLine* NewCacheLine(uint64_t addr)
    {
        uint64_t tag = addr >> 6;
        uint32_t index = FindTag(tag);
        if(index != -1)
        {
            printf("Create cache line error!\n");
            printf("The line has been create!\n");
            printf("Creating cache line by addr = 0x%lx\n", addr);
            assert(0);
        }

        CacheLine *line = new CacheLine(tag);
        cache_line_vec.push_back(line);
        return line;
    }


    void CleanInvalidCacheLine()
    {
        for(uint32_t i = 0; i < cache_line_vec.size();)
        {
            CacheLine *line = cache_line_vec[i];
            if(!line->InvalidCacheLine())
            {
                i++;
                continue;
            }

            cache_line_vec.erase(cache_line_vec.begin() + i);
            delete line;
        }
    }

};


}
#endif