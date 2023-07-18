#ifndef CHI_HNF_CHI_DRIVER_H_
#define CHI_HNF_CHI_DRIVER_H_

#include "../Protocol-Network-Layer/protocol-network-layer.h"
#include "../CHI-Common/chi_flit_to_packet.h"
#include "icn_rn_ports.h"


namespace CHI_HNF
{



class CHIDriver
{
private:
    ProtocolNetworkLayer *protocol_network_layer;
    ICN_RN_PORTS *icn_rn_ports;

private:
    FlitToPacket *flit_to_packet;

private:
    queue<SNPFLIT*> txsnp_flit_queue;
    queue<RSPFLIT*> txrsp_flit_queue;
    queue<DATFLIT*> txdat_flit_queue;




public:
    CHIDriver(ProtocolNetworkLayer *protocol_network_layer, ICN_RN_PORTS *icn_rn_ports);

    void DriverTxSNP();
    void DriverTxRSP();
    void DriverTxDAT();
};





CHIDriver::CHIDriver(ProtocolNetworkLayer *protocol_network_layer, ICN_RN_PORTS *icn_rn_ports)
{
    this->protocol_network_layer = protocol_network_layer;
    this->icn_rn_ports = icn_rn_ports;

    flit_to_packet = new FlitToPacket();
}






void CHIDriver::DriverTxSNP()
{
    if(txsnp_flit_queue.empty())
    {
        uint32_t rand_sel = rand() % 10;
        protocol_network_layer->DriverTxSNP(txsnp_flit_queue, rand_sel);
    }

    bool txsnp_packet_valid = false;
    if(!txsnp_flit_queue.empty())
    {
        SNPFLIT *txsnp_flit = txsnp_flit_queue.front();
        txsnp_flit_queue.pop();
        flit_to_packet->TransformSNP(txsnp_flit, icn_rn_ports->TXSNPFLIT);
        txsnp_packet_valid = true;
    }
    icn_rn_ports->TXSNPFLITV = txsnp_packet_valid;
}


void CHIDriver::DriverTxRSP()
{
    if(txrsp_flit_queue.empty())
    {
        uint32_t rand_sel = rand() % 10;
        protocol_network_layer->DriverTxRSP(txrsp_flit_queue, rand_sel);
    }

    bool txrsp_packet_valid = false;
    if(!txrsp_flit_queue.empty())
    {
        RSPFLIT *txrsp_flit = txrsp_flit_queue.front();
        txrsp_flit_queue.pop();
        flit_to_packet->TransformRSP(txrsp_flit, icn_rn_ports->TXRSPFLIT);
        delete txrsp_flit;
        txrsp_packet_valid = true;
    }
    icn_rn_ports->TXRSPFLITV = txrsp_packet_valid;
}


void CHIDriver::DriverTxDAT()
{
    if(txdat_flit_queue.empty())
    {
        uint32_t rand_sel = rand() % 10;
        protocol_network_layer->DriverTxDAT(txdat_flit_queue, rand_sel);
    }

    bool txdat_packet_valid = false;
    if(!txdat_flit_queue.empty())
    {
        DATFLIT *txdat_flit = txdat_flit_queue.front();
        txdat_flit_queue.pop();
        flit_to_packet->TransformDAT(txdat_flit, icn_rn_ports->TXDATFLIT);
        delete txdat_flit;
        txdat_packet_valid = true;
    }
    icn_rn_ports->TXDATFLITV = txdat_packet_valid;
}




}
#endif