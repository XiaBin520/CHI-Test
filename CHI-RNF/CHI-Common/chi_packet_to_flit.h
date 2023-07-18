#ifndef CHI_RNF_CHI_PACKET_TO_FLIT_H_
#define CHI_RNF_CHI_PACKET_TO_FLIT_H_


#include "chi_flit.h"


namespace CHI_RNF
{


class PacketToFlit
{
private:
    void PushQueue(uint32_t packet[], const uint32_t packet_length, queue<bool>& bits_queue)
    {
        if(!bits_queue.empty()) assert(0);

        for(uint32_t i = 0; i < packet_length; i++)
        {
            for(uint32_t j = 0; j < 32; j++)
            {
                bool a_bit = (packet[i] >> j) & 0b1;
                bits_queue.push(a_bit);
            }
        }
    }

    uint64_t GetField(queue<bool>& bits_queue, const uint32_t field_width)
    {
        if(bits_queue.size() < field_width) assert(0);

        uint64_t value = 0;
        for(uint32_t i = 0; i < field_width; i++)
        {
            bool a_bit = bits_queue.front() & 0b1;
            bits_queue.pop();
            value = value + (a_bit << i);
        }
        return value;
    }

public:
    void TransformREQ(uint32_t src_packet[], REQFLIT *dest_flit)
    {
        if(dest_flit == NULL) assert(0);

        queue<bool> bits_queue;
        uint32_t req_packet_length = ceil(Req_Total_Width/32.0);
        PushQueue(src_packet, req_packet_length, bits_queue);

        dest_flit->QoS           = GetField(bits_queue, Req_QoS_Width);
        dest_flit->TgtID         = GetField(bits_queue, Req_TgtID_Width);
        dest_flit->SrcID         = GetField(bits_queue, Req_SrcID_Width);
        dest_flit->TxnID         = GetField(bits_queue, Req_TxnID_Width);
        dest_flit->CF1           = GetField(bits_queue, Req_CF1_Width);
        dest_flit->CF2           = GetField(bits_queue, Req_CF2_Width);
        dest_flit->CF3           = GetField(bits_queue, Req_CF3_Width);
        dest_flit->Opcode        = GetField(bits_queue, Req_Opcode_Width);
        dest_flit->Size          = GetField(bits_queue, Req_Size_Width);
        dest_flit->Addr          = GetField(bits_queue, Req_Addr_Width);
        dest_flit->NS            = GetField(bits_queue, Req_NS_Width);
        dest_flit->LikelyShared  = GetField(bits_queue, Req_LikelyShared_Width);
        dest_flit->AllowRetry    = GetField(bits_queue, Req_AllowRetry_Width);
        dest_flit->Order         = GetField(bits_queue, Req_Order_Width);
        dest_flit->PCrdType      = GetField(bits_queue, Req_PCrdType_Width);
        dest_flit->MemAttr       = GetField(bits_queue, Req_MemAttr_Width);
        dest_flit->CF4           = GetField(bits_queue, Req_CF4_Width);
        dest_flit->CF5           = GetField(bits_queue, Req_CF5_Width);
        dest_flit->CF6           = GetField(bits_queue, Req_CF6_Width);
        dest_flit->ExpCompAck    = GetField(bits_queue, Req_ExpCompAck_Width);
        dest_flit->TagOp         = GetField(bits_queue, Req_TagOp_Width);
        dest_flit->TraceTag      = GetField(bits_queue, Req_TraceTag_Width);
    }


    void TransformRSP(uint32_t src_packet[], RSPFLIT *dest_flit)
    {
        if(dest_flit == NULL) assert(0);

        queue<bool> bits_queue;
        uint32_t rsp_packet_length = ceil(Rsp_Total_Width/32.0);
        PushQueue(src_packet, rsp_packet_length, bits_queue);

        dest_flit->QoS      = GetField(bits_queue, Rsp_QoS_Width);
        dest_flit->TgtID    = GetField(bits_queue, Rsp_TgtID_Width);
        dest_flit->SrcID    = GetField(bits_queue, Rsp_SrcID_Width);
        dest_flit->TxnID    = GetField(bits_queue, Rsp_TxnID_Width);
        dest_flit->Opcode   = GetField(bits_queue, Rsp_Opcode_Width);
        dest_flit->RespErr  = GetField(bits_queue, Rsp_RespErr_Width);
        dest_flit->Resp     = GetField(bits_queue, Rsp_Resp_Width);
        dest_flit->CF1      = GetField(bits_queue, Rsp_CF1_Width);
        dest_flit->CBusy    = GetField(bits_queue, Rsp_CBusy_Width);
        dest_flit->CF2      = GetField(bits_queue, Rsp_CF2_Width);
        dest_flit->PCrdType = GetField(bits_queue, Rsp_PCrdType_Width);
        dest_flit->TagOp    = GetField(bits_queue, Rsp_TagOp_Width);
        dest_flit->TraceTag = GetField(bits_queue, Rsp_TraceTag_Width);
    }


    void TransformDAT(uint32_t src_packet[], DATFLIT *dest_flit)
    {
        if(dest_flit == NULL) assert(0);

        queue<bool> bits_queue;
        uint32_t dat_packet_length = ceil(Dat_Total_Width/32.0);
        PushQueue(src_packet,  dat_packet_length, bits_queue);

        dest_flit->QoS       = GetField(bits_queue, Dat_QoS_Width);
        dest_flit->TgtID     = GetField(bits_queue, Dat_TgtID_Width);
        dest_flit->SrcID     = GetField(bits_queue, Dat_SrcID_Width);
        dest_flit->TxnID     = GetField(bits_queue, Dat_TxnID_Width);
        dest_flit->HomeNID   = GetField(bits_queue, Dat_HomeNID_Width);
        dest_flit->Opcode    = GetField(bits_queue, Dat_Opcode_Width);
        dest_flit->RespErr   = GetField(bits_queue, Dat_RespErr_Width);
        dest_flit->Resp      = GetField(bits_queue, Dat_Resp_Width);
        dest_flit->CF1       = GetField(bits_queue, Dat_CF1_Width);
        dest_flit->CBusy     = GetField(bits_queue, Dat_CBusy_Width);
        dest_flit->DBID      = GetField(bits_queue, Dat_DBID_Width);
        dest_flit->CCID      = GetField(bits_queue, Dat_CCID_Width);
        dest_flit->DataID    = GetField(bits_queue, Dat_DataID_Width);
        dest_flit->TagOp     = GetField(bits_queue, Dat_TagOp_Width);
        dest_flit->Tag       = GetField(bits_queue, Dat_Tag_Width);
        dest_flit->TU        = GetField(bits_queue, Dat_TU_Width);
        dest_flit->TraceTag  = GetField(bits_queue, Dat_TraceTag_Width);
        dest_flit->BE        = GetField(bits_queue, Dat_BE_Width);
        for(uint32_t i = 0; i < (Dat_Data_Width/32); i++)
          dest_flit->Data[i] = GetField(bits_queue, 32);
    }



    void TransformSNP(uint32_t src_packet[], SNPFLIT *snp_flit)
    {
        if(snp_flit == NULL) assert(0);

        queue<bool> bits_queue;
        uint32_t snp_packet_length = ceil(Snp_Total_Width/32.0);
        PushQueue(src_packet, snp_packet_length, bits_queue);

        snp_flit->QoS           = GetField(bits_queue, Snp_QoS_Width);
        snp_flit->SrcID         = GetField(bits_queue, Snp_SrcID_Width);
        snp_flit->TxnID         = GetField(bits_queue, Snp_TxnID_Width);
        snp_flit->FwdNID        = GetField(bits_queue, Snp_FwdNID_Width);
        snp_flit->CF1           = GetField(bits_queue, Snp_CF1_Width);
        snp_flit->Opcode        = GetField(bits_queue, Snp_Opcode_Width);
        snp_flit->Addr          = GetField(bits_queue, Snp_Addr_Width);
        snp_flit->NS            = GetField(bits_queue, Snp_NS_Width);
        snp_flit->DoNotGoToSD   = GetField(bits_queue, Snp_DoNotGoToSD_Width);
        snp_flit->RetToSrc      = GetField(bits_queue, Snp_RetToSrc_Width);
        snp_flit->TraceTag      = GetField(bits_queue, Snp_TraceTag_Width);
    }


};




}
#endif
