#ifndef MAIN_TEST_H_
#define MAIN_TEST_H_


#include "../../CHI-RNF/Link-Layer/link_layer.h"
#include "../../CHI-ICN/Link-Layer/link_layer.h"


const uint32_t Num_Lines = 512;


class GoldenMem
{
public:
    uint8_t data[Num_Lines][64];

public:
    GoldenMem()
    {
        for(uint32_t i = 0; i < Num_Lines; i++)
        {
            memset(&data[i][0], 0, 64);
        }
    }

    void Write(uint32_t index, uint8_t data_write[64], uint64_t write_enables)
    {
        if(index >= Num_Lines) assert(0);
        for(uint32_t i = 0; i < 64; i++)
        {
            bool wen = (write_enables >> i) & 0b1;
            if(wen)
                this->data[index][i] = data_write[i];
        }
    }
};





void Connect(CHI_RNF::LinkLayer *rn_layer, CHI_HNF::LinkLayer *hn_layer)
{
    uint32_t req_num = (uint32_t)ceil(Req_Total_Width/32.0);
    uint32_t snp_num = (uint32_t)ceil(Snp_Total_Width/32.0);
    uint32_t rsp_num = (uint32_t)ceil(Rsp_Total_Width/32.0);
    uint32_t dat_num = (uint32_t)ceil(Dat_Total_Width/32.0);

    hn_layer->icn_rn_ports->RXREQFLITV = rn_layer->rn_icn_ports->TXREQFLITV;
    for(uint32_t i = 0; i < req_num; i++)
        hn_layer->icn_rn_ports->RXREQFLIT[i] = rn_layer->rn_icn_ports->TXREQFLIT[i];

    hn_layer->icn_rn_ports->RXRSPFLITV = rn_layer->rn_icn_ports->TXRSPFLITV;
    for(uint32_t i = 0; i < rsp_num; i++)
        hn_layer->icn_rn_ports->RXRSPFLIT[i] = rn_layer->rn_icn_ports->TXRSPFLIT[i]; 

    hn_layer->icn_rn_ports->RXDATFLITV = rn_layer->rn_icn_ports->TXDATFLITV;
    for(uint32_t i = 0; i < dat_num; i++)
        hn_layer->icn_rn_ports->RXDATFLIT[i] = rn_layer->rn_icn_ports->TXDATFLIT[i];   



    rn_layer->rn_icn_ports->RXSNPFLITV = hn_layer->icn_rn_ports->TXSNPFLITV;
    for(uint32_t i = 0; i < snp_num; i++)
        rn_layer->rn_icn_ports->RXSNPFLIT[i] = hn_layer->icn_rn_ports->TXSNPFLIT[i];
    
    rn_layer->rn_icn_ports->RXRSPFLITV = hn_layer->icn_rn_ports->TXRSPFLITV;
    for(uint32_t i = 0; i < rsp_num; i++)
        rn_layer->rn_icn_ports->RXRSPFLIT[i] = hn_layer->icn_rn_ports->TXRSPFLIT[i];
    
    rn_layer->rn_icn_ports->RXDATFLITV = hn_layer->icn_rn_ports->TXDATFLITV;
    for(uint32_t i = 0; i < dat_num; i++)
        rn_layer->rn_icn_ports->RXDATFLIT[i] = hn_layer->icn_rn_ports->TXDATFLIT[i];

}





void GenerateAccREQ(uint32_t num_gen, queue<CHI_RNF::ACCFLIT*>& acc_flit_queue, GoldenMem *golden_mem)
{
    CHI_RNF::ACCFLIT *acc_flit = NULL;
    uint32_t accID = 0;


    for(uint32_t i = 0; i < num_gen; i++)
    {
        acc_flit = new CHI_RNF::ACCFLIT();
        // 1. set acc_id
        acc_flit->acc_id = accID++;
        // 2. set type 
        uint32_t rand_type = rand() % 8;
        switch(rand_type)
        {
            case 0:
                acc_flit->type = CHI_RNF::AccTypeEncoding::AccRead;
                break;
            case 1:
            case 2:
            case 3:
                acc_flit->type = CHI_RNF::AccTypeEncoding::AccWrite_Allocate;
                break;
            case 4:
            case 5:
            case 6:
                acc_flit->type = CHI_RNF::AccTypeEncoding::AccWrite_NoAllocate;
                break;
            default:
                acc_flit->type = CHI_RNF::AccTypeEncoding::AccFlush;
                break;
        }
        // 3. set addr
        if((acc_flit->type == CHI_RNF::AccTypeEncoding::AccWrite_Allocate) |
           (acc_flit->type == CHI_RNF::AccTypeEncoding::AccWrite_NoAllocate))
        {
            uint64_t rand_addr = 0;
            while(rand_addr <= 0x00ff)
            {
                rand_addr = rand() & 0x0fff;
            }
            acc_flit->addr = rand_addr;
        }
        else
        {
            acc_flit->addr = rand() & 0x7fff;
        }

        // 4. set data[64]
        for(uint32_t index = 0; index < 64; index++)
        {
            acc_flit->data[index] = rand() & 0xff;
        }
        // 5. set write_enables
        uint32_t rand_wen = rand() % 10;
        switch(rand_wen)
        {
            case 0:
                acc_flit->write_enable = 0x000000000000ffff;
                break;
            case 1:
                acc_flit->write_enable = 0x00000000ffff0000;
                break;
            case 2:
                acc_flit->write_enable = 0x0000ffff00000000;
                break;
            case 3:
                acc_flit->write_enable = 0xffff000000000000;
                break;
            case 5:
                acc_flit->write_enable = -1;
                break;
            default:
                acc_flit->write_enable = (uint64_t)rand();
                break;
        }
        if((acc_flit->type == CHI_RNF::AccTypeEncoding::AccRead) |
           (acc_flit->type == CHI_RNF::AccTypeEncoding::AccFlush))
        {
            acc_flit->write_enable = 0;
        }
        acc_flit_queue.push(acc_flit);


        if((acc_flit->type == CHI_RNF::AccTypeEncoding::AccWrite_Allocate) |
           (acc_flit->type == CHI_RNF::AccTypeEncoding::AccWrite_NoAllocate))
        {
            uint64_t index = (acc_flit->addr >> 6);
            golden_mem->Write(index, acc_flit->data, acc_flit->write_enable);
        }
    }
}





void GenerateAccSNP(uint32_t num_gen, queue<CHI_HNF::ACCFLIT*>& acc_flit_queue, GoldenMem *golden_mem)
{
    CHI_HNF::ACCFLIT *acc_flit = NULL;
    uint32_t accID = 0;


    for(uint32_t i = 0; i < num_gen; i++)
    {
        acc_flit = new CHI_HNF::ACCFLIT();
        // 1. set acc_id
        acc_flit->acc_id = accID++;
        // 2. set type 
        uint32_t rand_type = rand() % 10;
        switch(rand_type)
        {
            case 0:
            case 1:
            case 2:
            case 3:
            case 4:
                acc_flit->type = CHI_HNF::AccTypeEncoding::Snoop_Only;
                break;
            case 5:
            case 6:
                acc_flit->type = CHI_HNF::AccTypeEncoding::Snoop_Read;
                break;
            default:
                acc_flit->type = CHI_HNF::AccTypeEncoding::Snoop_Write;
                break;
        }
        // 3. set opcode
        uint32_t rand_opcode = rand() % 6;
        switch(rand_opcode)
        {
            case 0:
                acc_flit->snp_opcode = CHI_HNF::SnpOpcodeEncoding::SnpClean;
                break;
            case 1:
                acc_flit->snp_opcode = CHI_HNF::SnpOpcodeEncoding::SnpShared;
                break;
            case 2:
                acc_flit->snp_opcode = CHI_HNF::SnpOpcodeEncoding::SnpUnique;
                break;

            case 3:
                acc_flit->snp_opcode = CHI_HNF::SnpOpcodeEncoding::SnpCleanInvalid;
                break;
            case 4:
                acc_flit->snp_opcode = CHI_HNF::SnpOpcodeEncoding::SnpCleanShared;
                break;
            default:
                acc_flit->snp_opcode = CHI_HNF::SnpOpcodeEncoding::SnpQuery;
                break;
        }
        
        // 4. set addr
        if(acc_flit->type == CHI_HNF::AccTypeEncoding::Snoop_Write)
        {
            acc_flit->addr = rand() & 0x00ff;
        }
        else
        {
            acc_flit->addr = rand() & 0x0fff;
        }

        // 5. set data[64]
        for(uint32_t index = 0; index < 64; index++)
        {
            acc_flit->data[index] = rand() & 0xff;
        }
        // 6. set write_enables
        uint32_t rand_wen = rand() % 10;
        switch(rand_wen)
        {
            case 0:
                acc_flit->write_enable = 0x000000000000ffff;
                break;
            case 1:
                acc_flit->write_enable = 0x00000000ffff0000;
                break;
            case 2:
                acc_flit->write_enable = 0x0000ffff00000000;
                break;
            case 3:
                acc_flit->write_enable = 0xffff000000000000;
                break;
            case 5:
                acc_flit->write_enable = -1;
                break;
            default:
                acc_flit->write_enable = (uint64_t)rand();
                break;
        }
        if(acc_flit->type != CHI_HNF::AccTypeEncoding::Snoop_Write)
        {
            acc_flit->write_enable = 0;
        }
        acc_flit_queue.push(acc_flit);


        if(acc_flit->type == CHI_HNF::AccTypeEncoding::Snoop_Write)
        {
            uint64_t index = (acc_flit->addr >> 6);
            golden_mem->Write(index, acc_flit->data, acc_flit->write_enable);
        }
    }

}






void GenerateAccFlush(queue<CHI_RNF::ACCFLIT*>& acc_flit_queue)
{
    CHI_RNF::ACCFLIT *acc_flit = NULL;
    uint64_t addr = 0;

    for(uint32_t i = 0; i < Num_Lines; i++)
    {
        acc_flit = new CHI_RNF::ACCFLIT();
        acc_flit->acc_id = i;
        acc_flit->type = CHI_RNF::AccTypeEncoding::AccFlush;
        acc_flit->addr = addr;
        acc_flit_queue.push(acc_flit);

        addr = addr + 64;
    }
}






bool CompareMem(GoldenMem *golden_mem, uint8_t mem_data[Num_Lines][64])
{
    for(uint32_t i = 0; i < Num_Lines; i++)
    {
        for(uint32_t j = 0; j < 64; j++)
        {
            if(golden_mem->data[i][j] != mem_data[i][j])
            {
                printf("XiaBin: Compare -> Error!!!\n");
                printf("GoldenMem: data[%x][%d] = 0x%x\n", i, j, golden_mem->data[i][j]);
                printf("Mem  Data: data[%x][%d] = 0x%x\n", i, j, mem_data[i][j]);
                return true;
            }
        }
    }
    return false;
}







#endif