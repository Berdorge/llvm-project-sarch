#ifndef LLVM_LIB_TARGET_SARCH_SARCHSUBTARGET_H
#define LLVM_LIB_TARGET_SARCH_SARCHSUBTARGET_H

#include "SARCH.h"
#include "SARCHFrameLowering.h"
#include "SARCHISelLowering.h"
#include "SARCHInstrInfo.h"
#include "SARCHRegisterInfo.h"
#include "llvm/CodeGen/SelectionDAGTargetInfo.h"
#include "llvm/CodeGen/TargetSubtargetInfo.h"

#define GET_SUBTARGETINFO_HEADER
#include "SARCHGenSubtargetInfo.inc"

namespace llvm {
class SARCHSubtarget : public SARCHGenSubtargetInfo {
  SARCHTargetLowering TLInfo;
  SARCHFrameLowering FrameLowering;
  SARCHRegisterInfo RegInfo;
  SARCHInstrInfo InstrInfo;
  SelectionDAGTargetInfo TSInfo;

public:
  SARCHSubtarget(const Triple &TT, const std::string &CPU,
                 const std::string &FS, const TargetMachine &TM);

  void ParseSubtargetFeatures(StringRef CPU, StringRef TuneCPU, StringRef FS);

  const SARCHTargetLowering *getTargetLowering() const override {
    SARCH_DUMP_CYAN
    return &TLInfo;
  }

  const SARCHFrameLowering *getFrameLowering() const override {
    SARCH_DUMP_CYAN
    return &FrameLowering;
  }

  const SARCHRegisterInfo *getRegisterInfo() const override {
    SARCH_DUMP_CYAN
    return &RegInfo;
  }

  const SARCHInstrInfo *getInstrInfo() const override { return &InstrInfo; }

  const SelectionDAGTargetInfo *getSelectionDAGInfo() const override {
    SARCH_DUMP_CYAN
    return &TSInfo;
  }
};
} // namespace llvm

#endif
