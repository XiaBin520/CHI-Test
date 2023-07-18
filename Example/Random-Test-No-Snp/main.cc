#include "main_test.h"


int main()
{
    srand(10001);

    CHI_RNF::LinkLayer *rn_layer = new CHI_RNF::LinkLayer();
    CHI_HNF::LinkLayer *hn_layer = new CHI_HNF::LinkLayer();


    GoldenMem *golden_mem = new GoldenMem();
    uint8_t mem_data[512][64];


    const uint64_t CLK_COUNT = 100 * 10000;




    uint64_t all_gen_num = 0; // the number of all generate acc request
    uint64_t all_del_num = 0; // the number of all finish acc request
    uint64_t has_acc_num = 0; // the number of handling

    for(uint32_t loop = 0; loop < 10; loop++)
    {
        all_gen_num = 0;
        all_del_num = 0;
        has_acc_num = 0;

        // AccREQ
        for(uint64_t i = 0; i < CLK_COUNT; i++)
        {
            if((i >= (10*10000)) & (i % (10*10000) == 0))
            {
                printf("XiaBin: Test Loop = %d, Clock = %ld\n", loop, i);
            }

            has_acc_num = all_gen_num - all_del_num;
            if(has_acc_num < 100)
            {
                uint32_t rand_gen_num = rand() % 10;
                GenerateAccREQ(rand_gen_num, rn_layer->accreq_flit_queue, golden_mem);
                all_gen_num = all_gen_num + rand_gen_num;
            }

            Connect(rn_layer, hn_layer);
            rn_layer->RUN(); 
            hn_layer->RUN();


            while(!rn_layer->accrsp_flit_queue.empty())
            {
                CHI_RNF::ACCFLIT *accrsp_flit = rn_layer->accrsp_flit_queue.front();
                rn_layer->accrsp_flit_queue.pop();
                delete accrsp_flit;
                all_del_num++;
            }
        }


        // AccFlush
        for(uint64_t i = 0; i < 100000; i++)
        {
            if((i == 2))
            {
                GenerateAccFlush(rn_layer->accreq_flit_queue);
            }

            Connect(rn_layer, hn_layer);
            rn_layer->RUN(); 
            hn_layer->RUN();

            while(!rn_layer->accrsp_flit_queue.empty())
            {
                CHI_RNF::ACCFLIT *accrsp_flit = rn_layer->accrsp_flit_queue.front();
                rn_layer->accrsp_flit_queue.pop();
                delete accrsp_flit;
            }
        }

        // Compare
        hn_layer->PrintMemory(mem_data);
        bool different_data = CompareMem(golden_mem, mem_data);
        if(different_data) assert(0);

        printf("*************************************************\n");
        printf("XiaBin: Random-Test -> Loop = %02d, Success!\n", loop);
        printf("        Test REQ Number is %ld\n", all_gen_num);
        printf("*************************************************\n");
        printf("\n\n");
    }


    return 0;
}