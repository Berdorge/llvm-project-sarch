#ifndef LLVM_LIB_TARGET_SARCH_SARCH_H
#define LLVM_LIB_TARGET_SARCH_SARCH_H

#include "MCTargetDesc/SARCHMCTargetDesc.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Target/TargetMachine.h"

#define SARCH_DUMP(Color)                                                      \
  {                                                                            \
    llvm::errs().changeColor(Color)                                            \
        << __func__ << "\n\t\t" << __FILE__ << ":" << __LINE__ << "\n";        \
    llvm::errs().changeColor(llvm::raw_ostream::WHITE);                        \
  }

#define SARCH_DUMP_RED SARCH_DUMP(llvm::raw_ostream::RED)
#define SARCH_DUMP_GREEN SARCH_DUMP(llvm::raw_ostream::GREEN)
#define SARCH_DUMP_YELLOW SARCH_DUMP(llvm::raw_ostream::YELLOW)
#define SARCH_DUMP_CYAN SARCH_DUMP(llvm::raw_ostream::CYAN)
#define SARCH_DUMP_MAGENTA SARCH_DUMP(llvm::raw_ostream::MAGENTA)

namespace llvm {
class SARCHTargetMachine;
class FunctionPass;
class SARCHSubtarget;
class AsmPrinter;
class InstructionSelector;
class MCInst;
class MCOperand;
class MachineInstr;
class MachineOperand;
class PassRegistry;

bool lowerSARCHMachineInstrToMCInst(const MachineInstr *MI, MCInst &OutMI,
                                    AsmPrinter &AP);
bool LowerSARCHMachineOperandToMCOperand(const MachineOperand &MO,
                                         MCOperand &MCOp, const AsmPrinter &AP);
FunctionPass *createSARCHISelDag(SARCHTargetMachine &TM,
                                 CodeGenOptLevel OptLevel);
} // namespace llvm

#endif // LLVM_LIB_TARGET_SARCH_SARCH_H
