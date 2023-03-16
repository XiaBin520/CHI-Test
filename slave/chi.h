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

