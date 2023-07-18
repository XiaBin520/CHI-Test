#ifndef CHI_RNF_LINK_LAYER_H_
#define CHI_RNF_LINK_LAYER_H_

#include "chi_driver.h"
#include "chi_monitor.h"



namespace CHI_RNF
{



class LinkLayer
{
private:
    ProtocolNetworkLayer *protocol_network_layer;
    CHIDriver *chi_driver;
    CHIMonitor *chi_monitor;


public:
    queue<ACCFLIT*> accreq_flit_queue;
    queue<ACCFLIT*> accrsp_flit_queue;
    RN_ICN_Ports *rn_icn_ports;


    LinkLayer()
    {
        rn_icn_ports = new RN_ICN_Ports();
        protocol_network_layer = new ProtocolNetworkLayer();

        chi_driver = new CHIDriver(protocol_network_layer, rn_icn_ports);
        chi_monitor = new CHIMonitor(protocol_network_layer, rn_icn_ports);
    }

    void RUN()
    {
        chi_driver->DriverTxREQ();
        chi_driver->DriverTxRSP();
        chi_driver->DriverTxDAT();

        chi_monitor->MonitorTxRSP();
        chi_monitor->MonitorTxDAT();
        chi_monitor->MonitorRxRSP();
        chi_monitor->MonitorRxDAT();
        protocol_network_layer->CHI_CheckDone();

        protocol_network_layer->DriverACCRSP(accrsp_flit_queue);
        protocol_network_layer->ACC_CheckDone();
        protocol_network_layer->MonitorACCREQ(accreq_flit_queue);

        chi_monitor->MonitorRxSNP();

        protocol_network_layer->CleanInvalidCacheLine();
    }


};


}
#endif