#ifndef CHI_RNF_CHI_FLIT_FIELD_ENCODING_H_
#define CHI_RNF_CHI_FLIT_FIELD_ENCODING_H_

namespace CHI_RNF
{


namespace ReqOpcodeEncoding
{
enum{
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

};

namespace SnpOpcodeEncoding
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

};

namespace RspOpcodeEncoding
{
enum{
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

};

namespace DatOpcodeEncoding
{
enum{
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
};



namespace RespErrEncoding
{
enum{
    OK    = 0b00,
    EXOK  = 0b01,
    DERR  = 0b10,
    NDERR = 0b11,
};
};


namespace RespEncoding
{
    namespace SnoopResp
    {
    enum{
        I       = 0b000,
        SC      = 0b001,
        UC      = 0b010,
        UD      = 0b010,
        SD      = 0b011,
        I_PD    = 0b100,
        SC_PD   = 0b101,
        UC_PD   = 0b110,
    };
    };

    namespace CompResp
    {
    enum{
        I       = 0b000,
        SC      = 0b001,
        UC      = 0b010,
        UD_PD   = 0b110,
        SD_PD   = 0b111,
    };
    };

    namespace WriteDataResp
    {
    enum{
        I       = 0b000,
        SC      = 0b001,
        UC      = 0b010,
        UD_PD   = 0b110,
        SD_PD   = 0b111,
    };
    };

    namespace TagMatchResp
    {
    enum{
        Fail    = 0b000,
        Pass    = 0b001,
    };
    };

};


namespace TagOpEncoding
{
enum{
    Invalid     = 0b00,
    Transfer    = 0b01,
    Update      = 0b10,
    Match       = 0b11,
    Fetch       = 0b11,
};
};



namespace SizeEncoding
{
enum{
    Size_1Byte   = 0b000,
    Size_2Bytes  = 0b001,
    Size_4Bytes  = 0b010,
    Size_8Bytes  = 0b011,
    Size_16Bytes = 0b100,
    Size_32Bytes = 0b101,
    Size_64Bytes = 0b110,
};
};





}
#endif