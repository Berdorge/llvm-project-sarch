#include "SARCHFrameLowering.h"
#include "SARCH.h"
#include "SARCHInstrInfo.h"
#include "llvm/CodeGen/MachineFrameInfo.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/MachineMemOperand.h"
#include "llvm/CodeGen/TargetInstrInfo.h"
#include "llvm/MC/TargetRegistry.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/ErrorHandling.h"

using namespace llvm;

void SARCHFrameLowering::emitPrologue(MachineFunction &MF,
                                      MachineBasicBlock &MBB) const {
  MachineBasicBlock::iterator MBBI = MBB.begin();
  DebugLoc DL = MBBI != MBB.end() ? MBBI->getDebugLoc() : DebugLoc();

  const TargetInstrInfo &TII = *MF.getSubtarget().getInstrInfo();
  int FrameSize = MF.getFrameInfo().getStackSize();

  const TargetRegisterInfo *TRI = MF.getSubtarget().getRegisterInfo();
  Register FrameReg = TRI->getFrameRegister(MF);

  BuildMI(MBB, MBBI, DL, TII.get(SARCH::SUB_IMM), FrameReg)
      .addReg(FrameReg)
      .addImm(FrameSize)
      .setMIFlag(MachineInstr::FrameSetup);
}

void SARCHFrameLowering::emitEpilogue(MachineFunction &MF,
                                      MachineBasicBlock &MBB) const {
  MachineBasicBlock::iterator MBBI = MBB.getLastNonDebugInstr();
  DebugLoc DL = MBBI->getDebugLoc();

  const TargetInstrInfo &TII = *MF.getSubtarget().getInstrInfo();
  int FrameSize = MF.getFrameInfo().getStackSize();

  const TargetRegisterInfo *TRI = MF.getSubtarget().getRegisterInfo();
  Register FrameReg = TRI->getFrameRegister(MF);

  BuildMI(MBB, MBBI, DL, TII.get(SARCH::ADD_IMM), FrameReg)
      .addReg(FrameReg)
      .addImm(FrameSize)
      .setMIFlag(MachineInstr::FrameDestroy);
}

MachineBasicBlock::iterator SARCHFrameLowering::eliminateCallFramePseudoInstr(
    MachineFunction &MF, MachineBasicBlock &MBB,
    MachineBasicBlock::iterator I) const {
  const SARCHInstrInfo &TII =
      *static_cast<const SARCHInstrInfo *>(MF.getSubtarget().getInstrInfo());
  MachineInstr &Old = *I;
  MachineInstr *New = nullptr;

  if (!hasReservedCallFrame(MF)) {
    int Amount = TII.getFrameSize(Old);
    Amount = alignTo(Amount, getStackAlign());
    if (Old.getOpcode() == TII.getCallFrameSetupOpcode()) {
      New = BuildMI(MF, Old.getDebugLoc(), TII.get(SARCH::SUB_IMM), SARCH::RSP)
                .addReg(SARCH::RSP)
                .addImm(Amount);
    } else {
      assert(Old.getOpcode() == TII.getCallFrameDestroyOpcode());
      int AlreadyPopped = Old.getOperand(1).getImm();
      Amount -= AlreadyPopped;
      New = BuildMI(MF, Old.getDebugLoc(), TII.get(SARCH::ADD_IMM), SARCH::RSP)
                .addReg(SARCH::RSP)
                .addImm(Amount);
    }
  } else if (Old.getOpcode() == TII.getCallFrameDestroyOpcode()) {
    int AlreadyPopped = Old.getOperand(1).getImm();
    New = BuildMI(MF, Old.getDebugLoc(), TII.get(SARCH::SUB_IMM), SARCH::RSP)
              .addReg(SARCH::RSP)
              .addImm(AlreadyPopped);
  }

  if (New) {
    MBB.insert(I, New);
  }

  return MBB.erase(I);
}

bool SARCHFrameLowering::hasFPImpl(const MachineFunction &MF) const {
  return false;
}
