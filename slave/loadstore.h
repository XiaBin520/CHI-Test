
#include<stdint.h>

enum{OP_Load = 1, OP_Store};

class LSReq
{
public:
    bool     valid;
    uint32_t opcode;
    uint64_t addr;
};