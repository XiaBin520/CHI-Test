#ifndef CHI_RNF_ISNPHANDLE_H_
#define CHI_RNF_ISNPHANDLE_H_


#include "../../CHI-Common/chi_flit.h"
#include "../../cache.h"


namespace CHI_RNF
{

class ISnpHandle
{
protected:
    CacheLine *cache_line;

    SNPFLIT *got_rxsnp_flit;
    vector<RSPFLIT*> got_txrsp_flit_vec;
    vector<DATFLIT*> got_txdat_flit_vec;



public:
    ~ISnpHandle()
    {
        delete got_rxsnp_flit;

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
    }

public:
    virtual void DriverTxRSP(queue<RSPFLIT*>& txrsp_flit_queue) {return;}
    virtual void DriverTxDAT(queue<DATFLIT*>& txdat_flit_queue) {return;}
    virtual bool MonitorTxRSP(RSPFLIT *txrsp_flit) {return false;}
    virtual bool MonitorTxDAT(DATFLIT *txdat_flit) {return false;}
    virtual bool IsDone() {return false;}
};

}
#endif