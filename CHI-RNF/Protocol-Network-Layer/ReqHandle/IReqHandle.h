#ifndef CHI_RNF_IREQHANDLE_H_
#define CHI_RNF_IREQHANDLE_H_

#include "../../ACC-Common/acc_flit.h"
#include "../../CHI-Common/chi_flit.h"
#include "../../cache.h"

namespace CHI_RNF
{


class IReqHandle
{
protected:
    ACCFLIT *accreq_flit;
    CacheLine *cache_line;

protected:
    uint32_t age;

protected:
    REQFLIT *got_txreq_flit;
    vector<RSPFLIT*> got_txrsp_flit_vec;
    vector<DATFLIT*> got_txdat_flit_vec;
    vector<RSPFLIT*> got_rxrsp_flit_vec;
    vector<DATFLIT*> got_rxdat_flit_vec;


public:
    ~IReqHandle()
    {
        delete got_txreq_flit;

        while(!got_txrsp_flit_vec.empty())
        {
            RSPFLIT *txrsp_flit = got_txrsp_flit_vec.back();
            got_txrsp_flit_vec.pop_back();
            delete txrsp_flit;
        }

        while(!got_txdat_flit_vec.empty())
        {
            DATFLIT *txdat_flit = got_txdat_flit_vec.back();
            got_txdat_flit_vec.pop_back();
            delete txdat_flit;
        }

        while(!got_rxrsp_flit_vec.empty())
        {
            RSPFLIT *rxrsp_flit = got_rxrsp_flit_vec.back();
            got_rxrsp_flit_vec.pop_back();
            delete rxrsp_flit;
        }

        while(!got_rxdat_flit_vec.empty())
        {
            DATFLIT *rxdat_flit = got_rxdat_flit_vec.back();
            got_rxdat_flit_vec.pop_back();
            delete rxdat_flit;
        }

    }

    virtual void DriverTxRSP(queue<RSPFLIT*>& txrsp_flit_queue) {return;}
    virtual void DriverTxDAT(queue<DATFLIT*>& txdat_flit_queue) {return;}

    virtual bool MonitorTxRSP(RSPFLIT *txrsp_flit) {return false;}
    virtual bool MonitorTxDAT(DATFLIT *txdat_flit) {return false;}
    virtual bool MonitorRxRSP(RSPFLIT *rxrsp_flit) {return false;}
    virtual bool MonitorRxDAT(DATFLIT *rxdat_flit) {return false;}
    
public:
    virtual bool IsDone() {return false;}
    virtual uint32_t GetTxnID() {return got_txreq_flit->TxnID;}
};



}
#endif