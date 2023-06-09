#include<stdint.h>
#include<math.h>

#include "common.h"
#include "chi_config.h"


template<uint32_t BITS = 256>
class IChannel
{
public:
    bool        flitpend;
    bool        flitv;
    uint32_t    flit[BITS/32];
    bool        lcrdv;
};

typedef IChannel<(uint32_t)ceil(REQConfig::Total_Width / 32)> REQChannel;
typedef IChannel<(uint32_t)ceil(RSPConfig::Total_Width / 32)> RSPChannel;
typedef IChannel<(uint32_t)ceil(SNPConfig::Total_Width / 32)> SNPChannel;
typedef IChannel<(uint32_t)ceil(DATConfig::Total_Width / 32)> DATChannel;



class REQFlit
{
public:
    uint32_t    qos;
    uint32_t    tgtid;
    uint32_t    srcid;
    uint32_t    txnid;
    uint32_t    returnnid; // ReturnNID, StashNID, {'b0, SLCRepHint[6:0]}
    bool        stashnidvalid; // StashNIDValid, Endian, Deep
    uint32_t    returntxnid; // ReturnTxnID, {6'b0, StashLPIDValid, StashLPID[4:0]}
    uint32_t    opcode;
    uint16_t    size;
    uint64_t    addr;
    bool        ns;
    bool        likelyshared;
    bool        allowretry;
    uint32_t    order;
    uint32_t    pcrdtype;
    uint32_t    memattr;
    bool        snpattr; //SnpAttr, DoDWT
    uint32_t    pgroupid; //{GroupIDExt[2:0], LPID[4:0]}, PGroupID[7:0], StashGroupID[7:0], TagGroupID[7:0]
    bool        excl; // Excl, SnoopMe
    bool        expcompack;
    uint32_t    tagop;
    bool        tracetag;


    queue<bool> flit_bits;

    void GetField(uint32_t reqflit[])
    {
        while(!flit_bits.empty()) flit_bits.pop();

        uint32_t arr_size = ceil(REQConfig::Total_Width / 32);
        for(uint32_t i = 0; i < arr_size; i++)
        {
            ValueToBits(reqflit[i], 32, flit_bits);
        }
        this->qos           = BitsToValue(flit_bits,    REQConfig::QoS_Width);
        this->tgtid         = BitsToValue(flit_bits,    REQConfig::TgtID_Width);
        this->srcid         = BitsToValue(flit_bits,    REQConfig::SrcID_Width);
        this->txnid         = BitsToValue(flit_bits,    REQConfig::TxnID_Width);
        this->returnnid     = BitsToValue(flit_bits,    REQConfig::ReturnNID_Width);
        this->stashnidvalid = BitsToValue(flit_bits,    REQConfig::StashNID_Width);
        this->returntxnid   = BitsToValue(flit_bits,    REQConfig::ReturnTxnID_Width);
        this->opcode        = BitsToValue(flit_bits,    REQConfig::Opcode_Width);
        this->size          = BitsToValue(flit_bits,    REQConfig::Size_Width);
        this->addr          = BitsToValue(flit_bits,    REQConfig::Addr_Width);
        this->ns            = BitsToValue(flit_bits,    REQConfig::NS_Width);
        this->likelyshared  = BitsToValue(flit_bits,    REQConfig::LikelyShared_Width);
        this->allowretry    = BitsToValue(flit_bits,    REQConfig::AllowRetry_Width);
        this->order         = BitsToValue(flit_bits,    REQConfig::Order_Width);
        this->pcrdtype      = BitsToValue(flit_bits,    REQConfig::PCrdType_Width);
        this->memattr       = BitsToValue(flit_bits,    REQConfig::MemAttr_Width);
        this->snpattr       = BitsToValue(flit_bits,    REQConfig::SnpAttr_Width);
        this->pgroupid      = BitsToValue(flit_bits,    REQConfig::PGroupID_Width);
        this->excl          = BitsToValue(flit_bits,    REQConfig::Excl_Width);
        this->expcompack    = BitsToValue(flit_bits,    REQConfig::ExpCompAck_Width);
        this->tagop         = BitsToValue(flit_bits,    REQConfig::TagOp_Width);
        this->tracetag      = BitsToValue(flit_bits,    REQConfig::TraceTag_Width);
    }

    void GetFlit(uint32_t reqflit[])
    {
        while(!flit_bits.empty()) flit_bits.pop();

        ValueToBits(this->qos,              REQConfig::QoS_Width,              flit_bits);
        ValueToBits(this->tgtid,            REQConfig::TgtID_Width,            flit_bits);
        ValueToBits(this->srcid,            REQConfig::SrcID_Width,            flit_bits);
        ValueToBits(this->txnid,            REQConfig::TxnID_Width,            flit_bits);
        ValueToBits(this->returnnid,        REQConfig::ReturnNID_Width,        flit_bits);
        ValueToBits(this->stashnidvalid,    REQConfig::StashNID_Width,         flit_bits);
        ValueToBits(this->returntxnid,      REQConfig::ReturnTxnID_Width,      flit_bits);
        ValueToBits(this->opcode,           REQConfig::Opcode_Width,           flit_bits);
        ValueToBits(this->size,             REQConfig::Size_Width,             flit_bits);
        ValueToBits(this->addr,             REQConfig::Addr_Width,             flit_bits);
        ValueToBits(this->ns,               REQConfig::NS_Width,               flit_bits);
        ValueToBits(this->likelyshared,     REQConfig::LikelyShared_Width,     flit_bits);
        ValueToBits(this->allowretry,       REQConfig::AllowRetry_Width,       flit_bits);
        ValueToBits(this->order,            REQConfig::Order_Width,            flit_bits);
        ValueToBits(this->pcrdtype,         REQConfig::PCrdType_Width,         flit_bits);
        ValueToBits(this->memattr,          REQConfig::MemAttr_Width,          flit_bits);
        ValueToBits(this->snpattr,          REQConfig::SnpAttr_Width,          flit_bits);
        ValueToBits(this->pgroupid,         REQConfig::PGroupID_Width,         flit_bits);
        ValueToBits(this->excl,             REQConfig::Excl_Width,             flit_bits);
        ValueToBits(this->expcompack,       REQConfig::ExpCompAck_Width,       flit_bits);
        ValueToBits(this->tagop,            REQConfig::TagOp_Width,            flit_bits);
        ValueToBits(this->tracetag,         REQConfig::TraceTag_Width,         flit_bits);

        uint32_t arr_size = ceil(REQConfig::Total_Width / 32);
        for(uint32_t i = 0; i < arr_size; i++)
        {
            reqflit[i] = BitsToValue(flit_bits, 32);
        }
    }
};


class RSPFlit
{
public:
    uint32_t    qos;
    uint32_t    tgtid;
    uint32_t    srcid;
    uint32_t    txnid;
    uint32_t    opcode;
    uint32_t    resperr;
    uint32_t    resp;
    uint32_t    fwdstate; // FwdState, DataPull
    uint32_t    cbusy;
    uint32_t    dbid; // DBID[11:0], {4’b0,PGroupID[7:0]}, {4’b0,StashGroupID[7:0]}, {4’b0,TagGroupID[7:0]}
    uint32_t    pcrdtype;
    uint32_t    tagop;
    bool        tracetag;


    queue<bool> flit_bits;

    void GetField(uint32_t rspflit[])
    {
        while(!flit_bits.empty()) flit_bits.pop();

        uint32_t arr_size = ceil(RSPConfig::Total_Width / 32);
        for(uint32_t i = 0; i < arr_size; i++)
        {
            ValueToBits(rspflit[i], 32, flit_bits);
        }
        this->qos           = BitsToValue(flit_bits,    RSPConfig::QoS_Width);
        this->tgtid         = BitsToValue(flit_bits,    RSPConfig::TgtID_Width);
        this->srcid         = BitsToValue(flit_bits,    RSPConfig::SrcID_Width);
        this->txnid         = BitsToValue(flit_bits,    RSPConfig::TxnID_Width);
        this->opcode        = BitsToValue(flit_bits,    RSPConfig::Opcode_Width);
        this->resperr       = BitsToValue(flit_bits,    RSPConfig::RespErr_Width);
        this->resp          = BitsToValue(flit_bits,    RSPConfig::Resp_Width);
        this->fwdstate      = BitsToValue(flit_bits,    RSPConfig::FwdState_Width);
        this->cbusy         = BitsToValue(flit_bits,    RSPConfig::CBusy_Width);
        this->dbid          = BitsToValue(flit_bits,    RSPConfig::DBID_Width);
        this->pcrdtype      = BitsToValue(flit_bits,    RSPConfig::PCrdType_Width);
        this->tagop         = BitsToValue(flit_bits,    RSPConfig::TagOp_Width);
        this->tracetag      = BitsToValue(flit_bits,    RSPConfig::TraceTag_Width);
    }


    void GetFlit(uint32_t rspflit[])
    {
        while(!flit_bits.empty()) flit_bits.pop();

        ValueToBits(this->qos,          RSPConfig::QoS_Width,         flit_bits);
        ValueToBits(this->tgtid,        RSPConfig::TgtID_Width,       flit_bits);
        ValueToBits(this->srcid,        RSPConfig::SrcID_Width,       flit_bits);
        ValueToBits(this->txnid,        RSPConfig::TxnID_Width,       flit_bits);
        ValueToBits(this->opcode,       RSPConfig::Opcode_Width,      flit_bits);
        ValueToBits(this->resperr,      RSPConfig::RespErr_Width,     flit_bits);
        ValueToBits(this->resp,         RSPConfig::Resp_Width,        flit_bits);
        ValueToBits(this->fwdstate,     RSPConfig::FwdState_Width,    flit_bits);
        ValueToBits(this->cbusy,        RSPConfig::CBusy_Width,       flit_bits);
        ValueToBits(this->dbid,         RSPConfig::DBID_Width,        flit_bits);
        ValueToBits(this->pcrdtype,     RSPConfig::PCrdType_Width,    flit_bits);
        ValueToBits(this->tagop,        RSPConfig::TagOp_Width,       flit_bits);
        ValueToBits(this->tracetag,     RSPConfig::TraceTag_Width,    flit_bits);

        uint32_t arr_size = ceil(RSPConfig::Total_Width / 32);
        for(uint32_t i = 0; i < arr_size; i++)
        {
            rspflit[i] = BitsToValue(flit_bits, 32);
        }
    }
};


class SNPFlit
{
public:
    uint32_t    qos;
    uint32_t    srcid;
    uint32_t    txnid;
    uint32_t    fwdnid;
    uint32_t    fwdtxnid; // FwdTxnID[11:0], {6’b0,StashLPIDValid, StashLPID[4:0]}, {4’b0,VMIDExt[7:0]}
    uint32_t    opcode;
    uint64_t    addr;
    bool        ns;
    bool        donotgotosd;
    bool        rettosrc;
    bool        tracetag;


    queue<bool> flit_bits;


    void GetField(uint32_t snpflit[])
    {
        while(!flit_bits.empty()) flit_bits.pop();

        uint32_t arr_size = ceil(SNPConfig::Total_Width / 32);
        for(uint32_t i = 0; i < arr_size; i++)
        {
            ValueToBits(snpflit[i], 32, flit_bits);
        }
        this->qos           = BitsToValue(flit_bits,    SNPConfig::QoS_Width);
        this->srcid         = BitsToValue(flit_bits,    SNPConfig::SrcID_Width);
        this->txnid         = BitsToValue(flit_bits,    SNPConfig::TxnID_Width);
        this->fwdnid        = BitsToValue(flit_bits,    SNPConfig::FwdNID_Width);
        this->fwdtxnid      = BitsToValue(flit_bits,    SNPConfig::FwdTxnID_Width);
        this->opcode        = BitsToValue(flit_bits,    SNPConfig::Opcode_Width);
        this->addr          = BitsToValue(flit_bits,    SNPConfig::Addr_Width);
        this->ns            = BitsToValue(flit_bits,    SNPConfig::NS_Width);
        this->donotgotosd   = BitsToValue(flit_bits,    SNPConfig::DoNotGoToSD_Width);
        this->rettosrc      = BitsToValue(flit_bits,    SNPConfig::RetToSrc_Width);
        this->tracetag      = BitsToValue(flit_bits,    SNPConfig::TraceTag_Width);
    }


    void GetFlit(uint32_t snpflit[])
    {
        while(!flit_bits.empty()) flit_bits.pop();

        ValueToBits(this->qos,            SNPConfig::QoS_Width,            flit_bits);
        ValueToBits(this->srcid,          SNPConfig::SrcID_Width,          flit_bits);
        ValueToBits(this->txnid,          SNPConfig::TxnID_Width,          flit_bits);
        ValueToBits(this->fwdnid,         SNPConfig::FwdNID_Width,         flit_bits);
        ValueToBits(this->fwdtxnid,       SNPConfig::FwdTxnID_Width,       flit_bits);
        ValueToBits(this->opcode,         SNPConfig::Opcode_Width,         flit_bits);
        ValueToBits(this->addr,           SNPConfig::Addr_Width,           flit_bits);
        ValueToBits(this->ns,             SNPConfig::NS_Width,             flit_bits);
        ValueToBits(this->donotgotosd,    SNPConfig::DoNotGoToSD_Width,    flit_bits);
        ValueToBits(this->rettosrc,       SNPConfig::RetToSrc_Width,       flit_bits);
        ValueToBits(this->tracetag,       SNPConfig::TraceTag_Width,       flit_bits);

        uint32_t arr_size = ceil(SNPConfig::Total_Width / 32);
        for(uint32_t i = 0; i < arr_size; i++)
        {
            snpflit[i] = BitsToValue(flit_bits, 32);
        }
    }

};


class DATFlit
{
public:
    uint32_t    qos;
    uint32_t    tgtid;
    uint32_t    srcid;
    uint32_t    txnid;
    uint32_t    homenid;
    uint32_t    opcode;
    uint32_t    resperr;
    uint32_t    resp;
    uint32_t    datapull; // {1’b0, FwdState[2:0]}, {1’b0, DataPull[2:0]}, DataSource[3:0]
    uint32_t    cbusy;
    uint32_t    dbid;
    uint32_t    ccid;
    uint32_t    dataid;
    uint32_t    tagop;
    uint32_t    tag;
    uint32_t    tu;
    bool        tracetag;
    uint32_t    be;
    uint32_t    data[(DATConfig::Data_Width / 32)];


    queue<bool> flit_bits;

    void GetField(uint32_t datflit[])
    {
        while(!flit_bits.empty()) flit_bits.pop();

        uint32_t arr_size = ceil(DATConfig::Total_Width / 32);
        for(uint32_t i = 0; i < arr_size; i++)
        {
            ValueToBits(datflit[i], 32, flit_bits);
        }
        this->qos           = BitsToValue(flit_bits,    DATConfig::QoS_Width);
        this->tgtid         = BitsToValue(flit_bits,    DATConfig::TgtID_Width);
        this->srcid         = BitsToValue(flit_bits,    DATConfig::SrcID_Width);
        this->txnid         = BitsToValue(flit_bits,    DATConfig::TxnID_Width);
        this->homenid       = BitsToValue(flit_bits,    DATConfig::HomeNID_Width);
        this->opcode        = BitsToValue(flit_bits,    DATConfig::Opcode_Width);
        this->resperr       = BitsToValue(flit_bits,    DATConfig::RespErr_Width);
        this->resp          = BitsToValue(flit_bits,    DATConfig::Resp_Width);
        this->datapull      = BitsToValue(flit_bits,    DATConfig::DataPull_Width);
        this->cbusy         = BitsToValue(flit_bits,    DATConfig::CBusy_Width);
        this->dbid          = BitsToValue(flit_bits,    DATConfig::DBID_Width);
        this->ccid          = BitsToValue(flit_bits,    DATConfig::CCID_Width);
        this->dataid        = BitsToValue(flit_bits,    DATConfig::DataID_Width);
        this->tagop         = BitsToValue(flit_bits,    DATConfig::TagOp_Width);
        this->tag           = BitsToValue(flit_bits,    DATConfig::Tag_Width);
        this->tu            = BitsToValue(flit_bits,    DATConfig::TU_Width);
        this->tracetag      = BitsToValue(flit_bits,    DATConfig::TraceTag_Width);
        this->be            = BitsToValue(flit_bits,    DATConfig::BE_Width);
        for(uint32_t i = 0; i < (DATConfig::Data_Width / 32); i++)
        {
            this->data[i]   = BitsToValue(flit_bits, 32);
        }
    }


    void GetFlit(uint32_t datflit[])
    {
        while(!flit_bits.empty()) flit_bits.pop();

        ValueToBits(this->qos,         DATConfig::QoS_Width,         flit_bits);
        ValueToBits(this->tgtid,       DATConfig::TgtID_Width,       flit_bits);
        ValueToBits(this->srcid,       DATConfig::SrcID_Width,       flit_bits);
        ValueToBits(this->txnid,       DATConfig::TxnID_Width,       flit_bits);
        ValueToBits(this->homenid,     DATConfig::HomeNID_Width,     flit_bits);
        ValueToBits(this->opcode,      DATConfig::Opcode_Width,      flit_bits);
        ValueToBits(this->resperr,     DATConfig::RespErr_Width,     flit_bits);
        ValueToBits(this->resp,        DATConfig::Resp_Width,        flit_bits);
        ValueToBits(this->datapull,    DATConfig::DataPull_Width,    flit_bits);
        ValueToBits(this->cbusy,       DATConfig::CBusy_Width,       flit_bits);
        ValueToBits(this->dbid,        DATConfig::DBID_Width,        flit_bits);
        ValueToBits(this->ccid,        DATConfig::CCID_Width,        flit_bits);
        ValueToBits(this->dataid,      DATConfig::DataID_Width,      flit_bits);
        ValueToBits(this->tagop,       DATConfig::TagOp_Width,       flit_bits);
        ValueToBits(this->tag,         DATConfig::Tag_Width,         flit_bits);
        ValueToBits(this->tu,          DATConfig::TU_Width,          flit_bits);
        ValueToBits(this->tracetag,    DATConfig::TraceTag_Width,    flit_bits);
        ValueToBits(this->be,          DATConfig::BE_Width,          flit_bits);
        for(uint32_t i = 0; i < (DATConfig::Data_Width / 32); i++)
        {
            ValueToBits(this->data[i], DATConfig::Data_Width, flit_bits);
        }

        uint32_t arr_size = ceil(DATConfig::Total_Width / 32);
        for(uint32_t i = 0; i < arr_size; i++)
        {
            datflit[i] = BitsToValue(flit_bits, 32);
        }
    }
};





namespace REQChannelOpcode
{
    enum
    {
        ReqLCrdReturn            = 0x00,
        ReadShared               = 0x01,
        ReadClean                = 0x02,
        ReadOnce                 = 0x03,
        ReadNoSnp                = 0x04,
        PCrdReturn               = 0x05,
        ReadUnique               = 0x07,
        CleanShared              = 0x08,
        CleanInvalid             = 0x09,
        MakeInvalid              = 0x0A,
        CleanUnique              = 0x0B,
        MakeUnique               = 0x0C,
        Evict                    = 0x0D,

        ReadNoSnpSep             = 0x11,
        CleanSharedPersistSep    = 0x13,
        DVMOp                    = 0x14,
        WriteEvictFull           = 0x15,
        WriteCleanFull           = 0x17,
        WriteUniquePtl           = 0x18,
        WriteUniqueFull          = 0x19,
        WriteBackPtl             = 0x1A,
        WriteBackFull            = 0x1B,
        WriteNoSnpPtl            = 0x1C,
        WriteNoSnpFull           = 0x1D,

        WriteUniqueFullStash     = 0x20,
        WriteUniquePtlStash      = 0x21,
        StashOnceShared          = 0x22,
        StashOnceUnique          = 0x23,
        ReadOnceCleanInvalid     = 0x24,
        ReadOnceMakeInvalid      = 0x25,
        ReadNotSharedDirty       = 0x26,
        CleanSharedPersist       = 0x27,
        AtomicStore_ADD          = 0x28,
        AtomicStore_CLR          = 0x29,
        AtomicStore_EOR          = 0x2A,
        AtomicStore_SET          = 0x2B,
        AtomicStore_SMAX         = 0x2C,
        AtomicStore_SMIN         = 0x2D,
        AtomicStore_UMAX         = 0x2E,
        AtomicStore_UMIN         = 0x2F,

        AtomicLoad_ADD           = 0x30,
        AtomicLoad_CLR           = 0x31,
        AtomicLoad_EOR           = 0x32,
        AtomicLoad_SET           = 0x33,
        AtomicLoad_SMAX          = 0x34,
        AtomicLoad_SMIN          = 0x35,
        AtomicLoad_UMAX          = 0x36,
        AtomicLoad_UMIN          = 0x37,
        AtomicSwap               = 0x38,
        AtomicCompare            = 0x39,
        PrefetchTgt              = 0x3A,

        MakeReadUnique                   = 0x41,
        WriteEvictOrEvict                = 0x42,
        WriteUniqueZero                  = 0x43,
        WriteNoSnpZero                   = 0x44,
        StashOnceSepShared               = 0x47,
        StashOnceSepUnique               = 0x48,
        ReadPreferUnique                 = 0x4C,

        WriteNoSnpFullCleanSh            = 0x50,
        WriteNoSnpFullCleanInv           = 0x51,
        WriteNoSnpFullCleanShPerSep      = 0x52,
        WriteUniqueFullCleanSh           = 0x54,
        WriteBackFullCleanSh             = 0x58,
        WriteBackFullCleanInv            = 0x59,
        WriteBackFullCleanShPerSep       = 0x5A,
        WriteCleanFullCleanSh            = 0x5C,
        WriteCleanFullCleanShPerSep      = 0x5E,

        WriteNoSnpPtlCleanSh             = 0x60,
        WriteNoSnpPtlCleanInv            = 0x61,
        WriteNoSnpPtlCleanShPerSep       = 0x62,
        WriteUniquePtlCleanSh            = 0x64,
        WriteUniquePtlCleanShPerSep      = 0x66,
    };
} // namespace REQChannelOpcode



namespace RSPChannelOpcode
{
    enum
    {
        RespLCrdReturn  =  0x0,
        SnpResp         =  0x1,
        CompAck         =  0x2,
        RetryAck        =  0x3,
        Comp            =  0x4,
        CompDBIDResp    =  0x5,
        DBIDResp        =  0x6,
        PCrdGrant       =  0x7,
        ReadReceipt     =  0x8,
        SnpRespFwded    =  0x9,
        TagMatch        =  0xA,
        RespSepData     =  0xB,
        Persist         =  0xC,
        CompPersist     =  0xD,
        DBIDRespOrd     =  0xE,
        StashDone       =  0x10,
        CompStashDone   =  0x11,
        CompCMO         =  0x14,
    };
} // namespace RSPChannelOpcode



namespace SNPChannelOpcode
{
    enum
    {
        SnpLCrdReturn         =  0x00,
        SnpShared             =  0x01,
        SnpClean              =  0x02,
        SnpOnce               =  0x03,
        SnpNotSharedDirty     =  0x04,
        SnpUniqueStash        =  0x05,
        SnpMakeInvalidStash   =  0x06,
        SnpUnique             =  0x07,
        SnpCleanShared        =  0x08,
        SnpCleanInvalid       =  0x09,
        SnpMakeInvalid        =  0x0A,
        SnpStashUnique        =  0x0B,
        SnpStashShared        =  0x0C,
        SnpDVMOp              =  0x0D,
        SnpQuery              =  0x10,
        SnpSharedFwd          =  0x11,
        SnpCleanFwd           =  0x12,
        SnpOnceFwd            =  0x13,
        SnpNotSharedDirtyFwd  =  0x14,
        SnpPreferUnique       =  0x15,
        SnpPreferUniqueFwd    =  0x16,
        SnpUniqueFwd          =  0x17,
    };
} // namespace SNPChannelOpcode




namespace DATChannelOpcode
{
    enum
    {
        DataLCrdReturn     =  0x0,
        SnpRespData        =  0x1,
        CopyBackWrData     =  0x2,
        NonCopyBackWrData  =  0x3,
        CompData           =  0x4,
        SnpRespDataPtl     =  0x5,
        SnpRespDataFwded   =  0x6,
        WriteDataCancel    =  0x7,
        DataSepResp        =  0xB,
        NCBWrDataCompAck   =  0xC,
    };

} // namespace DATChannelOpcode






namespace DataID
{
    enum
    {
        // Data Width  = 128-bit
        Data_127_to_000 = 0x0,
        Data_255_to_128 = 0x1,
        Data_383_to_256 = 0x2,
        Data_511_to_384 = 0x3,

        Data_255_to_000 = 0x0,
        Data_511_to_256 = 0x2,

        Data_511_to_000 = 0x0, 
    };
} // namespace DataID


namespace CCID
{
    enum
    {
        Data_127_to_000 = 0x0,
        Data_255_to_128 = 0x1,
        Data_383_to_256 = 0x2,
        Data_511_to_384 = 0x3,
    };
} // namespace CCID

