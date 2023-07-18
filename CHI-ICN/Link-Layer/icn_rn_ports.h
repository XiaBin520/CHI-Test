#ifndef CHI_HNF_ICN_RN_PORTS_H_
#define CHI_HNF_ICN_RN_PORTS_H_

#include "../Protocol-Network-Layer/protocol-network-layer.h"


namespace CHI_HNF
{


class ICN_RN_PORTS
{
public:
    bool    TXSACTIVE;
    bool    RXSACTIVE;


    bool    TXLINKACTIVEREQ;
    bool    TXLINKACTIVEACK;

    bool    RXLINKACTIVEREQ;
    bool    RXLINKACTIVEACK;


    bool        RXREQFLITPEND;
    bool        RXREQFLITV;
    uint32_t    RXREQFLIT[(uint32_t)ceil(Req_Total_Width/32.0)];
    bool        RXREQLCRDV;

    bool        RXRSPFLITPEND;
    bool        RXRSPFLITV;
    uint32_t    RXRSPFLIT[(uint32_t)ceil(Rsp_Total_Width/32.0)];
    bool        RXRSPLCRDV;

    bool        RXDATFLITPEND;
    bool        RXDATFLITV;
    uint32_t    RXDATFLIT[(uint32_t)ceil(Dat_Total_Width/32.0)];
    bool        RXDATLCRDV;


    bool        TXSNPFLITPEND;
    bool        TXSNPFLITV;
    uint32_t    TXSNPFLIT[(uint32_t)ceil(Snp_Total_Width/32.0)];
    bool        TXSNPLCRDV;

    bool        TXRSPFLITPEND;
    bool        TXRSPFLITV;
    uint32_t    TXRSPFLIT[(uint32_t)ceil(Rsp_Total_Width/32.0)];
    bool        TXRSPLCRDV;

    bool        TXDATFLITPEND;
    bool        TXDATFLITV;
    uint32_t    TXDATFLIT[(uint32_t)ceil(Dat_Total_Width/32.0)];
    bool        TXDATLCRDV;
};



}
#endif