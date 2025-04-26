#include "MCTargetDesc/SARCHInfo.h"
#include "SARCH.h"
#include "SARCHInstPrinter.h"
#include "SARCHMCAsmInfo.h"
#include "TargetInfo/SARCHTargetInfo.h"
#include "llvm/MC/MCDwarf.h"
#include "llvm/MC/MCInstrInfo.h"
#include "llvm/MC/MCRegisterInfo.h"
// #include "llvm/Support/ErrorHandling.h"
#include "llvm/MC/MCSubtargetInfo.h"
#include "llvm/MC/TargetRegistry.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

#define GET_REGINFO_MC_DESC
#include "SARCHGenRegisterInfo.inc"

#define GET_INSTRINFO_MC_DESC
#include "SARCHGenInstrInfo.inc"

#define GET_SUBTARGETINFO_MC_DESC
#include "SARCHGenSubtargetInfo.inc"

static MCRegisterInfo *createSARCHMCRegisterInfo(const Triple &TT) {
  SARCH_DUMP_MAGENTA
  MCRegisterInfo *X = new MCRegisterInfo();
  InitSARCHMCRegisterInfo(X, SARCH::R0);
  return X;
}

static MCInstrInfo *createSARCHMCInstrInfo() {
  SARCH_DUMP_MAGENTA
  MCInstrInfo *X = new MCInstrInfo();
  InitSARCHMCInstrInfo(X);
  return X;
}

static MCSubtargetInfo *
createSARCHMCSubtargetInfo(const Triple &TT, StringRef CPU, StringRef FS) {
  SARCH_DUMP_MAGENTA
  return createSARCHMCSubtargetInfoImpl(TT, CPU, CPU, FS);
}

static MCAsmInfo *createSARCHMCAsmInfo(const MCRegisterInfo &MRI,
                                       const Triple &TT,
                                       const MCTargetOptions &Options) {
  SARCH_DUMP_MAGENTA
  MCAsmInfo *MAI = new SARCHMCAsmInfo(TT);
  unsigned SP = MRI.getDwarfRegNum(SARCH::R1, true);
  MCCFIInstruction Inst = MCCFIInstruction::cfiDefCfa(nullptr, SP, 0);
  MAI->addInitialFrameState(Inst);
  return MAI;
}

static MCInstPrinter *createSARCHMCInstPrinter(const Triple &T,
                                               unsigned SyntaxVariant,
                                               const MCAsmInfo &MAI,
                                               const MCInstrInfo &MII,
                                               const MCRegisterInfo &MRI) {
  SARCH_DUMP_MAGENTA
  return new SARCHInstPrinter(MAI, MII, MRI);
}

// We need to define this function for linking to succeed
extern "C" LLVM_EXTERNAL_VISIBILITY void LLVMInitializeSARCHTargetMC() {
  SARCH_DUMP_MAGENTA
  Target &TheSARCHTarget = getTheSARCHTarget();
  RegisterMCAsmInfoFn X(TheSARCHTarget, createSARCHMCAsmInfo);
  TargetRegistry::RegisterMCRegInfo(TheSARCHTarget, createSARCHMCRegisterInfo);
  TargetRegistry::RegisterMCInstrInfo(TheSARCHTarget, createSARCHMCInstrInfo);
  TargetRegistry::RegisterMCSubtargetInfo(TheSARCHTarget,
                                          createSARCHMCSubtargetInfo);
  TargetRegistry::RegisterMCInstPrinter(TheSARCHTarget,
                                        createSARCHMCInstPrinter);
}
