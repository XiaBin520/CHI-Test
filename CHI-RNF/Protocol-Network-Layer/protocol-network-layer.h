#ifndef CHI_RNF_PROTOCOL_NETWORK_LAYER_H_
#define CHI_RNF_PROTOCOL_NETWORK_LAYER_H_


#include "../cache.h"
#include "AccHandle/acchandle_list.h"
#include "ReqHandle/reqhandle_list.h"
#include "SnpHandle/snphandle_list.h"


namespace CHI_RNF
{


class ProtocolNetworkLayer
{
private:
    bool txnid_busy_arr[1024 * 4]; // CHI TxnID[11:0]
    Cache *cache;

private:
    uint32_t AllocTxnID();
    void FreeTxnID(uint32_t txnid);
public:
    ProtocolNetworkLayer();
    void CleanInvalidCacheLine();





private:
    vector<ACCFLIT*> accreq_flit_vec;
    vector<IAccHandle*> acc_handle_vec;
public:
    void MonitorACCREQ(queue<ACCFLIT*>& accreq_flit_queue);
    void DriverACCRSP(queue<ACCFLIT*>& accrsp_flit_queue);
    void ACC_CheckDone();



private:
    vector<SNPFLIT*> wait_rxsnp_flit_vec;
    vector<IReqHandle*> txreq_handle_vec;
    vector<ISnpHandle*> rxsnp_handle_vec;

public:
    // CHI Driver Tx Channel
    void DriverTxREQ(queue<REQFLIT*>& txreq_flit_queue, uint32_t sel_index);
    void DriverTxRSP(queue<RSPFLIT*>& txrsp_flit_queue, bool req_or_snp, uint32_t sel_index);
    void DriverTxDAT(queue<DATFLIT*>& txdat_flit_queue, bool req_or_snp, uint32_t sel_index);
    // CHI Monitor Tx and Rx Channel
    void MonitorTxRSP(RSPFLIT *txrsp_flit);
    void MonitorTxDAT(DATFLIT *txdat_flit);
    void MonitorRxSNP(SNPFLIT *rxsnp_flit);
    void MonitorRxRSP(RSPFLIT *rxrsp_flit);
    void MonitorRxDAT(DATFLIT *rxdat_flit);
    // CHI CheckDone
    void CHI_CheckDone();
};





ProtocolNetworkLayer::ProtocolNetworkLayer()
{
    memset(txnid_busy_arr, 0, sizeof(txnid_busy_arr));
    cache = new Cache();
}


uint32_t ProtocolNetworkLayer::AllocTxnID()
{
    for(uint32_t i = 0; i < 10; i++)
    {
        uint32_t rand_index = rand() & 0xfff; // the width of TxnID is 12
        if(!txnid_busy_arr[rand_index])
        {
            txnid_busy_arr[rand_index] = true;
            return rand_index;
        }
    }

    for(uint32_t i = 0; i < 4*1024; i++)
    {
        if(!txnid_busy_arr[i])
        {
            txnid_busy_arr[i] = true;
            return i;
        }
    }
    assert(0);
}


void ProtocolNetworkLayer::FreeTxnID(uint32_t txnid)
{
    if(txnid >= (1024*4)) assert(0);
    if(!txnid_busy_arr[txnid]) assert(0);

    txnid_busy_arr[txnid] = false;
}


void ProtocolNetworkLayer::CleanInvalidCacheLine()
{
    cache->CleanInvalidCacheLine();
}








/******************************************************/
/*  ACC Handle Functions                              */
/******************************************************/
void ProtocolNetworkLayer::MonitorACCREQ(queue<ACCFLIT*>& accreq_flit_queue)
{
    while(!accreq_flit_queue.empty())
    {
        ACCFLIT *accreq_flit = accreq_flit_queue.front();
        accreq_flit_queue.pop();
        accreq_flit_vec.push_back(accreq_flit);
    }

    for(uint32_t i = 0; i < accreq_flit_vec.size();)
    {
        ACCFLIT *temp_accreq = accreq_flit_vec[i];
        uint64_t temp_addr = temp_accreq->addr;
        CacheLine *cache_line = cache->FindCacheLine(temp_addr);
        if(cache_line == NULL) cache_line = cache->NewCacheLine(temp_addr);

        if(cache_line->acc_is_pending)
        {
            i++; // next accreq_flit
            continue;
        }
        // Set acc_handle
        IAccHandle *acc_handle = NULL;
        switch(temp_accreq->type)
        {
            case AccTypeEncoding::AccRead:
                acc_handle = new AccReadHandle(cache_line, temp_accreq);
                break;
            case AccTypeEncoding::AccWrite_Allocate:
                acc_handle = new AccWrAllocHandle(cache_line, temp_accreq);
                break;
            case AccTypeEncoding::AccWrite_NoAllocate:
                acc_handle = new AccWrNoAllocHandle(cache_line, temp_accreq);
                break;
            case AccTypeEncoding::AccFlush:
                acc_handle = new AccFlushHandle(cache_line, temp_accreq);
                break;
            default:
                assert(0);
                break;
        }
        acc_handle_vec.push_back(acc_handle);
        // delete from accreq_flit_vec
        accreq_flit_vec.erase(accreq_flit_vec.begin() + i);
    }
}


void ProtocolNetworkLayer::DriverACCRSP(queue<ACCFLIT*>& accrsp_flit_queue)
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










/******************************************************/
/*  CHI Driver Functions                              */
/******************************************************/
void ProtocolNetworkLayer::DriverTxREQ(queue<REQFLIT*>& txreq_flit_queue, uint32_t sel_index)
{
    uint32_t num_rxacc = acc_handle_vec.size();
    if(sel_index >= num_rxacc) return;

    queue<ACCFLIT*> got_accreq_queue;
    IAccHandle *acc_handle = acc_handle_vec[sel_index];
    acc_handle->DriverTxREQ(txreq_flit_queue, got_accreq_queue);


    if(!txreq_flit_queue.empty())
    {
        REQFLIT *txreq_flit = txreq_flit_queue.front();
        uint32_t txnid = AllocTxnID();
        txreq_flit->TxnID = txnid;

        ACCFLIT *accreq_flit = got_accreq_queue.front();

        CacheLine *cache_line = cache->FindCacheLine(txreq_flit->Addr);
        if(cache_line == NULL) assert(0);

        IReqHandle *txreq_handle = NULL;
        switch(txreq_flit->Opcode)
        {
            // Read transaction
            case ReqOpcodeEncoding::ReadClean:
                txreq_handle = new ReadCleanHandle(accreq_flit, txreq_flit, cache_line);
                break;
            case ReqOpcodeEncoding::ReadShared:
                txreq_handle = new ReadSharedHandle(accreq_flit, txreq_flit, cache_line);
                break;
            case ReqOpcodeEncoding::ReadUnique:
                txreq_handle = new ReadUniqueHandle(accreq_flit, txreq_flit, cache_line);
                break;
           
            // Dataless transaction
            case ReqOpcodeEncoding::CleanUnique:
                txreq_handle = new CleanUniqueHandle(accreq_flit, txreq_flit, cache_line);
                break;
            case ReqOpcodeEncoding::Evict:
                txreq_handle = new EvictHandle(accreq_flit, txreq_flit, cache_line);
                break;
            case ReqOpcodeEncoding::MakeUnique:
                txreq_handle = new MakeUniqueHandle(accreq_flit, txreq_flit, cache_line);
                break;
            
            // CopyBack Write Data transaction
            case ReqOpcodeEncoding::WriteBackFull:
                txreq_handle = new WriteBackFullHandle(accreq_flit, txreq_flit, cache_line);
                break;
            case ReqOpcodeEncoding::WriteCleanFull:
                txreq_handle = new WriteCleanFullHandle(accreq_flit, txreq_flit, cache_line);
                break;
            case ReqOpcodeEncoding::WriteEvictFull:
                txreq_handle = new WriteEvictFullHandle(accreq_flit, txreq_flit, cache_line);
                break;
            
            // Non-CopyBack Write transaction
            case ReqOpcodeEncoding::WriteUniquePtl:
                txreq_handle = new WriteUniquePtlHandle(accreq_flit, txreq_flit, cache_line);
                break;
            case ReqOpcodeEncoding::WriteUniqueFull:
                txreq_handle = new WriteUniqueFullHandle(accreq_flit, txreq_flit, cache_line);
                break;
            // assert(0)
            default:
                assert(0);
                break;
        }
        txreq_handle_vec.push_back(txreq_handle);
    }
}


void ProtocolNetworkLayer::DriverTxRSP(queue<RSPFLIT*>& txrsp_flit_queue, bool req_or_snp, uint32_t sel_index)
{
    bool sel_req = req_or_snp;
    
    if(sel_req) // DriverTxRSP for txreq
    {
        uint32_t num_txreq = txreq_handle_vec.size();
        if(sel_index >= num_txreq) return;

        IReqHandle *txreq_handle = txreq_handle_vec[sel_index];
        txreq_handle->DriverTxRSP(txrsp_flit_queue);
    }
    else // DriverTxRSP for rxsnp
    {
        uint32_t num_rxsnp = rxsnp_handle_vec.size();
        if(sel_index >= num_rxsnp) return;

        ISnpHandle *rxsnp_handle = rxsnp_handle_vec[sel_index];
        rxsnp_handle->DriverTxRSP(txrsp_flit_queue);
    }
}


void ProtocolNetworkLayer::DriverTxDAT(queue<DATFLIT*>& txdat_flit_queue, bool req_or_snp, uint32_t sel_index)
{
    bool sel_req = req_or_snp;
    if(sel_req) // DriverTxDAT for txreq
    {
        uint32_t num_txreq = txreq_handle_vec.size();
        if(sel_index >= num_txreq) return;

        IReqHandle *txreq_handle = txreq_handle_vec[sel_index];
        txreq_handle->DriverTxDAT(txdat_flit_queue);
    }
    else // DriverTxDAT for rxsnp
    {
        uint32_t num_rxsnp = rxsnp_handle_vec.size();
        if(sel_index >= num_rxsnp) return;

        ISnpHandle *rxsnp_handle = rxsnp_handle_vec[sel_index];
        rxsnp_handle->DriverTxDAT(txdat_flit_queue);
    }
}




/******************************************************/
/*  CHI Monitor Functions                             */
/******************************************************/
void ProtocolNetworkLayer::MonitorTxRSP(RSPFLIT *txrsp_flit)
{
    if(txrsp_flit == NULL) return;

    uint32_t num_txreq = txreq_handle_vec.size();
    for(uint32_t i = 0; i < num_txreq; i++)
    {
        IReqHandle *txreq_handle = txreq_handle_vec[i];
        bool txreq_monitor_en = txreq_handle->MonitorTxRSP(txrsp_flit);
        if(txreq_monitor_en) return;
    }

    uint32_t num_rxsnp = rxsnp_handle_vec.size();
    for(uint32_t i = 0; i < num_rxsnp; i++)
    {
        ISnpHandle *rxsnp_handle = rxsnp_handle_vec[i];
        bool rxsnp_monitor_en = rxsnp_handle->MonitorTxRSP(txrsp_flit);
        if(rxsnp_monitor_en) return;
    }

    assert(0);
}


void ProtocolNetworkLayer::MonitorTxDAT(DATFLIT *txdat_flit)
{
    if(txdat_flit == NULL) return;

    uint32_t num_txreq = txreq_handle_vec.size();
    for(uint32_t i = 0; i < num_txreq; i++)
    {
        IReqHandle *txreq_handle = txreq_handle_vec[i];
        bool txreq_monitor_en = txreq_handle->MonitorTxDAT(txdat_flit);
        if(txreq_monitor_en) return;
    }

    uint32_t num_rxsnp = rxsnp_handle_vec.size();
    for(uint32_t i = 0; i < num_rxsnp; i++)
    {
        ISnpHandle *rxsnp_handle = rxsnp_handle_vec[i];
        bool rxsnp_monitor_en = rxsnp_handle->MonitorTxDAT(txdat_flit);
        if(rxsnp_monitor_en) return;
    }

    assert(0);
}


void ProtocolNetworkLayer::MonitorRxSNP(SNPFLIT *rxsnp_flit)
{
    if(rxsnp_flit != NULL) wait_rxsnp_flit_vec.push_back(rxsnp_flit);

    for(uint32_t i = 0; i < wait_rxsnp_flit_vec.size();)
    {
        SNPFLIT *wait_rxsnp = wait_rxsnp_flit_vec[i];
        uint64_t snp_addr = wait_rxsnp->Addr;

        CacheLine *cache_line = cache->FindCacheLine(snp_addr);
        if(cache_line == NULL) cache_line = cache->NewCacheLine(snp_addr);

        if(cache_line->snp_is_pending) assert(0);
        if(cache_line->snp_response_is_locked)
        {
            i++; // next rxsnp_flit
            continue;
        }

        ISnpHandle *rxsnp_handle = NULL;
        switch(wait_rxsnp->Opcode)
        {
            case SnpOpcodeEncoding::SnpClean:
                rxsnp_handle = new SnpCleanHandle(wait_rxsnp, cache_line);
                break;
            case SnpOpcodeEncoding::SnpShared:
                rxsnp_handle = new SnpSharedHandle(wait_rxsnp, cache_line);
                break;
            case SnpOpcodeEncoding::SnpUnique:
                rxsnp_handle = new SnpUniqueHandle(wait_rxsnp, cache_line);
                break;
            
            case SnpOpcodeEncoding::SnpCleanInvalid:
                rxsnp_handle = new SnpCleanInvalidHandle(wait_rxsnp, cache_line);
                break;
            case SnpOpcodeEncoding::SnpCleanShared:
                rxsnp_handle = new SnpCleanSharedHandle(wait_rxsnp, cache_line);
                break;
            case SnpOpcodeEncoding::SnpMakeInvalid:
                rxsnp_handle = new SnpMakeInvalidHandle(wait_rxsnp, cache_line);
                break;

            case SnpOpcodeEncoding::SnpQuery:
                rxsnp_handle = new SnpQueryHandle(wait_rxsnp, cache_line);
                break;

            // assert(0)
            default:
                assert(0);
                break;
        }
        rxsnp_handle_vec.push_back(rxsnp_handle);

        wait_rxsnp_flit_vec.erase(wait_rxsnp_flit_vec.begin() + i);
    }

}


void ProtocolNetworkLayer::MonitorRxRSP(RSPFLIT *rxrsp_flit)
{
    if(rxrsp_flit == NULL) return;

    uint32_t num_txreq = txreq_handle_vec.size();
    for(uint32_t i = 0; i < num_txreq; i++)
    {
        IReqHandle *txreq_handle = txreq_handle_vec[i];
        bool txreq_monitor_en = txreq_handle->MonitorRxRSP(rxrsp_flit);
        if(txreq_monitor_en) return;
    }
    assert(0);
}


void ProtocolNetworkLayer::MonitorRxDAT(DATFLIT *rxdat_flit)
{
    if(rxdat_flit == NULL) return;

    uint32_t num_txreq = txreq_handle_vec.size();
    for(uint32_t i = 0; i < num_txreq; i++)
    {
        IReqHandle *txreq_handle = txreq_handle_vec[i];
        bool txreq_monitor_en = txreq_handle->MonitorRxDAT(rxdat_flit);
        if(txreq_monitor_en) return;
    }
    assert(0);
}






/******************************************************/
/*  CHI CheckDone Function                            */
/******************************************************/
void ProtocolNetworkLayer::CHI_CheckDone()
{
    // check txreq handle if it is done
    for(uint32_t i = 0; i < txreq_handle_vec.size();)
    {
        IReqHandle *txreq_handle = txreq_handle_vec[i];
        if(!txreq_handle->IsDone())
        {
            i++;
            continue;
        }

        uint32_t txnid_free = txreq_handle->GetTxnID();
        FreeTxnID(txnid_free);
        txreq_handle_vec.erase(txreq_handle_vec.begin() + i);
        delete txreq_handle;
    }

    // check rxsnp handle if it is done
    for(uint32_t i = 0; i < rxsnp_handle_vec.size();)
    {
        ISnpHandle *rxsnp_handle = rxsnp_handle_vec[i];
        if(!rxsnp_handle->IsDone())
        {
            i++;
            continue;
        }

        rxsnp_handle_vec.erase(rxsnp_handle_vec.begin() + i);
        delete rxsnp_handle;
    }
}





}
#endif

