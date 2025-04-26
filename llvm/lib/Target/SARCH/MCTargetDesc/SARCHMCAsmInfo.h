#ifndef LLVM_LIB_TARGET_SARCH_MCTARGETDESC_SARCHMCASMINFO_H
#define LLVM_LIB_TARGET_SARCH_MCTARGETDESC_SARCHMCASMINFO_H

#include "llvm/MC/MCAsmInfoELF.h"

namespace llvm {
class Triple;

class SARCHMCAsmInfo : public MCAsmInfoELF {
public:
  explicit SARCHMCAsmInfo(const Triple &TT);
};
} // namespace llvm

#endif
