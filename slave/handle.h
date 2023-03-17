

#include "chi.h"


class IHandleSet
{
public:
    bool    set_RetryAck;
    bool    set_PCrdGrant;

    uint32_t rand_sel;
    void RandSel()
    {
        rand_sel = (rand_sel > 10) ? 0 : 
                                     (rand_sel+1);
    }

    virtual void SetTxRSPFlit(vector<RSPFlit*>& rspflit_vec) {return;}
    virtual void SetTxDATFlit(vector<DATFlit*>& datflit_vec) {return;}
};



class ReadSet : public IHandleSet
{
public:
    bool set_CompData;
    bool set_RespSepData;
    bool set_DataSepResp;
    // bool set_ReadReceipt; // 暂时用不到


    ReadSet(bool CompData = false, bool RespSepData = false, bool DataSepResp = false)
    {
        set_CompData    = CompData;
        set_RespSepData = RespSepData;
        set_DataSepResp = DataSepResp;

        rand_sel = 0;
    }


    virtual void SetTxRSPFlit(vector<RSPFlit*>& rspflit_vec)
    {
        RandSel();

        /*  
        *   The requirements for setting RespSepData
        *       a. Not set RespSepData
        *       b. Not set DataSepResp
        *       c. Not set CompData
        */
        if((rand_sel == 0) &
           (!set_RespSepData) & (!set_DataSepResp) & (!set_CompData))
        {
            set_RespSepData = true;

            RSPFlit* rspflit = new RSPFlit();
            rspflit_vec.push_back(rspflit);
        }
        
    }


    virtual void SetTxDATFlit(vector<DATFlit*>& datflit_vec)
    {
        RandSel();

        /*  
        *   The requirements for setting CompData
        *       a. Not set CompData
        *       b. Not set DataSepResp
        *       c. Not set RespSepData
        */
        if((rand_sel == 0) & // Set CompData
           (!set_CompData) & (!set_RespSepData) & (!set_DataSepResp))
        {
            set_CompData = true;

            DATFlit* datflit = new DATFlit();
            datflit_vec.push_back(datflit);
        }

        /*
        *   The requirements for setting DataSepResp
        *       a. Not set DataSepResp
        *       b. Not set CompData
        *       c. set RespSepData
        */
        if((rand_sel == 1) &
           (!set_DataSepResp) & (!set_CompData) & set_RespSepData)
        {
            set_DataSepResp = true;
            DATFlit* datflit = new DATFlit();
            datflit_vec.push_back(datflit);
        }
    }

    
};



class DatalessSet : public IHandleSet
{
public:
    bool set_Comp;

    DatalessSet(bool Comp)
    {
        set_Comp = Comp;

        rand_sel = 0;
    }


    virtual void SetTxRSPFlit(vector<RSPFlit*>& rspflit_vec)
    {
        RandSel();
        /*
        *   The requirements for setting DataSepResp
        *       a. Not set Comp
        */
        if((rand_sel == 0) & 
           (!set_Comp))
        {
            set_Comp = true;
            RSPFlit* rspflit = new RSPFlit();
            rspflit_vec.push_back(rspflit);
        }

    }

    virtual void SetTxDATFlit(vector<DATFlit*>& datflit_vec) {return;}
};