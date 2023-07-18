#ifndef CHI_RNF_IACCHANDLE_H_
#define CHI_RNF_IACCHANDLE_H_

#include "../../CHI-Common/chi_flit.h"
#include "../../ACC-Common/acc_flit.h"
#include "../../cache.h"

namespace CHI_RNF
{


class IAccHandle
{
protected:
    CacheLine *cache_line;
    ACCFLIT *accreq_flit;
protected:
    uint32_t age;


protected:
    void SetCHITxREQ_ReadClean(REQFLIT *txreq_flit);
    void SetCHITxREQ_ReadShared(REQFLIT *txreq_flit);
    void SetCHITxREQ_ReadUnique(REQFLIT *txreq_flit);
protected:
    void SetCHITxREQ_CleanUnique(REQFLIT *txreq_flit);
    void SetCHITxREQ_MakeUnique(REQFLIT *txreq_flit);
    void SetCHITxREQ_Evict(REQFLIT *txreq_flit);
protected:
    void SetCHITxREQ_WriteUniquePtl(REQFLIT *txreq_flit);
    void SetCHITxREQ_WriteUniqueFull(REQFLIT *txreq_flit);
protected:
    void SetCHITxREQ_WriteBackFull(REQFLIT *txreq_flit);
    void SetCHITxREQ_WriteCleanFull(REQFLIT *txreq_flit);
    void SetCHITxREQ_WriteEvictFull(REQFLIT *txreq_flit);



public:
    virtual void DriverTxREQ(queue<REQFLIT*>& txreq_flit_queue, queue<ACCFLIT*>& accreq_flit_queue) {return;}
    virtual void DriverAccRSP(queue<ACCFLIT*>& accrsp_flit_queue) {return;}
    virtual bool IsDone() {return false;}
};







/******************************************************/
/*  Set CHI TxREQ: Read Transaction                   */
/******************************************************/
void IAccHandle::SetCHITxREQ_ReadClean(REQFLIT *txreq_flit)
{
    txreq_flit->QoS                = 0;
    txreq_flit->TgtID              = 456; // TODO
    txreq_flit->SrcID              = 123; // TODO
    txreq_flit->TxnID              = 0;
    txreq_flit->CF1                = 0;
    txreq_flit->CF2                = 0;
    txreq_flit->CF3                = 0;
    txreq_flit->Opcode             = ReqOpcodeEncoding::ReadClean;
    txreq_flit->Size               = SizeEncoding::Size_64Bytes;
    txreq_flit->Addr               = accreq_flit->addr;
    txreq_flit->NS                 = 0;
    txreq_flit->LikelyShared       = 0;
    txreq_flit->AllowRetry         = 1;
    txreq_flit->Order              = 0;
    txreq_flit->PCrdType           = 0;
    txreq_flit->MemAttr            = 0b0101;
    txreq_flit->CF4                = 1;
    txreq_flit->CF5                = 0;
    txreq_flit->CF6                = 0;
    txreq_flit->ExpCompAck         = 1;
    txreq_flit->TagOp              = 0;
    txreq_flit->TraceTag           = 0;
}


void IAccHandle::SetCHITxREQ_ReadShared(REQFLIT *txreq_flit)
{
    txreq_flit->QoS                = 0;
    txreq_flit->TgtID              = 456; // TODO
    txreq_flit->SrcID              = 123; // TODO
    txreq_flit->TxnID              = 0;
    txreq_flit->CF1                = 0;
    txreq_flit->CF2                = 0;
    txreq_flit->CF3                = 0;
    txreq_flit->Opcode             = ReqOpcodeEncoding::ReadShared;
    txreq_flit->Size               = SizeEncoding::Size_64Bytes;
    txreq_flit->Addr               = accreq_flit->addr;
    txreq_flit->NS                 = 0;
    txreq_flit->LikelyShared       = 0;
    txreq_flit->AllowRetry         = 1;
    txreq_flit->Order              = 0;
    txreq_flit->PCrdType           = 0;
    txreq_flit->MemAttr            = 0b0101;
    txreq_flit->CF4                = 1;
    txreq_flit->CF5                = 0;
    txreq_flit->CF6                = 0;
    txreq_flit->ExpCompAck         = 1;
    txreq_flit->TagOp              = 0;
    txreq_flit->TraceTag           = 0;
}


void IAccHandle::SetCHITxREQ_ReadUnique(REQFLIT *txreq_flit)
{
    txreq_flit->QoS                = 0;
    txreq_flit->TgtID              = 456; // TODO
    txreq_flit->SrcID              = 123; // TODO
    txreq_flit->TxnID              = 0;
    txreq_flit->CF1                = 0;
    txreq_flit->CF2                = 0;
    txreq_flit->CF3                = 0;
    txreq_flit->Opcode             = ReqOpcodeEncoding::ReadUnique;
    txreq_flit->Size               = SizeEncoding::Size_64Bytes;
    txreq_flit->Addr               = accreq_flit->addr;
    txreq_flit->NS                 = 0;
    txreq_flit->LikelyShared       = 0;
    txreq_flit->AllowRetry         = 1;
    txreq_flit->Order              = 0;
    txreq_flit->PCrdType           = 0;
    txreq_flit->MemAttr            = 0b0101;
    txreq_flit->CF4                = 1;
    txreq_flit->CF5                = 0;
    txreq_flit->CF6                = 0;
    txreq_flit->ExpCompAck         = 1;
    txreq_flit->TagOp              = 0;
    txreq_flit->TraceTag           = 0;
}





/******************************************************/
/*  Set CHI TxREQ: Dataless Transaction               */
/******************************************************/
void IAccHandle::SetCHITxREQ_CleanUnique(REQFLIT *txreq_flit)
{
    txreq_flit->QoS                = 0;
    txreq_flit->TgtID              = 456; // TODO
    txreq_flit->SrcID              = 123; // TODO
    txreq_flit->TxnID              = 0;
    txreq_flit->CF1                = 0;
    txreq_flit->CF2                = 0;
    txreq_flit->CF3                = 0;
    txreq_flit->Opcode             = ReqOpcodeEncoding::CleanUnique;
    txreq_flit->Size               = SizeEncoding::Size_64Bytes;
    txreq_flit->Addr               = accreq_flit->addr;
    txreq_flit->NS                 = 0;
    txreq_flit->LikelyShared       = 0;
    txreq_flit->AllowRetry         = 1;
    txreq_flit->Order              = 0;
    txreq_flit->PCrdType           = 0;
    txreq_flit->MemAttr            = 0b0101;
    txreq_flit->CF4                = 1;
    txreq_flit->CF5                = 0;
    txreq_flit->CF6                = 0;
    txreq_flit->ExpCompAck         = 1;
    txreq_flit->TagOp              = 0;
    txreq_flit->TraceTag           = 0;
}


void IAccHandle::SetCHITxREQ_MakeUnique(REQFLIT *txreq_flit)
{
    txreq_flit->QoS                = 0;
    txreq_flit->TgtID              = 456; // TODO
    txreq_flit->SrcID              = 123; // TODO
    txreq_flit->TxnID              = 0;
    txreq_flit->CF1                = 0;
    txreq_flit->CF2                = 0;
    txreq_flit->CF3                = 0;
    txreq_flit->Opcode             = ReqOpcodeEncoding::MakeUnique;
    txreq_flit->Size               = SizeEncoding::Size_64Bytes;
    txreq_flit->Addr               = accreq_flit->addr;
    txreq_flit->NS                 = 0;
    txreq_flit->LikelyShared       = 0;
    txreq_flit->AllowRetry         = 1;
    txreq_flit->Order              = 0;
    txreq_flit->PCrdType           = 0;
    txreq_flit->MemAttr            = 0b0101;
    txreq_flit->CF4                = 1;
    txreq_flit->CF5                = 0;
    txreq_flit->CF6                = 0;
    txreq_flit->ExpCompAck         = 1;
    txreq_flit->TagOp              = 0;
    txreq_flit->TraceTag           = 0;
}


void IAccHandle::SetCHITxREQ_Evict(REQFLIT *txreq_flit)
{
    txreq_flit->QoS                = 0;
    txreq_flit->TgtID              = 456; // TODO
    txreq_flit->SrcID              = 123; // TODO
    txreq_flit->TxnID              = 0;
    txreq_flit->CF1                = 0;
    txreq_flit->CF2                = 0;
    txreq_flit->CF3                = 0;
    txreq_flit->Opcode             = ReqOpcodeEncoding::Evict;
    txreq_flit->Size               = SizeEncoding::Size_64Bytes;
    txreq_flit->Addr               = accreq_flit->addr;
    txreq_flit->NS                 = 0;
    txreq_flit->LikelyShared       = 0;
    txreq_flit->AllowRetry         = 1;
    txreq_flit->Order              = 0;
    txreq_flit->PCrdType           = 0;
    txreq_flit->MemAttr            = 0b0101;
    txreq_flit->CF4                = 1;
    txreq_flit->CF5                = 0;
    txreq_flit->CF6                = 0;
    txreq_flit->ExpCompAck         = 0;
    txreq_flit->TagOp              = 0;
    txreq_flit->TraceTag           = 0;
}






/******************************************************/
/*  Set CHI TxREQ: Non-CopyBack Write Data Transaction*/
/******************************************************/
void IAccHandle::SetCHITxREQ_WriteUniquePtl(REQFLIT *txreq_flit)
{
    uint64_t write_enables = accreq_flit->write_enable;
    
    uint32_t mask_16B = 0xffff;
    uint32_t we0_16B = (write_enables >> (16*0)) & mask_16B;
    uint32_t we1_16B = (write_enables >> (16*1)) & mask_16B;
    uint32_t we2_16B = (write_enables >> (16*2)) & mask_16B;
    uint32_t we3_16B = (write_enables >> (16*3)) & mask_16B;

    uint32_t low_ccid = (we0_16B != 0) ? 0 :
                        (we1_16B != 0) ? 1 :
                        (we2_16B != 0) ? 2 :
                        (we3_16B != 0) ? 3 :
                                         0;
    uint32_t high_ccid = (we3_16B != 0) ? 3 :
                         (we2_16B != 0) ? 2 :
                         (we1_16B != 0) ? 1 :
                         (we0_16B != 0) ? 0 :
                                          0;
    uint32_t addr_ccid = (accreq_flit->addr >> 4) & 0b11;

    uint32_t size = -1;
    //  三者位于同一个 16B
    if((addr_ccid == high_ccid) & (addr_ccid == low_ccid))
    {
        size = SizeEncoding::Size_16Bytes;
    }
    // 三者位于低 32B
    else if((addr_ccid <= 1) & (low_ccid <= 1) & (high_ccid <= 1))
    {
        size = SizeEncoding::Size_32Bytes;
    }
    // 三者位于高 32B
    else if((addr_ccid >= 2) & (low_ccid >= 2) & (low_ccid >= 2))
    {
        size = SizeEncoding::Size_32Bytes;
    }
    else
    {
        size = SizeEncoding::Size_64Bytes;
    }

    txreq_flit->QoS                = 0;
    txreq_flit->TgtID              = 456; // TODO
    txreq_flit->SrcID              = 123; // TODO
    txreq_flit->TxnID              = 0;
    txreq_flit->CF1                = 0;
    txreq_flit->CF2                = 0;
    txreq_flit->CF3                = 0;
    txreq_flit->Opcode             = ReqOpcodeEncoding::WriteUniquePtl;
    txreq_flit->Size               = size;
    txreq_flit->Addr               = accreq_flit->addr;
    txreq_flit->NS                 = 0;
    txreq_flit->LikelyShared       = 0;
    txreq_flit->AllowRetry         = 1;
    txreq_flit->Order              = 0;
    txreq_flit->PCrdType           = 0;
    txreq_flit->MemAttr            = 0b0101;
    txreq_flit->CF4                = 1;
    txreq_flit->CF5                = 0;
    txreq_flit->CF6                = 0;
    txreq_flit->ExpCompAck         = 0;
    txreq_flit->TagOp              = 0;
    txreq_flit->TraceTag           = 0;
}


void IAccHandle::SetCHITxREQ_WriteUniqueFull(REQFLIT *txreq_flit)
{
    txreq_flit->QoS                = 0;
    txreq_flit->TgtID              = 456; // TODO
    txreq_flit->SrcID              = 123; // TODO
    txreq_flit->TxnID              = 0;
    txreq_flit->CF1                = 0;
    txreq_flit->CF2                = 0;
    txreq_flit->CF3                = 0;
    txreq_flit->Opcode             = ReqOpcodeEncoding::WriteUniquePtl;
    txreq_flit->Size               = SizeEncoding::Size_64Bytes;
    txreq_flit->Addr               = accreq_flit->addr;
    txreq_flit->NS                 = 0;
    txreq_flit->LikelyShared       = 0;
    txreq_flit->AllowRetry         = 1;
    txreq_flit->Order              = 0;
    txreq_flit->PCrdType           = 0;
    txreq_flit->MemAttr            = 0b0101;
    txreq_flit->CF4                = 1;
    txreq_flit->CF5                = 0;
    txreq_flit->CF6                = 0;
    txreq_flit->ExpCompAck         = 0;
    txreq_flit->TagOp              = 0;
    txreq_flit->TraceTag           = 0;
}





/******************************************************/
/*  Set CHI TxREQ: CopyBack Write Data Transaction    */
/******************************************************/
void IAccHandle::SetCHITxREQ_WriteBackFull(REQFLIT *txreq_flit)
{
    txreq_flit->QoS                = 0;
    txreq_flit->TgtID              = 456; // TODO
    txreq_flit->SrcID              = 123; // TODO
    txreq_flit->TxnID              = 0;
    txreq_flit->CF1                = 0;
    txreq_flit->CF2                = 0;
    txreq_flit->CF3                = 0;
    txreq_flit->Opcode             = ReqOpcodeEncoding::WriteBackFull;
    txreq_flit->Size               = SizeEncoding::Size_64Bytes;
    txreq_flit->Addr               = accreq_flit->addr;
    txreq_flit->NS                 = 0;
    txreq_flit->LikelyShared       = 0;
    txreq_flit->AllowRetry         = 1;
    txreq_flit->Order              = 0;
    txreq_flit->PCrdType           = 0;
    txreq_flit->MemAttr            = 0b0101;
    txreq_flit->CF4                = 1;
    txreq_flit->CF5                = 0;
    txreq_flit->CF6                = 0;
    txreq_flit->ExpCompAck         = 0;
    txreq_flit->TagOp              = 0;
    txreq_flit->TraceTag           = 0;
}


void IAccHandle::SetCHITxREQ_WriteCleanFull(REQFLIT *txreq_flit)
{
    txreq_flit->QoS                = 0;
    txreq_flit->TgtID              = 456; // TODO
    txreq_flit->SrcID              = 123; // TODO
    txreq_flit->TxnID              = 0;
    txreq_flit->CF1                = 0;
    txreq_flit->CF2                = 0;
    txreq_flit->CF3                = 0;
    txreq_flit->Opcode             = ReqOpcodeEncoding::WriteCleanFull;
    txreq_flit->Size               = SizeEncoding::Size_64Bytes;
    txreq_flit->Addr               = accreq_flit->addr;
    txreq_flit->NS                 = 0;
    txreq_flit->LikelyShared       = 0;
    txreq_flit->AllowRetry         = 1;
    txreq_flit->Order              = 0;
    txreq_flit->PCrdType           = 0;
    txreq_flit->MemAttr            = 0b0101;
    txreq_flit->CF4                = 1;
    txreq_flit->CF5                = 0;
    txreq_flit->CF6                = 0;
    txreq_flit->ExpCompAck         = 0;
    txreq_flit->TagOp              = 0;
    txreq_flit->TraceTag           = 0;
}


void IAccHandle::SetCHITxREQ_WriteEvictFull(REQFLIT *txreq_flit)
{
    txreq_flit->QoS                = 0;
    txreq_flit->TgtID              = 456; // TODO
    txreq_flit->SrcID              = 123; // TODO
    txreq_flit->TxnID              = 0;
    txreq_flit->CF1                = 0;
    txreq_flit->CF2                = 0;
    txreq_flit->CF3                = 0;
    txreq_flit->Opcode             = ReqOpcodeEncoding::WriteEvictFull;
    txreq_flit->Size               = SizeEncoding::Size_64Bytes;
    txreq_flit->Addr               = accreq_flit->addr;
    txreq_flit->NS                 = 0;
    txreq_flit->LikelyShared       = 0;
    txreq_flit->AllowRetry         = 1;
    txreq_flit->Order              = 0;
    txreq_flit->PCrdType           = 0;
    txreq_flit->MemAttr            = 0b1101;
    txreq_flit->CF4                = 1;
    txreq_flit->CF5                = 0;
    txreq_flit->CF6                = 0;
    txreq_flit->ExpCompAck         = 0;
    txreq_flit->TagOp              = 0;
    txreq_flit->TraceTag           = 0;
}





}
#endif