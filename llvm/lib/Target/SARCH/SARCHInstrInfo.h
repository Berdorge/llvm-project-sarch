#ifndef LLVM_LIB_TARGET_SARCH_SARCHINSTRINFO_H
#define LLVM_LIB_TARGET_SARCH_SARCHINSTRINFO_H

#include "MCTargetDesc/SARCHInfo.h"
#include "SARCHRegisterInfo.h"
#include "llvm/CodeGen/TargetInstrInfo.h"

#define GET_INSTRINFO_HEADER
#include "SARCHGenInstrInfo.inc"

namespace llvm {
class SARCHSubtarget;

class SARCHInstrInfo : public SARCHGenInstrInfo {
public:
  SARCHInstrInfo();
};
} // namespace llvm

#endif
