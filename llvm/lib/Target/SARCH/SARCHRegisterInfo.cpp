#include "SARCHRegisterInfo.h"
#include "SARCH.h"
#include "SARCHFrameLowering.h"
#include "llvm/CodeGen/TargetInstrInfo.h"

using namespace llvm;

#define GET_REGINFO_TARGET_DESC
#include "SARCHGenRegisterInfo.inc"

SARCHRegisterInfo::SARCHRegisterInfo() : SARCHGenRegisterInfo(SARCH::R0) {
  SARCH_DUMP_GREEN
}

const MCPhysReg *
SARCHRegisterInfo::getCalleeSavedRegs(const MachineFunction *MF) const {
  SARCH_DUMP_GREEN
  return CSR_SARCH_SaveList;
}

BitVector SARCHRegisterInfo::getReservedRegs(const MachineFunction &MF) const {
  SARCH_DUMP_GREEN
  SARCHFrameLowering const *TFI = getFrameLowering(MF);

  BitVector Reserved(getNumRegs());
  Reserved.set(SARCH::R1);

  if (TFI->hasFP(MF)) {
    Reserved.set(SARCH::R2);
  }
  return Reserved;
}

bool SARCHRegisterInfo::requiresRegisterScavenging(
    const MachineFunction &MF) const {
  return false;
}

bool SARCHRegisterInfo::eliminateFrameIndex(MachineBasicBlock::iterator II,
                                            int SPAdj, unsigned FIOperandNum,
                                            RegScavenger *RS) const {
  SARCH_DUMP_GREEN
  assert(SPAdj == 0 && "Unexpected non-zero SPAdj value");

  MachineInstr &MI = *II;
  MachineFunction &MF = *MI.getParent()->getParent();
  DebugLoc DL = MI.getDebugLoc();

  int FrameIndex = MI.getOperand(FIOperandNum).getIndex();
  Register FrameReg;
  int Offset = getFrameLowering(MF)
                   ->getFrameIndexReference(MF, FrameIndex, FrameReg)
                   .getFixed();
  Offset += MI.getOperand(FIOperandNum + 1).getImm();

  if (!isInt<16>(Offset)) {
    llvm_unreachable("");
  }

  MI.getOperand(FIOperandNum).ChangeToRegister(FrameReg, false, false, false);
  MI.getOperand(FIOperandNum + 1).ChangeToImmediate(Offset);
  return false;
}

Register SARCHRegisterInfo::getFrameRegister(const MachineFunction &MF) const {
  SARCH_DUMP_GREEN
  const TargetFrameLowering *TFI = getFrameLowering(MF);
  return TFI->hasFP(MF) ? SARCH::R2 : SARCH::R1;
}

const uint32_t *
SARCHRegisterInfo::getCallPreservedMask(const MachineFunction &MF,
                                        CallingConv::ID CC) const {
  SARCH_DUMP_GREEN
  return CSR_SARCH_RegMask;
}
