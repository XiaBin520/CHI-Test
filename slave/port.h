
#include "chi.h"
#include "loadstore.h"


class VTestTop
{
public:
    bool        txreqflitpend;
    bool        txreqflitv;
    uint32_t    txreqflit[8];
    bool        txreqlcrdv;

    bool        txrspflitpend;
    bool        txrspflitv;
    uint32_t    txrspflit[8];
    bool        txrsplcrdv;

    bool        txdatflitpend;
    bool        txdatflitv;
    uint32_t    txdatflit[8];
    bool        txdatlcrdv;

// ===============================
    bool        rxsnpflitpend;
    bool        rxsnpflitv;
    uint32_t    rxsnpflit[8];
    bool        rxsnplcrdv;

    bool        rxrspflitpend;
    bool        rxrspflitv;
    uint32_t    rxrspflit[8];
    bool        rxrsplcrdv;

    bool        rxdatflitpend;
    bool        rxdatflitv;
    uint32_t    rxdatflit[8];
    bool        rxdatlcrdv;

};



class Port
{
public:
    LSReq*   rxlsreq;
    REQFlit* rxreqflit;
    RSPFlit* rxrspflit;
    DATFlit* rxdatflit;

    SNPFlit* txsnpflit;
    RSPFlit* txrspflit;
    DATFlit* txdatflit;

    queue<SNPFlit*> txsnpflit_queue;
    queue<RSPFlit*> txrspflit_queue;
    queue<DATFlit*> txdatflit_queue;

    enum{RxReqID = 0, RxRspID, RxDatID, TxSnpID, TxRspID, TxDatID};

    template<uint32_t channel_id, typename TT>
    TT* Payload(bool flitpend, bool flitv, bool lcrdv, uint32_t flit[])
    {
        static bool lastpend = false;

        bool valid = lastpend & flitv & lcrdv;
        lastpend = flitpend;

        if(valid)
        {
            TT *temp = new TT();
            TT->GetField(flit);
            return temp;
        }
        else
        {
            return NULL;
        }
    }


    void Monitor(VTestTop *dut, LSReq *ls)
    {
        if(ls->valid)
        {
            rxlsreq = new LSReq();
            rxlsreq->valid = true;
            rxlsreq->opcode = ls->opcode;
            rxlsreq->addr   = ls->addr;
        }
        else
        {
            rxlsreq = NULL;
        }
        
        rxreqflit = Payload<RxReqID, REQFlit>(dut->txreqflitpend, dut->txreqflitv, dut->txreqlcrdv, dut->txreqflit);
        rxrspflit = Payload<RxRspID, RSPFlit>(dut->txrspflitpend, dut->txrspflitv, dut->txrsplcrdv, dut->txrspflit);
        rxdatflit = Payload<RxDatID, DATFlit>(dut->txdatflitpend, dut->txdatflitv, dut->txdatlcrdv, dut->txdatflit);

        txsnpflit = Payload<TxSnpID, SNPFlit>(dut->rxsnpflitpend, dut->rxsnpflitv, dut->rxsnplcrdv, dut->rxsnpflit);
        txrspflit = Payload<TxRspID, RSPFlit>(dut->rxrspflitpend, dut->rxrspflitv, dut->rxrsplcrdv, dut->rxrspflit);
        txdatflit = Payload<TxDatID, DATFlit>(dut->rxdatflitpend, dut->rxdatflitv, dut->txdatlcrdv, dut->rxdatflit);


        if(txsnpflit != NULL)
        {
            SNPFlit *temp = txsnpflit_queue.front();
            txsnpflit_queue.pop();
            delete temp;
        }
        if(txrspflit != NULL)
        {
            RSPFlit *temp = txrspflit_queue.front();
            txrspflit_queue.pop();
            delete temp;
        }
        if(txdatflit != NULL)
        {
            DATFlit *temp = txdatflit_queue.front();
            txdatflit_queue.pop();
            delete temp;
        }
    }


    void Driver(VTestTop *dut)
    {
        // RxREQ Channel
        dut->txreqlcrdv = true;
        // RxSNP Channel
        dut->txrsplcrdv = true;
        // RxDAT Channel
        dut->txdatlcrdv = true;

        // TxSNP Channel
        dut->rxsnpflitpend = true;
        if(!txsnpflit_queue.empty())
        {
            dut->rxsnpflitv = true;
            SNPFlit *snpflit = txsnpflit_queue.front();
            snpflit->GetFlit(dut->rxsnpflit);
        }
        else
        {
            dut->rxsnpflitv = false;
        }
        // TxRSP Channel
        dut->rxrspflitpend = true;
        if(!txrspflit_queue.empty())
        {
            dut->rxrspflitv = true;
            RSPFlit *rspflit = txrspflit_queue.front();
            rspflit->GetFlit(dut->rxrspflit);
        }
        else
        {
            dut->rxrspflitv = false;
        }
        // TxDAT Channel
        dut->rxdatflitpend = true;
        if(!txdatflit_queue.empty())
        {
            dut->rxdatflitv = true;
            DATFlit *datflit = txdatflit_queue.front();
            datflit->GetFlit(dut->rxdatflit);
        }
    }
};