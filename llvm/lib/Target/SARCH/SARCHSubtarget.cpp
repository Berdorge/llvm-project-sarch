#include "SARCHSubtarget.h"
#include "SARCH.h"
#include "llvm/Target/TargetMachine.h"

using namespace llvm;

#define DEBUG_TYPE "sarch-subtarget"

#define GET_SUBTARGETINFO_TARGET_DESC
#define GET_SUBTARGETINFO_CTOR
#include "SARCHGenSubtargetInfo.inc"

SARCHSubtarget::SARCHSubtarget(const Triple &TT, const std::string &CPU,
                               const std::string &FS, const TargetMachine &TM)
    : SARCHGenSubtargetInfo(TT, CPU, CPU, FS), TLInfo(TM, *this),
      FrameLowering(*this), InstrInfo() {
  SARCH_DUMP_CYAN
}
