#include "main_test.h"


int main()
{
    srand(1000002);

    CHI_RNF::LinkLayer *rn_layer = new CHI_RNF::LinkLayer();
    CHI_HNF::LinkLayer *hn_layer = new CHI_HNF::LinkLayer();

    GoldenMem *golden_mem = new GoldenMem();
    uint8_t mem_data[512][64];


    const uint64_t CLK_COUNT =  100 * 10000;



    uint64_t all_gen_req_num, all_gen_snp_num; // the number of all generate request
    uint64_t all_del_req_num, all_del_snp_num; // the number of all finish request
    uint64_t has_req_num, has_snp_num; // the number of handling request




    for(uint32_t loop = 0; loop < 10; loop++)
    {
        all_gen_req_num = 0;
        all_gen_snp_num = 0;
        all_del_req_num = 0;
        all_del_snp_num = 0;
        has_req_num = 0;
        has_snp_num = 0;

        // Create and Run
        for(uint64_t i = 0; i < CLK_COUNT; i++)
        {
            if((i >= (10*10000)) & (i % (10*10000) == 0))
            {
                printf("XiaBin: Test Loop = %d, Clock = %ld\n", loop, i);
            }

            has_req_num = all_gen_req_num - all_del_req_num;
            if(has_req_num < 100)
            {
                uint32_t gen_req_num = rand() % 10;
                GenerateAccREQ(gen_req_num, rn_layer->accreq_flit_queue, golden_mem);
                all_gen_req_num = all_gen_req_num + gen_req_num;
            }


            has_snp_num = all_gen_snp_num - all_del_snp_num;
            if(has_snp_num < 100)
            {
                uint32_t gen_snp_num = rand() % 10;
                GenerateAccSNP(gen_snp_num, hn_layer->accreq_flit_queue, golden_mem);
                all_gen_snp_num = all_gen_snp_num + gen_snp_num;
            }



            Connect(rn_layer, hn_layer);
            rn_layer->RUN();
            hn_layer->RUN();


            while(!rn_layer->accrsp_flit_queue.empty())
            {
                CHI_RNF::ACCFLIT *accreq_rsp_flit = rn_layer->accrsp_flit_queue.front();
                rn_layer->accrsp_flit_queue.pop();
                delete accreq_rsp_flit;
                all_del_req_num++;
            }
            while(!hn_layer->accrsp_flit_queue.empty())
            {
                CHI_HNF::ACCFLIT *accsnp_rsp_flit = hn_layer->accrsp_flit_queue.front();
                hn_layer->accrsp_flit_queue.pop();
                delete accsnp_rsp_flit;
                all_del_snp_num++;
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
            while(!hn_layer->accrsp_flit_queue.empty())
            {
                CHI_HNF::ACCFLIT *accsnp_rsp_flit = hn_layer->accrsp_flit_queue.front();
                hn_layer->accrsp_flit_queue.pop();
                delete accsnp_rsp_flit;
            }
        }


        // Compare
        hn_layer->PrintMemory(mem_data);
        bool different_data = CompareMem(golden_mem, mem_data);
        if(different_data) assert(0);

        printf("********************************************************\n");
        printf("XiaBin: Random-Snp -> Loop = %02d, Success!\n", loop);
        printf("        Test REQ Number = %ld, SNP Number = %ld\n", all_gen_req_num, all_gen_snp_num);
        printf("********************************************************\n");
        printf("\n\n");
    }


    return 0;
}