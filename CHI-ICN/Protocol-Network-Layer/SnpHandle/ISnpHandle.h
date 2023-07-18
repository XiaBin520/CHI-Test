#ifndef CHI_HNF_ISNPHANDLE_H_
#define CHI_HNF_ISNPHANDLE_H_

#include "../../CHI-Common/chi_flit.h"
#include "../../ACC-Common/acc_flit.h"
#include "../memory.h"


namespace CHI_HNF
{


class ISnpHandle
{
protected:
    MemLine *mem_line;

    SNPFLIT *got_txsnp_flit;
    vector<RSPFLIT*> got_rxrsp_flit_vec;
    vector<DATFLIT*> got_rxdat_flit_vec;


public:
    ~ISnpHandle()
    {
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

    virtual bool MonitorRxRSP(RSPFLIT *rxrsp_flit) {return false;}
    virtual bool MonitorRxDAT(DATFLIT *rxdat_flit) {return false;}
    virtual bool IsDone() {return false;}

    uint32_t GetTxnID() {return got_txsnp_flit->TxnID;}
};




}
#endif