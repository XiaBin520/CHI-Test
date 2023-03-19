#include<stdint.h>
#include<queue>
#include<vector>
#include<assert.h>

using namespace std;


// 将值 value 转化为二进制数，按位存入到 flit_bits 中
void ValueToBits(uint64_t value, uint32_t width, queue<bool>& flit_bits)
{
    for(uint32_t i = 0; i < width; i++)
    {
        bool bit = (1 << i) & value;
        flit_bits.push(bit);
    }
}
// 从 flit_bits 中提取指定位宽的数据
uint64_t BitsToValue(queue<bool>& flit_bits, uint32_t width)
{
    uint64_t data = 0;
    for(uint32_t i = 0; i < width; i++)
    {
        if(flit_bits.empty()) break;
        if(flit_bits.front()) data = data + (1 << i);
        flit_bits.pop();
    }
    return data;
}
