#ifndef CHI_RNF_CHI_FLIT_H_
#define CHI_RNF_CHI_FLIT_H_

#include "../common.h"
#include "config.h"
#include "chi_flit_field_encoding.h"



namespace CHI_RNF
{


class REQFLIT
{
public:
    uint32_t    QoS;
    uint32_t    TgtID;
    uint32_t    SrcID;
    uint32_t    TxnID;
    uint32_t    CF1;
        uint32_t    CF1_ReturnNID;
        uint32_t    CF1_StashNID;
        uint32_t    CF1_SLCRepHint;
    bool        CF2;
        bool        CF2_StashNIDValid;
        bool        CF2_Endian;
        bool        CF2_Deep;
    uint32_t    CF3;
        uint32_t    CF3_ReturnTxnID;
        bool        CF3_StashLPIDValid;
        uint32_t    CF3_StashLPID;
    uint32_t    Opcode;
    uint32_t    Size;
    uint64_t    Addr;
    bool        NS;
    bool        LikelyShared;
    bool        AllowRetry;
    uint32_t    Order;
    uint32_t    PCrdType;
    uint32_t    MemAttr;
        bool        MemAttr_Allocate;
        bool        MemAttr_Cacheable;
        bool        MemAttr_Device;
        bool        MemAttr_EWA;
    bool        CF4;
        bool        CF4_SnpAttr;
        bool        CF4_DoDWT;
    uint32_t    CF5;
        uint32_t    CF5_GroupIDExt;
        uint32_t    CF5_LPID;
        uint32_t    CF5_PGroupID;
        uint32_t    CF5_StashGroupID;
        uint32_t    CF5_TagGroupID;
    bool        CF6;
        bool        CF6_Excl;
        bool        CF6_SnoopMe;
    bool        ExpCompAck;
    uint32_t    TagOp;
    bool        TraceTag;
};


class RSPFLIT
{
public:
    uint32_t    QoS;
    uint32_t    TgtID;
    uint32_t    SrcID;
    uint32_t    TxnID;
    uint32_t    Opcode;
    uint32_t    RespErr;
    uint32_t    Resp;
    uint32_t    CF1;
        uint32_t    CF1_FwdState;
        uint32_t    CF1_DataPull;
    uint32_t    CBusy;
    uint32_t    CF2;
        uint32_t    CF2_DBID;
        uint32_t    CF2_PGroupID;
        uint32_t    CF2_StashGroupID;
        uint32_t    CF2_TagGroupID;
    uint32_t    PCrdType;
    uint32_t    TagOp;
    bool        TraceTag;
};


class DATFLIT
{
public:
    uint32_t    QoS;
    uint32_t    TgtID;
    uint32_t    SrcID;
    uint32_t    TxnID;
    uint32_t    HomeNID;
    uint32_t    Opcode;
    uint32_t    RespErr;
    uint32_t    Resp;
    uint32_t    CF1;
        uint32_t    CF1_FwdState;
        uint32_t    CF1_DataPull;
        uint32_t    CF1_DataSource;
    uint32_t    CBusy;
    uint32_t    DBID;
    uint32_t    CCID;
    uint32_t    DataID;
    uint32_t    TagOp;
    uint32_t    Tag;
    uint32_t    TU;
    bool        TraceTag;
    uint64_t    BE;
    uint32_t    Data[Data_Width/32];
};




class SNPFLIT
{
public:
    uint32_t    QoS;
    uint32_t    SrcID;
    uint32_t    TxnID;
    uint32_t    FwdNID;
    uint32_t    CF1;
        uint32_t    CF1_FwdTxnID;
        bool        CF1_StashLPIDValid;
        uint32_t    CF1_StashLPID;
        uint32_t    CF1_VMIDExt;
    uint32_t    Opcode;
    uint64_t    Addr;
    bool        NS;
    bool        DoNotGoToSD;
    bool        RetToSrc;
    bool        TraceTag;
};



}
#endif