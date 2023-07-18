#ifndef CHI_RNF_CHI_DRIVER_H_
#define CHI_RNF_CHI_DRIVER_H_

#include "../CHI-Common/chi_flit_to_packet.h"
#include "../Protocol-Network-Layer/protocol-network-layer.h"
#include "rn_icn_ports.h"


namespace CHI_RNF
{


class CHIDriver
{
private:
    ProtocolNetworkLayer *protocol_network_layer;
    RN_ICN_Ports *rn_icn_ports;
private:
    FlitToPacket *flit_to_packet;

private:
    queue<REQFLIT*> txreq_flit_queue;
    queue<RSPFLIT*> txrsp_flit_queue;
    queue<DATFLIT*> txdat_flit_queue;



public:
    CHIDriver(ProtocolNetworkLayer *protocol_network_layer, RN_ICN_Ports *rn_icn_ports);
public:
    void DriverTxREQ();
    void DriverTxRSP();
    void DriverTxDAT();
};





CHIDriver::CHIDriver(ProtocolNetworkLayer *protocol_network_layer, RN_ICN_Ports *rn_icn_ports)
{
    this->protocol_network_layer = protocol_network_layer;
    this->rn_icn_ports = rn_icn_ports;

    flit_to_packet = new FlitToPacket();
}



void CHIDriver::DriverTxREQ()
{
    if(txreq_flit_queue.empty())
    {
        uint32_t rand_sel = rand() % 10;
        protocol_network_layer->DriverTxREQ(txreq_flit_queue, rand_sel);
    }

    bool txreq_packet_valid = false;
    if(!txreq_flit_queue.empty())
    {
        REQFLIT *txreq_flit = txreq_flit_queue.front();
        txreq_flit_queue.pop();
        flit_to_packet->TransformREQ(txreq_flit, rn_icn_ports->TXREQFLIT);
        txreq_packet_valid = true;
    }

    rn_icn_ports->TXREQFLITV = txreq_packet_valid;
}


void CHIDriver::DriverTxRSP()
{
    if(txrsp_flit_queue.empty())
    {
        bool req_or_snp = ((rand() % 2) == 1);
        uint32_t rand_sel = rand() % 10;
        protocol_network_layer->DriverTxRSP(txrsp_flit_queue, req_or_snp, rand_sel);
    }

    bool txrsp_packet_valid = false;
    if(!txrsp_flit_queue.empty())
    {
        RSPFLIT *txrsp_flit = txrsp_flit_queue.front();
        txrsp_flit_queue.pop();
        flit_to_packet->TransformRSP(txrsp_flit, rn_icn_ports->TXRSPFLIT);
        delete txrsp_flit;
        txrsp_packet_valid = true;
    }

    rn_icn_ports->TXRSPFLITV = txrsp_packet_valid;
}


void CHIDriver::DriverTxDAT()
{
    if(txdat_flit_queue.empty())
    {
        bool req_or_snp = ((rand() % 2) == 1);
        uint32_t rand_sel = rand() % 10;
        protocol_network_layer->DriverTxDAT(txdat_flit_queue, req_or_snp, rand_sel);
    }

    bool txdat_packet_valid = false;
    if(!txdat_flit_queue.empty())
    {
        DATFLIT *txdat_flit = txdat_flit_queue.front();
        txdat_flit_queue.pop();
        flit_to_packet->TransformDAT(txdat_flit, rn_icn_ports->TXDATFLIT);
        delete txdat_flit;
        txdat_packet_valid = true;
    }

    rn_icn_ports->TXDATFLITV = txdat_packet_valid;
}



}
#endif