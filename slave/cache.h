#include<stdint.h>
#include<string.h>

enum{Owner_NONE = 0, Owner_LSREQ, Owner_CHIREQ};

class CacheLine
{
public:
    uint32_t    owner;
    uint32_t    data[16]; // 32 bits * 16 = 4B * 16 = 64B = 512 bits

    CacheLine()
    {
        owner = Owner_NONE;
    }

    void Write(uint32_t data[])
    {
        memcpy(this->data, data, sizeof(data));
    }
};


template<uint32_t size = 1024>
class Cache
{
private:
    CacheLine* cache[size]; // 1024 * 64B = 64 KB

public:
    Cache()
    {
        for(uint32_t i = 0; i < size; i++)
        {
            CacheLine *cacheline = new CacheLine();
        }
    }

    CacheLine* GetLine(uint64_t addr)
    {
        return cache[addr];
    }
};