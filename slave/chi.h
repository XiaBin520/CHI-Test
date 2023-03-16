#include<stdint.h>
#include<math.h>

#include "chi_config.h"

template<uint32_t BITS = 256>
class IChannel
{
public:
    bool        flitpend;
    bool        flitv;
    uint32_t    flit[BITS/32];
    bool        lcrdv;
};

typedef IChannel<(uint32_t)ceil(REQConfig::Total_Width / 32)> REQChannel;
typedef IChannel<(uint32_t)ceil(RSPConfig::Total_Width / 32)> RSPChannel;
typedef IChannel<(uint32_t)ceil(SNPConfig::Total_Width / 32)> SNPChannel;
typedef IChannel<(uint32_t)ceil(DATConfig::Total_Width / 32)> DATChannel;

