
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

    queue<SNPFlit*> handling_txsnpflit_queue;
    queue<RSPFlit*> handling_txrspflit_queue;
    queue<DATFlit*> handling_txdatflit_queue;


    bool TxSnpChannelIsBusy() {return !handling_txdatflit_queue.empty();}
    bool TxRspChannelIsBusy() {return !handling_txrspflit_queue.empty();}
    bool TxDatChannelIsBusy() {return !handling_txdatflit_queue.empty();}

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


        bool txsnpflit_was_accpeted = (txsnpflit != NULL);
        if(txsnpflit_was_accpeted)
        {
            SNPFlit *temp = handling_txsnpflit_queue.front();
            handling_txsnpflit_queue.pop();
            delete temp;
            delete txsnpflit;
        }

        bool txrspflit_was_accepted = (txrspflit != NULL);
        if(txrspflit_was_accepted)
        {
            RSPFlit *temp = handling_txrspflit_queue.front();
            handling_txrspflit_queue.pop();
            delete temp;
        }

        bool txdatflit_was_accepted = (txdatflit != NULL);
        if(txdatflit_was_accepted)
        {
            DATFlit *temp = handling_txdatflit_queue.front();
            handling_txdatflit_queue.pop();
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
        bool has_txsnpflit_payload = !handling_txsnpflit_queue.empty();
        if(has_txsnpflit_payload)
        {
            dut->rxsnpflitv  = true;
            SNPFlit *snpflit = handling_txsnpflit_queue.front();
            snpflit->GetFlit(dut->rxsnpflit);
        }
        else
        {
            dut->rxsnpflitv = false;
        }
        // TxRSP Channel
        dut->rxrspflitpend = true;
        bool has_txrspflit_payload = !handling_txrspflit_queue.empty();
        if(has_txrspflit_payload)
        {
            dut->rxrspflitv  = true;
            RSPFlit *rspflit = handling_txrspflit_queue.front();
            rspflit->GetFlit(dut->rxrspflit);
        }
        else
        {
            dut->rxrspflitv = false;
        }
        // TxDAT Channel
        dut->rxdatflitpend = true;
        bool has_txdatflit_payload = !handling_txdatflit_queue.empty();
        if(has_txdatflit_payload)
        {
            dut->rxdatflitv  = true;
            DATFlit *datflit = handling_txdatflit_queue.front();
            datflit->GetFlit(dut->rxdatflit);
        }
        {
            dut->rxdatflitv = false;
        }
    }
};