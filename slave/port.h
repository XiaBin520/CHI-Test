
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
    REQFlit* rxreqflit;
    RSPFlit* rxrspflit;
    DATFlit* rxdatflit;

    SNPFlit* txsnpflit;
    RSPFlit* txrspflit;
    DATFlit* txdatflit;


    enum{RxReqID = 0, RxRspID, RxDatID, TxSnpID, TxRspID, TxDatID};

    template<uint32_t channel_id>
    bool payload(bool flitpend, bool flitv, bool lcrdv)
    {
        static bool lastpend = false;

        bool valid_payload = lastpend & flitv & lcrdv;
        lastpend = flitpend;
        return valid_payload;
    }


    void Monitor(VTestTop *dut, LSReq *ls)
    {
        

    }
};