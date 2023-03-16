
namespace REQConfig
{
    enum
    {
        QoS_Width           = 4,
        TgtID_Width         = 11,
        SrcID_Width         = 11,
        TxnID_Width         = 12,

        ReturnNID_Width     = 11,
        StashNID_Width      = 1,
        ReturnTxnID_Width   = 12,

        Opcode_Width        = 7,
        Size_Width          = 3,
        Addr_Width          = 44,

        NS_Width            = 1,
        LikelyShared_Width  = 1,
        AllowRetry_Width    = 1,
        Order_Width         = 1,
        PCrdType_Width      = 4,
        MemAttr_Width       = 4,
        SnpAttr_Width       = 1,

        PGroupID_Width      = 8,
        Excl_Width          = 1,
        ExpCompAck_Width    = 1,
        TagOp_Width         = 2,
        TraceTag_Width      = 1,

        Total_Width = 
            QoS_Width +
            TgtID_Width +
            SrcID_Width +
            TxnID_Width +

            ReturnNID_Width +
            StashNID_Width +
            ReturnTxnID_Width +

            Opcode_Width +
            Size_Width +
            Addr_Width +

            NS_Width +
            LikelyShared_Width +
            AllowRetry_Width +
            Order_Width +
            PCrdType_Width +
            MemAttr_Width +
            SnpAttr_Width +

            PGroupID_Width +
            Excl_Width +
            ExpCompAck_Width +
            TagOp_Width +
            TraceTag_Width,
    };
}


namespace RSPConfig
{
    enum
    {
        QoS_Width           = 4,
        TgtID_Width         = 11,
        SrcID_Width         = 11,
        TxnID_Width         = 12,

        Opcode_Width        = 7,
        RespErr_Width       = 2,
        Resp_Width          = 3,
        FwdState_Width      = 3,
        CBusy_Width         = 3,

        DBID_Width          = 12,
        PCrdType_Width      = 4,
        TagOp_Width         = 2,
        TraceTag_Width      = 1,

        Total_Width = 
            QoS_Width +
            TgtID_Width +
            SrcID_Width +
            TxnID_Width +

            Opcode_Width +
            RespErr_Width +
            Resp_Width +
            FwdState_Width +
            CBusy_Width +

            DBID_Width +
            PCrdType_Width +
            TagOp_Width +
            TraceTag_Width,
    };
}


namespace SNPConfig
{
    enum
    {
        QoS_Width           = 4,
        SrcID_Width         = 11,
        TxnID_Width         = 12,
        FwdNID_Width        = 11,
        FwdTxnID_Width      = 12,
        Opcode_Width        = 5,
        Addr_Width          = 41,
        NS_Width            = 1,
        DoNotGoToSD_Width   = 1,
        RetToSrc_Width      = 1,
        TraceTag_Width      = 1,

        Total_Width = 
            QoS_Width +
            SrcID_Width +
            TxnID_Width +
            FwdNID_Width +
            FwdTxnID_Width +
            Opcode_Width +
            Addr_Width +
            NS_Width +
            DoNotGoToSD_Width +
            RetToSrc_Width +
            TraceTag_Width,
    };
}


