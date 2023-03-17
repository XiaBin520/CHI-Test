

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
                                     (rand_sel + 1);
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


    ReadSet()
    {
        set_CompData    = false;
        set_RespSepData = false;
        set_DataSepResp = false;

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
        if((rand_sel == 1) & // Set DataSepResp
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

    DatalessSet()
    {
        set_Comp = false;
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



class NonCopyBackSet : public IHandleSet
{
public:
    bool set_Comp;
    bool set_DBIDResp;
    bool set_CompDBIDResp;

    NonCopyBackSet()
    {
        set_Comp         = false;
        set_DBIDResp     = false;
        set_CompDBIDResp = false;

        rand_sel = 0;
    }

    virtual void SetTxRSPFlit(vector<RSPFlit*>& rspflit_vec)
    {
        RandSel();

        /*
        *   The requirements for setting Comp:
        *       a. Not set Comp
        *       b. Not set CompDBIDResp
        */
        if((rand_sel == 0) & // Set Comp
           (!set_Comp) & (!set_CompDBIDResp))
        {
            set_Comp = true;

            RSPFlit* rspflit = new RSPFlit();
            rspflit_vec.push_back(rspflit);
        }

        /*
        *   The requirements for setting DBIDResp:
        *       a. Not set DBIDResp
        *       b. Not set CompDBIDResp
        */
        if((rand_sel == 1) & 
           (!set_DBIDResp) & (!set_CompDBIDResp))
        {
            set_DBIDResp = true;

            RSPFlit* rspflit = new RSPFlit();
            rspflit_vec.push_back(rspflit);
        }

        /*
        *   The requirements for setting CompDBIDResp:
        *       a. Not set CompDBIDResp
        *       b. Not set Comp
        *       c. Not set DBIDResp
        */
        if((rand_sel == 3) & 
           (!set_CompDBIDResp) & (!set_Comp) (!set_DBIDResp))
        {
            set_CompDBIDResp = true;

            RSPFlit* rspflit = new RSPFlit();
            rspflit_vec.push_back(rspflit);
        }

    }

    virtual void SetTxDATFlit(vector<DATFlit*>& datflit_vec) {return;}

};



class CopyBackSet : public IHandleSet
{
public:
    bool set_CompDBIDResp;

    virtual void SetTxRSPFlit(vector<RSPFlit*>& rspflit_vec)
    {
        RandSel();

        /*
        *   The requirements for setting CompDBIDResp
        *       a. Not set CompDBIDResp
        */
        if((rand_sel == 0) &
           (!set_CompDBIDResp))
        {
            set_CompDBIDResp = true;

            RSPFlit* rspflit = new RSPFlit();
            rspflit_vec.push_back(rspflit);
        }
    }

    virtual void SetTxDATFlit(vector<DATFlit*>& datflit_vec) {return;}
};



class WriteEvictOrEvictSet : public IHandleSet
{
public:
    bool set_Comp;
    bool set_CompDBIDResp;

    virtual void SetTxRSPFlit(vector<RSPFlit*>& rspflit_vec)
    {
        RandSel();

        /*
        *   The requirements for setting Comp
        *       a. Not set Comp
        *       b. Not set CompDBIDResp
        */
        if((rand_sel == 0) &
           (!set_Comp) & (!set_CompDBIDResp))
        {
            set_Comp = true;

            RSPFlit* rspflit = new RSPFlit();
            rspflit_vec.push_back(rspflit);
        }

        /*
        *   The requirements for setting CompDBIDResp
        *       a. Not set CompDBIDResp
        *       b. Not set Comp
        */
        if((rand_sel == 0) &
           (!set_CompDBIDResp) & (!set_Comp))
        {
            set_CompDBIDResp = true;

            RSPFlit* rspflit = new RSPFlit();
            rspflit_vec.push_back(rspflit);
        }
    }

    virtual void SetTxDATFlit(vector<DATFlit*>& datflit_vec) {return;}

};




class IHandleGot
{
public:
    virtual void GotRxRspFlit(RSPFlit* rspflit) {return;}
    virtual void GotRxDatFlit(DATFlit* datflit) {return;}
    virtual void GotTxRspFlit(RSPFlit* rspflit) {return;}
    virtual void GotTxDatFlit(DATFlit* datflit) {return;}

    virtual bool Done() {return false;}
};



class ReadGot : public IHandleGot
{
public:
    bool got_CompData;
    bool got_RespSepData;
    bool got_DataSepResp;
    bool got_CompAck;

    ReadGot(bool CompAck = false)
    {
        got_CompData    = false;
        got_RespSepData = false;
        got_DataSepResp = false;
        got_CompAck     = CompAck;
    }

    virtual void GotRxRspFlit(RSPFlit* rspflit) {return;}
    virtual void GotRxDatFlit(DATFlit* datflit) {return;}
    virtual void GotTxRspFlit(RSPFlit* rspflit) {return;}
    virtual void GotTxDatFlit(DATFlit* datflit) {return;}

    virtual bool Done()
    {
        bool sent_done   = got_CompData | (got_RespSepData & got_DataSepResp);
        bool accept_done = got_CompAck;
        return (sent_done & accept_done);
    }
};



class DatalessGot : public IHandleGot
{
public:
    bool got_Comp;
    bool got_CompAck;

    DatalessGot(bool CompAck)
    {
        got_Comp = false;
        got_CompAck = false;
    }

    virtual void GotRxRspFlit(RSPFlit* rspflit) {return;}
    virtual void GotRxDatFlit(DATFlit* datflit) {return;}
    virtual void GotTxRspFlit(RSPFlit* rspflit) {return;}
    virtual void GotTxDatFlit(DATFlit* datflit) {return;}

    virtual bool Done()
    {
        bool sent_done = got_Comp;
        bool accept_done = got_CompAck;
        return (sent_done & accept_done);
    }
};


class NonCopyBackGot : public IHandleGot
{
public:
    bool got_Comp;
    bool got_DBIDResp;
    bool got_CompDBIDResp;

    bool got_NonCopyBackWrData;
    bool got_CompAck;

    bool got_NCBWrDataCompAck;

    NonCopyBackGot(bool CompAck)
    {   
        got_Comp              = false;
        got_DBIDResp          = false;
        got_CompDBIDResp      = false;
        got_NonCopyBackWrData = false;
        got_CompAck           = CompAck;
        got_NCBWrDataCompAck  = false;
    }

    virtual void GotRxRspFlit(RSPFlit* rspflit) {return;}
    virtual void GotRxDatFlit(DATFlit* datflit) {return;}
    virtual void GotTxRspFlit(RSPFlit* rspflit) {return;}
    virtual void GotTxDatFlit(DATFlit* datflit) {return;}

    virtual bool Done()
    {
        bool sent_done = got_CompDBIDResp | (got_Comp & got_DBIDResp);
        bool accept_done = (got_NonCopyBackWrData & got_CompAck) | got_NCBWrDataCompAck;
        return (sent_done & accept_done);
    }

};


