#ifndef CHI_HNF_LINK_LAYER_H_
#define CHI_HNF_LINK_LAYER_H_

#include "chi_monitor.h"
#include "chi_driver.h"


namespace CHI_HNF
{

class LinkLayer
{
private:
    ProtocolNetworkLayer *protocol_network_layer;

    CHIDriver *chi_driver;
    CHIMonitor *chi_monitor;



public:
    void PrintMemory(uint8_t mem_data[512][64])
    {
        protocol_network_layer->PrintMemory(mem_data);
    }
    
public:
    ICN_RN_PORTS *icn_rn_ports;
    queue<ACCFLIT*> accreq_flit_queue;
    queue<ACCFLIT*> accrsp_flit_queue;


    LinkLayer()
    {
        this->icn_rn_ports = new ICN_RN_PORTS();
        this->protocol_network_layer = new ProtocolNetworkLayer();

        this->chi_driver = new CHIDriver(protocol_network_layer, icn_rn_ports);
        this->chi_monitor = new CHIMonitor(protocol_network_layer, icn_rn_ports);
    }

    void RUN()
    {
        chi_driver->DriverTxSNP(); 
        chi_driver->DriverTxRSP();
        chi_driver->DriverTxDAT();
        chi_monitor->MonitorTxRSP();
        chi_monitor->MonitorTxDAT();
        chi_monitor->MonitorRxRSP();
        chi_monitor->MonitorRxDAT();
        protocol_network_layer->CHIReq_CheckDone();
        protocol_network_layer->CHISnp_CheckDone(); 
        chi_monitor->MonitorRxREQ(); 


        protocol_network_layer->DriverAccRSP(accrsp_flit_queue);
        protocol_network_layer->ACC_CheckDone();
        protocol_network_layer->MonitorAccREQ(accreq_flit_queue);
    }
};





}
#endif