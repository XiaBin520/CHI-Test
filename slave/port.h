
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


