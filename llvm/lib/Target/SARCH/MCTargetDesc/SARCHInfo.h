#ifndef LLVM_LIB_TARGET_SARCH_MCTARGETDESC_SARCHINFO_H
#define LLVM_LIB_TARGET_SARCH_MCTARGETDESC_SARCHINFO_H

#include "llvm/MC/MCInstrDesc.h"

namespace llvm {
namespace SARCHOp {
enum OperandType : unsigned {
  OPERAND_SIMM16 = MCOI::OPERAND_FIRST_TARGET,
};
}
} // namespace llvm

#endif
