#ifndef LLVM_LIB_TARGET_SARCH_MCTARGETDESC_SARCHFIXUPKINDS_H
#define LLVM_LIB_TARGET_SARCH_MCTARGETDESC_SARCHFIXUPKINDS_H

#include "llvm/MC/MCFixup.h"

namespace llvm::SARCH {
enum Fixups {
  fixup_SARCH_PC16 = FirstTargetFixupKind,
  LastTargetFixupKind,
  NumTargetFixupKinds = LastTargetFixupKind - FirstTargetFixupKind
};
}

#endif
