#ifndef CHI_HNF_IREQHANDLE_H_
#define CHI_HNF_IREQHANDLE_H_

#include "../../CHI-Common/chi_flit.h"
#include "../memory.h"

namespace CHI_HNF
{



class IReqHandle
{
protected:
    uint32_t dbid;
    MemLine *mem_line;

    REQFLIT *got_rxreq_flit;
    vector<RSPFLIT*> got_rxrsp_flit_vec;
    vector<DATFLIT*> got_rxdat_flit_vec;
    vector<RSPFLIT*> got_txrsp_flit_vec;
    vector<DATFLIT*> got_txdat_flit_vec;



public:
    ~IReqHandle()
    {
        delete got_rxreq_flit;

        while(!got_rxrsp_flit_vec.empty())
        {
            RSPFLIT *temp_rxrsp = got_rxrsp_flit_vec.back();
            got_rxrsp_flit_vec.pop_back();
            delete temp_rxrsp;
        }

        while(!got_rxdat_flit_vec.empty())
        {
            DATFLIT *temp_rxdat = got_rxdat_flit_vec.back();
            got_rxdat_flit_vec.pop_back();
            delete temp_rxdat;
        }

        while(!got_txrsp_flit_vec.empty())
        {
            RSPFLIT *temp_txrsp = got_txrsp_flit_vec.back();
            got_txrsp_flit_vec.pop_back();
            delete temp_txrsp;
        }

        while(!got_txdat_flit_vec.empty())
        {
            DATFLIT *temp_txdat = got_txdat_flit_vec.back();
            got_txdat_flit_vec.pop_back();
            delete temp_txdat;
        }
    }

    virtual void DriverTxRSP(queue<RSPFLIT*>& txrsp_flit_queue) {return;}
    virtual void DriverTxDAT(queue<DATFLIT*>& txdat_flit_queue) {return;}
    virtual bool MonitorTxRSP(RSPFLIT *txrsp_flit) {return false;}
    virtual bool MonitorTxDAT(DATFLIT *txdat_flit) {return false;}
    virtual bool MonitorRxRSP(RSPFLIT *rxrsp_flit) {return false;}
    virtual bool MonitorRxDAT(DATFLIT *rxdat_flit) {return false;}
    virtual bool IsDone() {return false;}

    uint32_t GetDBID() {return dbid;}
};




}
#endif