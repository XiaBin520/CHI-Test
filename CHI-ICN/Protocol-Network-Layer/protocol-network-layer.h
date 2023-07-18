
#ifndef CHI_HNF_PROTOCOL_NETWORK_LAYER_H_
#define CHI_HNF_PROTOCOL_NETWORK_LAYER_H_

#include "ACCHandle/acchandle_list.h"
#include "ReqHandle/reqhandle_list.h"
#include "SnpHandle/SnoopHandle.h"
#include "memory.h"


namespace CHI_HNF
{

class ProtocolNetworkLayer
{
private:
    Memory *memory;
    bool transid_busy_arr[1024 * 4]; // CHI TxnID[11:0]


private:
    uint32_t AllocTransID();
    void FreeTransID(uint32_t trans_id);
public:
    ProtocolNetworkLayer();
    void PrintMemory(uint8_t mem_data[512][64])
    {
        memory->PrintMemory(mem_data);
    }



private:
    vector<ACCFLIT*> wait_accreq_flit_vec;
    vector<IAccHandle*> acc_handle_vec;
public:
    void MonitorAccREQ(queue<ACCFLIT*>& accreq_flit_queue);
    void DriverAccRSP(queue<ACCFLIT*>& accrsp_flit_queue);
    void ACC_CheckDone();



private:
    vector<IReqHandle*> rxreq_handle_vec;
    vector<ISnpHandle*> txsnp_handle_vec;
public:
    void DriverTxSNP(queue<SNPFLIT*>& txsnp_flit_queue, uint32_t sel_index);
    void DriverTxRSP(queue<RSPFLIT*>& txrsp_flit_queue, uint32_t sel_index);
    void DriverTxDAT(queue<DATFLIT*>& txdat_flit_queue, uint32_t sel_index);

    void MonitorRxREQ(REQFLIT *rxreq_flit);
    void MonitorRxRSP(RSPFLIT *rxrsp_flit);
    void MonitorRxDAT(DATFLIT *rxdat_flit);
    void MonitorTxRSP(RSPFLIT *txrsp_flit);
    void MonitorTxDAT(DATFLIT *txdat_flit);

    void CHIReq_CheckDone();
    void CHISnp_CheckDone();
};





uint32_t ProtocolNetworkLayer::AllocTransID()
{
    for(uint32_t i = 0; i < 10; i++)
    {
        uint32_t rand_index = rand() & 0xfff; // the width of TxnID is 12
        if(!transid_busy_arr[rand_index])
        {
            transid_busy_arr[rand_index] = true;
            return rand_index;
        }
    }

    for(uint32_t i = 0; i < 4*1024; i++)
    {
        if(!transid_busy_arr[i])
        {
            transid_busy_arr[i] = true;
            return i;
        }
    }
    assert(0);
}


void ProtocolNetworkLayer::FreeTransID(uint32_t trans_id)
{
    if(trans_id >= (1024*4)) assert(0);
    if(!transid_busy_arr[trans_id]) assert(0);

    transid_busy_arr[trans_id] = false;
}




ProtocolNetworkLayer::ProtocolNetworkLayer()
{
    this->memory = new Memory();
    memset(transid_busy_arr, 0, sizeof(transid_busy_arr));
}








/***************************************************/
/* ACC Handle Functions                            */
/***************************************************/
void ProtocolNetworkLayer::MonitorAccREQ(queue<ACCFLIT*>& accreq_flit_queue)
{
    while(!accreq_flit_queue.empty())
    {
        ACCFLIT *accreq_flit = accreq_flit_queue.front();
        accreq_flit_queue.pop();
        wait_accreq_flit_vec.push_back(accreq_flit);
    }

    for(uint32_t i = 0; i < wait_accreq_flit_vec.size();)
    {
        ACCFLIT *wait_accreq = wait_accreq_flit_vec[i];
        uint64_t addr = wait_accreq->addr;
        MemLine *mem_line = memory->FindMemLine(addr);
        if(mem_line == NULL) assert(0);

        if(mem_line->acc_is_pending)
        {
            i++;
            continue;
        }

        IAccHandle *acc_handle = NULL;
        switch(wait_accreq->type)
        {
            case AccTypeEncoding::Snoop_Only:
                acc_handle = new SnoopOnlyHandle(wait_accreq, mem_line);
                break;
            case AccTypeEncoding::Snoop_Read:
                acc_handle = new SnoopReadHandle(wait_accreq, mem_line);
                break;
            case AccTypeEncoding::Snoop_Write:
                acc_handle = new SnoopWriteHandle(wait_accreq, mem_line);
                break;
            default:
                assert(0);
                break;
        }
        acc_handle_vec.push_back(acc_handle);
        wait_accreq_flit_vec.erase(wait_accreq_flit_vec.begin() + i);
    }
}


void ProtocolNetworkLayer::DriverAccRSP(queue<ACCFLIT*>& accrsp_flit_queue)
{
    uint32_t num_acc_handle = acc_handle_vec.size();
    for(uint32_t i = 0; i < num_acc_handle; i++)
    {
        IAccHandle *acc_handle = acc_handle_vec[i];
        acc_handle->DriverAccRSP(accrsp_flit_queue);
    }
}


void ProtocolNetworkLayer::ACC_CheckDone()
{
    for(uint32_t i = 0; i < acc_handle_vec.size();)
    {
        IAccHandle *acc_handle = acc_handle_vec[i];
        if(!acc_handle->IsDone())
        {
            i++;
            continue;
        }

        acc_handle_vec.erase(acc_handle_vec.begin() + i);
        delete acc_handle;
    }
}












//********************************************************
//  Driver CHI TxSNP TxRSP TxDAT Channel
//********************************************************
void ProtocolNetworkLayer::DriverTxSNP(queue<SNPFLIT*>& txsnp_flit_queue, uint32_t sel_index)
{
    uint32_t num_acc_handle = acc_handle_vec.size();
    if(sel_index >= num_acc_handle) return;

    IAccHandle *acc_handle = acc_handle_vec[sel_index];
    acc_handle->DriverTxSNP(txsnp_flit_queue);

    // if sent txsnp, create txsnp_handle
    if(!txsnp_flit_queue.empty())
    {
        SNPFLIT *txsnp_flit = txsnp_flit_queue.front();
        txsnp_flit->TxnID = AllocTransID();

        MemLine *mem_line = memory->FindMemLine(txsnp_flit->Addr);
        if(mem_line == NULL) assert(0);
        SnoopHandle *txsnp_handle = new SnoopHandle(txsnp_flit, mem_line);
        txsnp_handle_vec.push_back(txsnp_handle);
    }
}


void ProtocolNetworkLayer::DriverTxRSP(queue<RSPFLIT*>& txrsp_flit_queue, uint32_t sel_index)
{
    uint32_t rxreq_size = rxreq_handle_vec.size();
    if(sel_index >= rxreq_size) return;

    IReqHandle *temp_rxreq = rxreq_handle_vec[sel_index];
    temp_rxreq->DriverTxRSP(txrsp_flit_queue);
}


void ProtocolNetworkLayer::DriverTxDAT(queue<DATFLIT*>& txdat_flit_queue, uint32_t sel_index)
{
    uint32_t rxreq_size = rxreq_handle_vec.size();
    if(sel_index >= rxreq_size) return;

    IReqHandle *temp_rxreq = rxreq_handle_vec[sel_index];
    temp_rxreq->DriverTxDAT(txdat_flit_queue);
}






//********************************************************
// Monitor CHI RxREQ RxRSP RxDAT TxSNP TxRSP TxDAT
//********************************************************
#define CreateReqHandle(REQOPCODE) \
            case ReqOpcodeEncoding::REQOPCODE: \
                rxreq_handle = new REQOPCODE##Handle(dbid, rxreq_flit, mem_line); \
                break;
 
void ProtocolNetworkLayer::MonitorRxREQ(REQFLIT *rxreq_flit)
{
    if(rxreq_flit == NULL) return;

    uint32_t dbid = AllocTransID();
    MemLine *mem_line = memory->FindMemLine(rxreq_flit->Addr);
    if(mem_line == NULL) assert(0);
    if(mem_line->req_is_pending) assert(0);

    IReqHandle *rxreq_handle = NULL;
    switch(rxreq_flit->Opcode)
    {
        // Read Request
        CreateReqHandle(ReadClean)
        CreateReqHandle(ReadShared)
        CreateReqHandle(ReadUnique)
        // Dataless Request
        CreateReqHandle(CleanUnique)
        CreateReqHandle(Evict)
        CreateReqHandle(MakeUnique)
        // CopyBack Write Data Request
        CreateReqHandle(WriteBackPtl)
        CreateReqHandle(WriteBackFull)
        CreateReqHandle(WriteCleanFull)
        CreateReqHandle(WriteEvictFull)
        // None CopyBack Write Data Request
        CreateReqHandle(WriteUniquePtl)
        CreateReqHandle(WriteUniqueFull)
        default:
            assert(0);
            break;
    }

    if(rxreq_handle != NULL)
        rxreq_handle_vec.push_back(rxreq_handle);
}


void ProtocolNetworkLayer::MonitorRxRSP(RSPFLIT *rxrsp_flit)
{
    if(rxrsp_flit == NULL) return;

    uint32_t rxreq_size = rxreq_handle_vec.size();
    for(uint32_t i = 0; i < rxreq_size; i++)
    {
        IReqHandle *temp_rxreq = rxreq_handle_vec[i];
        if(temp_rxreq->MonitorRxRSP(rxrsp_flit)) {return;}
    }

    uint32_t txsnp_size = txsnp_handle_vec.size();
    for(uint32_t i = 0; i < txsnp_size; i++)
    {
        ISnpHandle *temp_txsnp = txsnp_handle_vec[i];
        if(temp_txsnp->MonitorRxRSP(rxrsp_flit)) {return;}
    }

    assert(0);
}


void ProtocolNetworkLayer::MonitorRxDAT(DATFLIT *rxdat_flit)
{
    if(rxdat_flit == NULL) return;

    uint32_t rxreq_size = rxreq_handle_vec.size();
    for(uint32_t i = 0; i < rxreq_size; i++)
    {
        IReqHandle *temp_rxreq = rxreq_handle_vec[i];
        if(temp_rxreq->MonitorRxDAT(rxdat_flit)) {return;}
    }

    uint32_t txsnp_size = txsnp_handle_vec.size();
    for(uint32_t i = 0; i < txsnp_size; i++)
    {
        ISnpHandle *temp_txsnp = txsnp_handle_vec[i];
        if(temp_txsnp->MonitorRxDAT(rxdat_flit)) {return;}
    }

    assert(0);
}


void ProtocolNetworkLayer::MonitorTxRSP(RSPFLIT *txrsp_flit)
{
    if(txrsp_flit == NULL) return;
    uint32_t rxreq_size = rxreq_handle_vec.size();
    for(uint32_t i = 0; i < rxreq_size; i++)
    {
        IReqHandle *temp_rxreq = rxreq_handle_vec[i];
        if(temp_rxreq->MonitorTxRSP(txrsp_flit)) {return;}
    }

    assert(0);
}


void ProtocolNetworkLayer::MonitorTxDAT(DATFLIT *txdat_flit)
{
    if(txdat_flit == NULL) return;

    uint32_t rxreq_size = rxreq_handle_vec.size();
    for(uint32_t i = 0; i < rxreq_size; i++)
    {
        IReqHandle *temp_rxreq = rxreq_handle_vec[i];
        if(temp_rxreq->MonitorTxDAT(txdat_flit)) {return;}
    }

    assert(0);
}








//********************************************
//  Check Transaction Done
//********************************************
void ProtocolNetworkLayer::CHIReq_CheckDone()
{
    for(uint32_t i = 0; i < rxreq_handle_vec.size();)
    {
        IReqHandle *rxreq_handle = rxreq_handle_vec[i];
        if(!rxreq_handle->IsDone())
        {
            i++;
            continue;
        }
        
        rxreq_handle_vec.erase(rxreq_handle_vec.begin() + i);
        uint32_t dbid = rxreq_handle->GetDBID();
        FreeTransID(dbid);
        delete rxreq_handle;
    }
}


void ProtocolNetworkLayer::CHISnp_CheckDone()
{
    for(uint32_t i = 0; i < txsnp_handle_vec.size();)
    {
        ISnpHandle *txsnp_handle = txsnp_handle_vec[i];
        if(!txsnp_handle->IsDone())
        {
            i++;
            continue;
        }

        txsnp_handle_vec.erase(txsnp_handle_vec.begin() + i);
        uint32_t txnid = txsnp_handle->GetTxnID();
        FreeTransID(txnid);
        delete txsnp_handle;
    }
}



}
#endif