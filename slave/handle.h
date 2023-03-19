
#include "chi.h"
#include "loadstore.h"
#include "port.h"
#include "alloc_cacheline.h"

class CHIReqExtension
{
public:
    REQFlit *reqflit;
    uint32_t input_txnid;
    uint32_t output_txnid;
};


class LSReqExtension
{
public:
    LSReq *lsreq;
    uint32_t output_txnid;
};



class Handle
{
public:
    vector<CHIReqExtension*> chireq_vec;
    vector<LSReqExtension*>  lsreq_vec;

    uint32_t output_txnid;

    IHandleAccept *handle_accept_arr[1024*4];

    IHandleSent *handle_response_arr[1024*4];

    uint32_t AllocTxnID()
    {
        uint32_t last_txnid = output_txnid;
        output_txnid = (output_txnid > 1024) ? 0 : (output_txnid + 1);
        return last_txnid;
    }

    void HandleLSReq(Port *port, AllocCacheLine *alloc_cacheline)
    {
        bool txsnpchannel_is_free = !port->TxSnpChannelIsBusy();
        bool has_lsreq_payload = (alloc_cacheline->lsreq != NULL);

        if(txsnpchannel_is_free & has_lsreq_payload)
        {
            LSReqExtension *lsreq_extension = new LSReqExtension();
            lsreq_extension->lsreq = alloc_cacheline->lsreq;
            lsreq_extension->output_txnid = AllocTxnID();
            lsreq_vec.push_back(lsreq_extension);

            SNPFlit *txsnpflit = SetTxSNPFlit(lsreq_extension);
            port->handling_txsnpflit_queue.push(txsnpflit);

            SnoopAccept *handle_snp_accept = new SnoopAccept();
            uint32_t index = lsreq_extension->output_txnid;
            handle_accept_arr[index] = handle_snp_accept;
        }
    }


    void HandleCHIReq(Port *port, AllocCacheLine *alloc_cacheline)
    {
        // !alloc_cacheline->chireq_queue.empty() ----> it is the mean that has chireq payload in chireq_queue
        while(!alloc_cacheline->chireq_queue.empty())
        {
            REQFlit *reqflit = alloc_cacheline->chireq_queue.front();
            alloc_cacheline->chireq_queue.pop();

            CHIReqExtension *chireq_extension = new CHIReqExtension();
            chireq_extension->reqflit = reqflit;
            chireq_extension->input_txnid = reqflit->txnid;
            chireq_extension->output_txnid = AllocTxnID();

            uint32_t index = chireq_extension->output_txnid;
            switch(chireq_extension->reqflit->opcode)
            {
                case REQChannelOpcode::ReadClean:
                case REQChannelOpcode::ReadNotSharedDirty:
                case REQChannelOpcode::ReadShared:
                case REQChannelOpcode::ReadUnique:
                case REQChannelOpcode::ReadPreferUnique:
                case REQChannelOpcode::MakeReadUnique:
                {
                    ReadSent *read_response = new ReadSent();
                    handle_response_arr[index] = read_response;

                    ReadAccept *read_accept = new ReadAccept(false);
                    handle_accept_arr[index] = read_accept;
                    break;
                }                    
                case REQChannelOpcode::CleanUnique:
                case REQChannelOpcode::MakeUnique:
                case REQChannelOpcode::Evict:
                case REQChannelOpcode::CleanShared:
                case REQChannelOpcode::CleanSharedPersist:
                case REQChannelOpcode::CleanInvalid:
                case REQChannelOpcode::MakeInvalid:
                {
                    DatalessSent *dataless_response = new DatalessSent();
                    handle_response_arr[index] = dataless_response;

                    DatalessAccept *dataless_accept = new DatalessAccept(false);
                    handle_accept_arr[index] = dataless_accept;
                    break;
                }
                case REQChannelOpcode::WriteNoSnpFull:
                case REQChannelOpcode::WriteNoSnpPtl:
                case REQChannelOpcode::WriteUniqueFull:
                case REQChannelOpcode::WriteUniquePtl:
                {
                    NonCopyBackSent *noncopyback_response = new NonCopyBackSent();
                    handle_response_arr[index] = noncopyback_response;

                    NonCopyBackAccept *noncopyback_accept = new NonCopyBackAccept(false);
                    handle_accept_arr[index] = noncopyback_accept;
                    break;
                }
                case REQChannelOpcode::WriteBackFull:
                case REQChannelOpcode::WriteBackPtl:
                case REQChannelOpcode::WriteCleanFull:
                case REQChannelOpcode::WriteEvictFull:
                {
                    CopyBackSent *copyback_response = new CopyBackSent();
                    handle_response_arr[index] = copyback_response;

                    CopyBackAccept *copyback_accept = new CopyBackAccept();
                    handle_accept_arr[index] = copyback_accept;
                    break;
                }
                case REQChannelOpcode::WriteEvictOrEvict:
                {
                    WriteEvictOrEvictSent *req_response = new WriteEvictOrEvictSent();
                    handle_response_arr[index] = req_response;

                    WriteEvictOrEvictAccept *req_accept = new WriteEvictOrEvictAccept();
                    handle_accept_arr[index] = req_accept;
                    break;
                }
                default:
                {
                    assert(0);
                    break;
                }
            }
        }


        // handle Response in TxRsp and TxDat Channel
        uint32_t rand_sel_txrsp = 0;
        uint32_t rand_sel_txdat = 0;
        
        bool has_chireq = !chireq_vec.empty();
        bool txrspchannel_is_free = !port->TxRspChannelIsBusy();
        bool txdatchannel_is_free = !port->TxDatChannelIsBusy();

        if(has_chireq & txrspchannel_is_free)
        {
            uint32_t txrsp_txnid = chireq_vec[rand_sel_txrsp]->output_txnid;
            IHandleSent *txrsp_response = handle_response_arr[txrsp_txnid];
            txrsp_response->SetTxRSPFlit(port->handling_txrspflit_queue);
        }
        if(has_chireq & txdatchannel_is_free)
        {
            uint32_t txdat_txnid = chireq_vec[rand_sel_txdat]->output_txnid;
            IHandleSent *txdat_response = handle_response_arr[txdat_txnid];
            txdat_response->SetTxDATFlit(port->handling_txdatflit_queue);
        }


    }






    SNPFlit* SetTxSNPFlit(LSReqExtension *lsreq_extension)
    {
        static uint32_t op_sel = 0;

        SNPFlit *snpflit = new SNPFlit();
        snpflit->qos = 0;
        snpflit->txnid = lsreq_extension->output_txnid;
        snpflit->addr  = lsreq_extension->lsreq->addr;
        return snpflit;
    }







};



class IHandleSent
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

    virtual void SetTxRSPFlit(queue<RSPFlit*>& rspflit_queue) {return;}
    virtual void SetTxDATFlit(queue<DATFlit*>& datflit_queue) {return;}
};



class ReadSent : public IHandleSent
{
public:
    bool set_CompData;
    bool set_RespSepData;
    bool set_DataSepResp;
    // bool set_ReadReceipt; // 暂时用不到


    ReadSent()
    {
        set_CompData    = false;
        set_RespSepData = false;
        set_DataSepResp = false;

        rand_sel = 0;
    }


    virtual void SetTxRSPFlit(queue<RSPFlit*>& rspflit_queue)
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
            rspflit_queue.push(rspflit);
        }
        
    }


    virtual void SetTxDATFlit(queue<DATFlit*>& datflit_queue)
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
            datflit_queue.push(datflit);
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
            datflit_queue.push(datflit);
        }
    }

    
};



class DatalessSent : public IHandleSent
{
public:
    bool set_Comp;

    DatalessSent()
    {
        set_Comp = false;
        rand_sel = 0;
    }


    virtual void SetTxRSPFlit(queue<RSPFlit*>& rspflit_queue)
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
            rspflit_queue.push(rspflit);
        }

    }

    virtual void SetTxDATFlit(queue<DATFlit*>& datflit_queue) {return;}
};



class NonCopyBackSent : public IHandleSent
{
public:
    bool set_Comp;
    bool set_DBIDResp;
    bool set_CompDBIDResp;

    NonCopyBackSent()
    {
        set_Comp         = false;
        set_DBIDResp     = false;
        set_CompDBIDResp = false;

        rand_sel = 0;
    }

    virtual void SetTxRSPFlit(queue<RSPFlit*>& rspflit_queue)
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
            rspflit_queue.push(rspflit);
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
            rspflit_queue.push(rspflit);
        }

        /*
        *   The requirements for setting CompDBIDResp:
        *       a. Not set CompDBIDResp
        *       b. Not set Comp
        *       c. Not set DBIDResp
        */
        if((rand_sel == 3) & 
           (!set_CompDBIDResp) & (!set_Comp) & (!set_DBIDResp))
        {
            set_CompDBIDResp = true;

            RSPFlit* rspflit = new RSPFlit();
            rspflit_queue.push(rspflit);
        }

    }

    virtual void SetTxDATFlit(queue<DATFlit*>& datflit_queue) {return;}

};



class CopyBackSent : public IHandleSent
{
public:
    bool set_CompDBIDResp;

    virtual void SetTxRSPFlit(queue<RSPFlit*>& rspflit_queue)
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
            rspflit_queue.push(rspflit);
        }
    }

    virtual void SetTxDATFlit(queue<DATFlit*>& datflit_queue) {return;}
};



class WriteEvictOrEvictSent : public IHandleSent
{
public:
    bool set_Comp;
    bool set_CompDBIDResp;

    virtual void SetTxRSPFlit(queue<RSPFlit*>& rspflit_queue)
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
            rspflit_queue.push(rspflit);
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
            rspflit_queue.push(rspflit);
        }
    }

    virtual void SetTxDATFlit(queue<DATFlit*>& datflit_queue) {return;}

};





class IHandleAccept
{
public:
    virtual void GotRxRspFlit(RSPFlit* rspflit) {return;}
    virtual void GotRxDatFlit(DATFlit* datflit) {return;}
    virtual void GotTxRspFlit(RSPFlit* rspflit) {return;}
    virtual void GotTxDatFlit(DATFlit* datflit) {return;}

    virtual bool Done() {return false;}
};



class ReadAccept : public IHandleAccept
{
public:
    bool got_CompData;
    bool got_RespSepData;
    bool got_DataSepResp;
    bool got_CompAck;

    ReadAccept(bool CompAck = false)
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


class DatalessAccept : public IHandleAccept
{
public:
    bool got_Comp;
    bool got_CompAck;

    DatalessAccept(bool CompAck = false)
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


class NonCopyBackAccept : public IHandleAccept
{
public:
    bool got_Comp;
    bool got_DBIDResp;
    bool got_CompDBIDResp;

    bool got_NonCopyBackWrData;
    bool got_CompAck;

    bool got_NCBWrDataCompAck;

    NonCopyBackAccept(bool CompAck = false)
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


class CopyBackAccept : public IHandleAccept
{
public:
    bool got_CompDBIDResp;
    bool got_CopyBackWrData;

    CopyBackAccept()
    {
        got_CompDBIDResp   = false;
        got_CopyBackWrData = false;
    }

    virtual void GotRxRspFlit(RSPFlit* rspflit) {return;}
    virtual void GotRxDatFlit(DATFlit* datflit) {return;}
    virtual void GotTxRspFlit(RSPFlit* rspflit) {return;}
    virtual void GotTxDatFlit(DATFlit* datflit) {return;}

    virtual bool Done()
    {
        bool sent_done   = got_CompDBIDResp;
        bool accept_done = got_CopyBackWrData;
        return (sent_done & accept_done);
    }
};


class WriteEvictOrEvictAccept : public IHandleAccept
{
public:
    bool got_Comp;
    bool got_CompDBIDresp;
    bool got_CompAck;
    bool got_CopyBackWrData;

    WriteEvictOrEvictAccept()
    {
        got_Comp           = false;
        got_CompDBIDresp   = false;
        got_CompAck        = false;
        got_CopyBackWrData = false;
    }

    virtual void GotRxRspFlit(RSPFlit* rspflit) {return;}
    virtual void GotRxDatFlit(DATFlit* datflit) {return;}
    virtual void GotTxRspFlit(RSPFlit* rspflit) {return;}
    virtual void GotTxDatFlit(DATFlit* datflit) {return;}

    virtual bool Done()
    {
        bool sent_done   = got_Comp | got_CompDBIDresp;
        bool accept_done = got_CompAck | got_CopyBackWrData;
        return (sent_done & accept_done);
    }
};


class SnoopAccept : public IHandleAccept
{
public:
    bool got_SnpResp;
    bool got_SnpRespData;

    virtual void GotRxRspFlit(RSPFlit* rspflit) {return;}
    virtual void GotRxDatFlit(DATFlit* datflit) {return;}
    virtual void GotTxRspFlit(RSPFlit* rspflit) {return;}
    virtual void GotTxDatFlit(DATFlit* datflit) {return;}

    virtual bool Done()
    {
        return (got_SnpResp | got_SnpRespData);
    }
};
