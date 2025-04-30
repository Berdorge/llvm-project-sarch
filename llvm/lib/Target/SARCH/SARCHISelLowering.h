#ifndef LLVM_LIB_TARGET_SARCH_SARCHISELLOWERING_H
#define LLVM_LIB_TARGET_SARCH_SARCHISELLOWERING_H

#include "SARCH.h"
#include "llvm/CodeGen/SelectionDAG.h"
#include "llvm/CodeGen/TargetLowering.h"

namespace llvm {
class SARCHSubtarget;
class SARCHTargetMachine;

namespace SARCHISD {
enum NodeType : unsigned {
  FIRST_NUMBER = ISD::BUILTIN_OP_END,
  RET,
  CALL,
  CMP,
  CJMP,
  WRAPPER,
  SELECT_CC,
};
}

class SARCHTargetLowering : public TargetLowering {
public:
  explicit SARCHTargetLowering(const TargetMachine &TM,
                               const SARCHSubtarget &subtarget);

  const char *getTargetNodeName(unsigned Opcode) const override;

  bool isLegalAddressingMode(const DataLayout &DL, const AddrMode &AM, Type *Ty,
                             unsigned AS,
                             Instruction *I = nullptr) const override;

  SARCHSubtarget const &getSubtarget() const { return subtarget; }

private:
  const SARCHSubtarget &subtarget;

  void ReplaceNodeResults(SDNode *N, SmallVectorImpl<SDValue> &Results,
                          SelectionDAG &DAG) const override;

  SDValue PerformDAGCombine(SDNode *N, DAGCombinerInfo &DCI) const override;

  SDValue LowerFormalArguments(SDValue Chain, CallingConv::ID CallConv,
                               bool IsVarArg,
                               const SmallVectorImpl<ISD::InputArg> &Ins,
                               const SDLoc &DL, SelectionDAG &DAG,
                               SmallVectorImpl<SDValue> &InVals) const override;

  SDValue LowerCall(TargetLowering::CallLoweringInfo &CLI,
                    SmallVectorImpl<SDValue> &InVals) const override;

  SDValue LowerReturn(SDValue Chain, CallingConv::ID CallConv, bool IsVarArg,
                      const SmallVectorImpl<ISD::OutputArg> &Outs,
                      const SmallVectorImpl<SDValue> &OutVals, const SDLoc &DL,
                      SelectionDAG &DAG) const override;

  bool CanLowerReturn(CallingConv::ID CallConv, MachineFunction &MF,
                      bool IsVarArg,
                      const SmallVectorImpl<ISD::OutputArg> &ArgsFlags,
                      LLVMContext &Context, const Type *RetTy) const override;

  virtual SDValue LowerOperation(SDValue Op, SelectionDAG &DAG) const override;
  SDValue LowerBRCC(SDValue Op, SelectionDAG &DAG) const;
  SDValue LowerConstant(SDValue Op, SelectionDAG &DAG) const;
  SDValue LowerFrameIndex(SDValue Op, SelectionDAG &DAG) const;
  SDValue LowerExternalSymbol(SDValue Op, SelectionDAG &DAG) const;
  SDValue LowerSELECTCC(SDValue Op, SelectionDAG &DAG) const;
};
} // namespace llvm

#endif
