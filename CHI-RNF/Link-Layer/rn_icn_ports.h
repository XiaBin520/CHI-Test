#ifndef CHI_RNF_RN_ICN_PORTS_H_
#define CHI_RNF_RN_ICN_PORTS_H_

#include "../Protocol-Network-Layer/protocol-network-layer.h"

namespace CHI_RNF
{



class RN_ICN_Ports
{
public:
    bool    TXSACTIVE;
    bool    RXSACTIVE;


    bool    TXLINKACTIVEREQ;
    bool    TXLINKACTIVEACK;

    bool    RXLINKACTIVEREQ;
    bool    RXLINKACTIVEACK;


    bool        TXREQFLITPEND;
    bool        TXREQFLITV;
    uint32_t    TXREQFLIT[(uint32_t)ceil(Req_Total_Width/32.0)];
    bool        TXREQLCRDV;

    bool        TXRSPFLITPEND;
    bool        TXRSPFLITV;
    uint32_t    TXRSPFLIT[(uint32_t)ceil(Rsp_Total_Width/32.0)];
    bool        TXRSPLCRDV;

    bool        TXDATFLITPEND;
    bool        TXDATFLITV;
    uint32_t    TXDATFLIT[(uint32_t)ceil(Dat_Total_Width/32.0)];
    bool        TXDATLCRDV;


    bool        RXSNPFLITPEND;
    bool        RXSNPFLITV;
    uint32_t    RXSNPFLIT[(uint32_t)ceil(Snp_Total_Width/32.0)];
    bool        RXSNPLCRDV;

    bool        RXRSPFLITPEND;
    bool        RXRSPFLITV;
    uint32_t    RXRSPFLIT[(uint32_t)ceil(Rsp_Total_Width/32.0)];
    bool        RXRSPLCRDV;

    bool        RXDATFLITPEND;
    bool        RXDATFLITV;
    uint32_t    RXDATFLIT[(uint32_t)ceil(Dat_Total_Width/32.0)];
    bool        RXDATLCRDV;

};



}
#endif