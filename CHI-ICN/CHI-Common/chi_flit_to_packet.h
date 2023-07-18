#ifndef CHI_HNF_CHI_FLIT_TO_PACKET_H_
#define CHI_HNF_CHI_FLIT_TO_PACKET_H_

#include "chi_flit.h"

namespace CHI_HNF
{


class FlitToPacket
{
private:
    void PushQueue(const uint64_t data, const uint32_t width, queue<bool>& bits_queue)
    {
        for(uint32_t i = 0; i < width; i++)
        {
            bool a_bit = (data >> i) & 0b1;
            bits_queue.push(a_bit);
        }
    }

    void GetPacket(queue<bool>& bits_queue, uint32_t packet[], const uint32_t packet_length)
    {
        for(uint32_t i = 0; i < packet_length; i++)
        {
            uint32_t data = 0;
            for(uint32_t j = 0; j < 32; j++)
            {
                if(bits_queue.empty()) break;
                data = data + ((bits_queue.front() & 0b1) << j);
                bits_queue.pop();
            }
            packet[i] = data;
        }
    }



public:
    void TransformREQ(REQFLIT* src_flit, uint32_t dest_packet[])
    {
        if(src_flit == NULL) assert(0);

        queue<bool> bits_queue;
        
        PushQueue(src_flit->QoS,          Req_QoS_Width,          bits_queue);
        PushQueue(src_flit->TgtID,        Req_TgtID_Width,        bits_queue);
        PushQueue(src_flit->SrcID,        Req_SrcID_Width,        bits_queue);
        PushQueue(src_flit->TxnID,        Req_TxnID_Width,        bits_queue);
        PushQueue(src_flit->CF1,          Req_CF1_Width,          bits_queue);
        PushQueue(src_flit->CF2,          Req_CF2_Width,          bits_queue);
        PushQueue(src_flit->CF3,          Req_CF3_Width,          bits_queue);
        PushQueue(src_flit->Opcode,       Req_Opcode_Width,       bits_queue);
        PushQueue(src_flit->Size,         Req_Size_Width,         bits_queue);
        PushQueue(src_flit->Addr,         Req_Addr_Width,         bits_queue);
        PushQueue(src_flit->NS,           Req_NS_Width,           bits_queue);
        PushQueue(src_flit->LikelyShared, Req_LikelyShared_Width, bits_queue);
        PushQueue(src_flit->AllowRetry,   Req_AllowRetry_Width,   bits_queue);
        PushQueue(src_flit->Order,        Req_Order_Width,        bits_queue);
        PushQueue(src_flit->PCrdType,     Req_PCrdType_Width,     bits_queue);
        PushQueue(src_flit->MemAttr,      Req_MemAttr_Width,      bits_queue);
        PushQueue(src_flit->CF4,          Req_CF4_Width,          bits_queue);
        PushQueue(src_flit->CF5,          Req_CF5_Width,          bits_queue);
        PushQueue(src_flit->CF6,          Req_CF6_Width,          bits_queue);
        PushQueue(src_flit->ExpCompAck,   Req_ExpCompAck_Width,   bits_queue);
        PushQueue(src_flit->TagOp,        Req_TagOp_Width,        bits_queue);
        PushQueue(src_flit->TraceTag,     Req_TraceTag_Width,     bits_queue);

        uint32_t req_packet_length = ceil(Req_Total_Width/32.0);
        GetPacket(bits_queue, dest_packet, req_packet_length);
    }


    void TransformRSP(RSPFLIT *src_flit, uint32_t dest_packet[])
    {
        if(src_flit == NULL) assert(0);

        queue<bool> bits_queue;

        PushQueue(src_flit->QoS,        Rsp_QoS_Width,      bits_queue);
        PushQueue(src_flit->TgtID,      Rsp_TgtID_Width,    bits_queue);
        PushQueue(src_flit->SrcID,      Rsp_SrcID_Width,    bits_queue);
        PushQueue(src_flit->TxnID,      Rsp_TxnID_Width,    bits_queue);
        PushQueue(src_flit->Opcode,     Rsp_Opcode_Width,   bits_queue);
        PushQueue(src_flit->RespErr,    Rsp_RespErr_Width,  bits_queue);
        PushQueue(src_flit->Resp,       Rsp_Resp_Width,     bits_queue);
        PushQueue(src_flit->CF1,        Rsp_CF1_Width,      bits_queue);
        PushQueue(src_flit->CBusy,      Rsp_CBusy_Width,    bits_queue);
        PushQueue(src_flit->CF2,        Rsp_CF2_Width,      bits_queue);
        PushQueue(src_flit->PCrdType,   Rsp_PCrdType_Width, bits_queue);
        PushQueue(src_flit->TagOp,      Rsp_TagOp_Width,    bits_queue);
        PushQueue(src_flit->TraceTag,   Rsp_TraceTag_Width, bits_queue);

        uint32_t rsp_packet_length = ceil(Rsp_Total_Width/32.0);
        GetPacket(bits_queue, dest_packet, rsp_packet_length);
    }


    void TransformDAT(DATFLIT *src_flit, uint32_t dest_packet[])
    {
        if(src_flit == NULL) assert(0);

        queue<bool> bits_queue;

        PushQueue(src_flit->QoS,        Dat_QoS_Width,      bits_queue);
        PushQueue(src_flit->TgtID,      Dat_TgtID_Width,    bits_queue);
        PushQueue(src_flit->SrcID,      Dat_SrcID_Width,    bits_queue);
        PushQueue(src_flit->TxnID,      Dat_TxnID_Width,    bits_queue);
        PushQueue(src_flit->HomeNID,    Dat_HomeNID_Width,  bits_queue);
        PushQueue(src_flit->Opcode,     Dat_Opcode_Width,   bits_queue);
        PushQueue(src_flit->RespErr,    Dat_RespErr_Width,  bits_queue);
        PushQueue(src_flit->Resp,       Dat_Resp_Width,     bits_queue);
        PushQueue(src_flit->CF1,        Dat_CF1_Width,      bits_queue);
        PushQueue(src_flit->CBusy,      Dat_CBusy_Width,    bits_queue);
        PushQueue(src_flit->DBID,       Dat_DBID_Width,     bits_queue);
        PushQueue(src_flit->CCID,       Dat_CCID_Width,     bits_queue);
        PushQueue(src_flit->DataID,     Dat_DataID_Width,   bits_queue);
        PushQueue(src_flit->TagOp,      Dat_TagOp_Width,    bits_queue);
        PushQueue(src_flit->Tag,        Dat_Tag_Width,      bits_queue);
        PushQueue(src_flit->TU,         Dat_TU_Width,       bits_queue);
        PushQueue(src_flit->TraceTag,   Dat_TraceTag_Width, bits_queue);
        PushQueue(src_flit->BE,         Dat_BE_Width,       bits_queue);
        for(uint32_t i = 0; i < (Dat_Data_Width/32); i++)
            PushQueue(src_flit->Data[i], 32, bits_queue);

        uint32_t dat_packet_length = ceil(Dat_Total_Width/32.0);
        GetPacket(bits_queue, dest_packet, dat_packet_length);
    }


    void TransformSNP(SNPFLIT *src_flit, uint32_t dest_packet[])
    {
        if(src_flit == NULL) assert(0);

        queue<bool> bits_queue;

        PushQueue(src_flit->QoS,            Snp_QoS_Width,          bits_queue);
        PushQueue(src_flit->SrcID,          Snp_SrcID_Width,        bits_queue);
        PushQueue(src_flit->TxnID,          Snp_TxnID_Width,        bits_queue);
        PushQueue(src_flit->FwdNID,         Snp_FwdNID_Width,       bits_queue);
        PushQueue(src_flit->CF1,            Snp_CF1_Width,          bits_queue);
        PushQueue(src_flit->Opcode,         Snp_Opcode_Width,       bits_queue);
        PushQueue(src_flit->Addr,           Snp_Addr_Width,         bits_queue);
        PushQueue(src_flit->NS,             Snp_NS_Width,           bits_queue);
        PushQueue(src_flit->DoNotGoToSD,    Snp_DoNotGoToSD_Width,  bits_queue);
        PushQueue(src_flit->RetToSrc,       Snp_RetToSrc_Width,     bits_queue);
        PushQueue(src_flit->TraceTag,       Snp_TraceTag_Width,     bits_queue);

        uint32_t snp_packet_length = ceil(Snp_Total_Width/32.0);
        GetPacket(bits_queue, dest_packet, snp_packet_length);
    }


};




}
#endif