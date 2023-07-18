#ifndef CHI_HNF_CHI_MONITOR_H_
#define CHI_HNF_CHI_MONITOR_H_


#include "../Protocol-Network-Layer/protocol-network-layer.h"
#include "../CHI-Common/chi_packet_to_flit.h"
#include "icn_rn_ports.h"


namespace CHI_HNF
{


class CHIMonitor
{
private:
    ProtocolNetworkLayer *protocol_network_layer;
    ICN_RN_PORTS *icn_rn_ports;

private:
    PacketToFlit *packet_to_flit;



public:
    CHIMonitor(ProtocolNetworkLayer *protocol_network_layer, ICN_RN_PORTS *icn_rn_ports);

    void MonitorTxRSP();
    void MonitorTxDAT();
    void MonitorRxREQ();
    void MonitorRxRSP();
    void MonitorRxDAT();
};







CHIMonitor::CHIMonitor(ProtocolNetworkLayer *protocol_network_layer, ICN_RN_PORTS *icn_rn_ports)
{
    this->protocol_network_layer = protocol_network_layer;
    this->icn_rn_ports = icn_rn_ports;

    packet_to_flit = new PacketToFlit();
}



void CHIMonitor::MonitorTxRSP()
{
    if(icn_rn_ports->TXRSPFLITV)
    {
        RSPFLIT *txrsp_flit = new RSPFLIT();
        packet_to_flit->TransformRSP(icn_rn_ports->TXRSPFLIT, txrsp_flit);
        protocol_network_layer->MonitorTxRSP(txrsp_flit);
    }
}

void CHIMonitor::MonitorTxDAT()
{
    if(icn_rn_ports->TXDATFLITV)
    {
        DATFLIT *txdat_flit = new DATFLIT();
        packet_to_flit->TransformDAT(icn_rn_ports->TXDATFLIT, txdat_flit);
        protocol_network_layer->MonitorTxDAT(txdat_flit);
    }
}

void CHIMonitor::MonitorRxREQ()
{
    if(icn_rn_ports->RXREQFLITV)
    {
        REQFLIT *rxreq_flit = new REQFLIT();
        packet_to_flit->TransformREQ(icn_rn_ports->RXREQFLIT, rxreq_flit);
        protocol_network_layer->MonitorRxREQ(rxreq_flit);
    }
}

void CHIMonitor::MonitorRxRSP()
{
    if(icn_rn_ports->RXRSPFLITV)
    {
        RSPFLIT *rxrsp_flit = new RSPFLIT();
        packet_to_flit->TransformRSP(icn_rn_ports->RXRSPFLIT, rxrsp_flit);
        protocol_network_layer->MonitorRxRSP(rxrsp_flit);
    }
}

void CHIMonitor::MonitorRxDAT()
{
    if(icn_rn_ports->RXDATFLITV)
    {
        DATFLIT *rxdat_flit = new DATFLIT();
        packet_to_flit->TransformDAT(icn_rn_ports->RXDATFLIT, rxdat_flit);
        protocol_network_layer->MonitorRxDAT(rxdat_flit);
    }
}



}
#endif