#ifndef CHI_RNF_CHI_MONITOR_H_
#define CHI_RNF_CHI_MONITOR_H_


#include "../CHI-Common/chi_packet_to_flit.h"
#include "../Protocol-Network-Layer/protocol-network-layer.h"
#include "rn_icn_ports.h"


namespace CHI_RNF
{


class CHIMonitor
{
private:
    ProtocolNetworkLayer *protocol_network_layer;
    RN_ICN_Ports *rn_icn_ports;
private:
    PacketToFlit *packet_to_flit;


public:
    CHIMonitor(ProtocolNetworkLayer *protocol_network_layer, RN_ICN_Ports *rn_icn_ports);
public:
    void MonitorTxRSP();
    void MonitorTxDAT();
    void MonitorRxSNP();
    void MonitorRxRSP();
    void MonitorRxDAT();

};



CHIMonitor::CHIMonitor(ProtocolNetworkLayer *protocol_network_layer, RN_ICN_Ports *rn_icn_ports)
{
    this->protocol_network_layer = protocol_network_layer;
    this->rn_icn_ports = rn_icn_ports;

    packet_to_flit = new PacketToFlit();
}





void CHIMonitor::MonitorTxRSP()
{
    if(rn_icn_ports->TXRSPFLITV)
    {
        RSPFLIT *txrsp_flit = new RSPFLIT();
        packet_to_flit->TransformRSP(rn_icn_ports->TXRSPFLIT, txrsp_flit);
        protocol_network_layer->MonitorTxRSP(txrsp_flit);
    }
}

void CHIMonitor::MonitorTxDAT()
{
    if(rn_icn_ports->TXDATFLITV)
    {
        DATFLIT *txdat_flit = new DATFLIT();
        packet_to_flit->TransformDAT(rn_icn_ports->TXDATFLIT, txdat_flit);
        protocol_network_layer->MonitorTxDAT(txdat_flit);
    }
}

void CHIMonitor::MonitorRxSNP()
{
    if(rn_icn_ports->RXSNPFLITV)
    {
        SNPFLIT *rxsnp_flit = new SNPFLIT();
        packet_to_flit->TransformSNP(rn_icn_ports->RXSNPFLIT, rxsnp_flit);
        protocol_network_layer->MonitorRxSNP(rxsnp_flit);
    }
}

void CHIMonitor::MonitorRxRSP()
{
    if(rn_icn_ports->RXRSPFLITV)
    {
        RSPFLIT *rxrsp_flit = new RSPFLIT();
        packet_to_flit->TransformRSP(rn_icn_ports->RXRSPFLIT, rxrsp_flit);
        protocol_network_layer->MonitorRxRSP(rxrsp_flit);
    }
}

void CHIMonitor::MonitorRxDAT()
{
    if(rn_icn_ports->RXDATFLITV)
    {
        DATFLIT *rxdat_flit = new DATFLIT();
        packet_to_flit->TransformDAT(rn_icn_ports->RXDATFLIT, rxdat_flit);
        protocol_network_layer->MonitorRxDAT(rxdat_flit);
    }
}



}
#endif