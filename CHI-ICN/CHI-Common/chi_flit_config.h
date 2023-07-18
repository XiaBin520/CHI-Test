#ifndef CHI_HNF_CHI_FLIT_CONFIG_H_
#define CHI_HNF_CHI_FLIT_CONFIG_H_


namespace CHI_HNF
{


#define Req_Addr_Width      44
#define NodeID_Width        11
#define Data_Width          128



// Fixed Config
#define Atomic_Transactions             false
#define Cache_Stash_Transactions        false
#define Direct_Memory_Transfer          false
#define Direct_Cache_Transfer           false
#define Data_Poison                     false
#define Data_Check                      false
#define Check_Type                      false
#define CleanSharedPersistSep_Request   false
#define MPAM_Support                    false
#define CCF_Wrap_Order                  true
#define Enhanced_Features               false


//  Req Channel Width
#define Req_QoS_Width               4
#define Req_TgtID_Width             NodeID_Width
#define Req_SrcID_Width             NodeID_Width
#define Req_TxnID_Width             12
#define Req_CF1_Width               NodeID_Width // CF1: ReturnNID, StashNID, {(NodeID_Width-7)'b0, SLCRepHint[6:0]}
#define Req_CF2_Width               1 // CF2: StashNIDValid, Endian, Deep
#define Req_CF3_Width               12 // CF3: ReturnTxnID[11:0], {6'b0, StashLPIDValid, StashLPID[4:0]}
#define Req_Opcode_Width            7
#define Req_Size_Width              3
#define Req_NS_Width                1
#define Req_LikelyShared_Width      1
#define Req_AllowRetry_Width        1
#define Req_Order_Width             2
#define Req_PCrdType_Width          4
#define Req_MemAttr_Width           4
#define Req_CF4_Width               1 // CF4: SnpAttr, DoDWT
#define Req_CF5_Width               8 // CF5: {GroupIDExt[2:0], LPID[4:0]}, PGroupID[7:0], StashGroupID[7:0], TagGroupID[7:0]
#define Req_CF6_Width               1 // CF6: Excl, SnoopMe
#define Req_ExpCompAck_Width        1
#define Req_TagOp_Width             2
#define Req_TraceTag_Width          1
#define Req_Total_Width             (Req_QoS_Width + Req_TgtID_Width + Req_SrcID_Width + Req_TxnID_Width \
                                        + Req_CF1_Width + Req_CF2_Width+ Req_CF3_Width \
                                        + Req_Opcode_Width + Req_Size_Width + Req_NS_Width \
                                        + Req_LikelyShared_Width + Req_AllowRetry_Width \
                                        + Req_Order_Width + Req_PCrdType_Width + Req_MemAttr_Width \
                                        + Req_CF4_Width + Req_CF5_Width + Req_CF6_Width \
                                        + Req_ExpCompAck_Width + Req_TagOp_Width + Req_TraceTag_Width \
                                        + Req_Addr_Width)


//  Snp Channel Width
#define Snp_QoS_Width               4
#define Snp_SrcID_Width             NodeID_Width
#define Snp_TxnID_Width             12
#define Snp_FwdNID_Width            NodeID_Width
#define Snp_CF1_Width               12 // CF1: FwdTxnId[11:0], {6'b0, StashLPIDValid, StashLPID[4:0]}, {4'b0, VMIDExt[7:0]}
#define Snp_Opcode_Width            5
#define Snp_Addr_Width              (Req_Addr_Width - 3)
#define Snp_NS_Width                1
#define Snp_DoNotGoToSD_Width       1
#define Snp_RetToSrc_Width          1
#define Snp_TraceTag_Width          1
#define Snp_Total_Width             (Snp_QoS_Width + Snp_SrcID_Width + Snp_TxnID_Width \
                                        + Snp_FwdNID_Width + Snp_CF1_Width \
                                        + Snp_Opcode_Width + Snp_Addr_Width + Snp_NS_Width \
                                        + Snp_DoNotGoToSD_Width + Snp_RetToSrc_Width + Snp_TraceTag_Width)


//  Rsp Channel Width
#define Rsp_QoS_Width               4
#define Rsp_TgtID_Width             NodeID_Width
#define Rsp_SrcID_Width             NodeID_Width
#define Rsp_TxnID_Width             12
#define Rsp_Opcode_Width            5
#define Rsp_RespErr_Width           2
#define Rsp_Resp_Width              3
#define Rsp_CF1_Width               3 // CF1: FdwState[2:0], DataPull[2:0]
#define Rsp_CBusy_Width             3
#define Rsp_CF2_Width               12 // CF2: DBID[11:0], {4'b0, PGroupID[7:0]}, {4'b0, StashGroupID[7:0]}, {4'b0, TagGroupID[7:0]}
#define Rsp_PCrdType_Width          4
#define Rsp_TagOp_Width             2
#define Rsp_TraceTag_Width          1
#define Rsp_Total_Width             (Rsp_QoS_Width + Rsp_TgtID_Width + Rsp_SrcID_Width \
                                        + Rsp_TxnID_Width + Rsp_Opcode_Width + Rsp_RespErr_Width \
                                        + Rsp_Resp_Width + Rsp_CF1_Width + Rsp_CBusy_Width \
                                        + Rsp_CF2_Width + Rsp_PCrdType_Width + Rsp_TagOp_Width \
                                        + Rsp_TraceTag_Width)



//  Dat Channel Width
#define Dat_QoS_Width               4
#define Dat_TgtID_Width             NodeID_Width
#define Dat_SrcID_Width             NodeID_Width
#define Dat_TxnID_Width             12
#define Dat_HomeNID_Width           NodeID_Width
#define Dat_Opcode_Width            4
#define Dat_RespErr_Width           2
#define Dat_Resp_Width              3
#define Dat_CF1_Width               4 // CF1: {1'b0, FwdState[2:0]}, {1'b0, DataPull[2:0]}, DataSource[3:0]
#define Dat_CBusy_Width             3
#define Dat_DBID_Width              12
#define Dat_CCID_Width              2
#define Dat_DataID_Width            2
#define Dat_TagOp_Width             2
#define Dat_Tag_Width               (Data_Width/32)
#define Dat_TU_Width                (Data_Width/128)
#define Dat_TraceTag_Width          1
#define Dat_BE_Width                (Data_Width/8)
#define Dat_Data_Width              Data_Width
#define Dat_Total_Width             (Dat_QoS_Width + Dat_TgtID_Width + Dat_SrcID_Width \
                                        + Dat_TxnID_Width + Dat_HomeNID_Width + Dat_Opcode_Width \
                                        + Dat_RespErr_Width + Dat_Resp_Width + Dat_CF1_Width \
                                        + Dat_CBusy_Width + Dat_DBID_Width + Dat_CCID_Width \
                                        + Dat_DataID_Width + Dat_TagOp_Width + Dat_Tag_Width \
                                        + Dat_TU_Width + Dat_TraceTag_Width + Dat_BE_Width \
                                        + Dat_Data_Width)


}
#endif