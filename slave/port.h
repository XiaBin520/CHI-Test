
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
    }


    void Driver(VTestTop *dut, SNPFlit *txsnpflit, RSPFlit *txrspflit, DATFlit *txdatflit)
    {
        // RxREQ Channel
        dut->txreqlcrdv = true;
        // RxSNP Channel
        dut->txrsplcrdv = true;
        // RxDAT Channel
        dut->txdatlcrdv = true;

        // TxSNP Channel
        dut->rxsnpflitpend = true;
        dut->rxsnpflitv    = (txsnpflit != NULL);
        if(txsnpflit != NULL) txsnpflit->GetFlit(dut->rxsnpflit);
        // TxRSP Channel
        dut->rxrspflitpend = true;
        dut->rxrspflitv    = (txrspflit != NULL);
        if(txrspflit != NULL) txrspflit->GetFlit(dut->rxrspflit);
        // TxDAT Channel
        dut->rxdatflitpend = true;
        dut->rxdatflitv    = (txdatflit != NULL);
        if(txdatflit != NULL) txdatflit->GetFlit(dut->rxdatflit);
    }
};