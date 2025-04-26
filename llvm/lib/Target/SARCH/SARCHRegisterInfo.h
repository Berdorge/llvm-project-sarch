#ifndef LLVM_LIB_TARGET_SARCH_SARCHREGISTERINFO_H
#define LLVM_LIB_TARGET_SARCH_SARCHREGISTERINFO_H

#define GET_REGINFO_HEADER
#include "SARCHGenRegisterInfo.inc"

namespace llvm {
class TargetInstrInfo;
class SARCHSubtarget;

struct SARCHRegisterInfo : public SARCHGenRegisterInfo {
public:
  SARCHRegisterInfo();

  const MCPhysReg *getCalleeSavedRegs(const MachineFunction *MF) const override;

  const uint32_t *getCallPreservedMask(const MachineFunction &MF,
                                       CallingConv::ID CC) const override;

  BitVector getReservedRegs(const MachineFunction &MF) const override;

  bool requiresRegisterScavenging(const MachineFunction &MF) const override;

  bool eliminateFrameIndex(MachineBasicBlock::iterator II, int SPAdj,
                           unsigned FIOperandNum,
                           RegScavenger *RS = nullptr) const override;

  Register getFrameRegister(const MachineFunction &MF) const override;
};
} // namespace llvm

#endif
