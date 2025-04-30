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
  Reserved.set(SARCH::RSP);

  if (TFI->hasFP(MF)) {
    Reserved.set(SARCH::R1);
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
  MachineBasicBlock &MBB = *MI.getParent();
  MachineFunction &MF = *MI.getParent()->getParent();
  DebugLoc DL = MI.getDebugLoc();
  const TargetInstrInfo *TII = MF.getSubtarget().getInstrInfo();

  int FrameIndex = MI.getOperand(FIOperandNum).getIndex();
  Register FrameReg;
  int Offset = getFrameLowering(MF)
                   ->getFrameIndexReference(MF, FrameIndex, FrameReg)
                   .getFixed();

  if (!isInt<16>(Offset)) {
    llvm_unreachable("");
  }

  MI.getOperand(FIOperandNum).ChangeToRegister(FrameReg, false, false, false);

  BuildMI(MBB, II, DL, TII->get(SARCH::ADD_IMM), FrameReg)
      .addReg(FrameReg)
      .addImm(Offset);

  BuildMI(MBB, std::next(II), DL, TII->get(SARCH::SUB_IMM), FrameReg)
      .addReg(FrameReg)
      .addImm(Offset);

  return false;
}

Register SARCHRegisterInfo::getFrameRegister(const MachineFunction &MF) const {
  SARCH_DUMP_GREEN
  const SARCHFrameLowering *TFI = getFrameLowering(MF);
  return TFI->hasFP(MF) ? SARCH::R1 : SARCH::RSP;
}

const uint32_t *
SARCHRegisterInfo::getCallPreservedMask(const MachineFunction &MF,
                                        CallingConv::ID CC) const {
  SARCH_DUMP_GREEN
  return CSR_SARCH_RegMask;
}
