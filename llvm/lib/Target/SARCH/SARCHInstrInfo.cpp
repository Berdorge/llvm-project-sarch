#include "SARCHInstrInfo.h"
#include "SARCH.h"
#include "llvm/CodeGen/MachineFrameInfo.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/MachineMemOperand.h"
#include "llvm/MC/TargetRegistry.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/ErrorHandling.h"

using namespace llvm;

#define GET_INSTRINFO_CTOR_DTOR
#include "SARCHGenInstrInfo.inc"

#define DEBUG_TYPE "SARCH-inst-info"

SARCHInstrInfo::SARCHInstrInfo()
    : SARCHGenInstrInfo(SARCH::ADJCALLSTACKDOWN, SARCH::ADJCALLSTACKUP) {
  SARCH_DUMP_GREEN
}

void SARCHInstrInfo::storeRegToStackSlot(
    MachineBasicBlock &MBB, MachineBasicBlock::iterator MI, Register SrcReg,
    bool isKill, int FrameIdx, const TargetRegisterClass *RC,
    const TargetRegisterInfo *TRI, Register VReg,
    MachineInstr::MIFlag Flags) const {
  DebugLoc DL;

  if (MI != MBB.end()) {
    DL = MI->getDebugLoc();
  }

  if (RC == &SARCH::GPRRegClass) {
    BuildMI(MBB, MI, DL, get(SARCH::STORE_REG))
        .addFrameIndex(FrameIdx)
        .addReg(SrcReg, getKillRegState(isKill));
  } else {
    llvm_unreachable("Cannot store this register to stack slot!");
  }
}

void SARCHInstrInfo::loadRegFromStackSlot(
    MachineBasicBlock &MBB, MachineBasicBlock::iterator MI, Register DestReg,
    int FrameIdx, const TargetRegisterClass *RC, const TargetRegisterInfo *TRI,
    Register VReg, MachineInstr::MIFlag Flags) const {
  DebugLoc DL;

  if (MI != MBB.end()) {
    DL = MI->getDebugLoc();
  }

  if (RC == &SARCH::GPRRegClass) {
    BuildMI(MBB, MI, DL, get(SARCH::LOAD))
        .addReg(DestReg, getDefRegState(true))
        .addFrameIndex(FrameIdx);
  } else {
    llvm_unreachable("Cannot store this register to stack slot!");
  }
}

void SARCHInstrInfo::copyPhysReg(MachineBasicBlock &MBB,
                                 MachineBasicBlock::iterator MBBI,
                                 const DebugLoc &DL, MCRegister DstReg,
                                 MCRegister SrcReg, bool KillSrc,
                                 bool RenamableDest, bool RenamableSrc) const {
  if (SARCH::GPRRegClass.contains(DstReg, SrcReg)) {
    BuildMI(MBB, MBBI, DL, get(SARCH::ASSIGN_REG), DstReg)
        .addReg(SrcReg, getKillRegState(KillSrc));
    return;
  }

  llvm_unreachable("can't copyPhysReg");
}
