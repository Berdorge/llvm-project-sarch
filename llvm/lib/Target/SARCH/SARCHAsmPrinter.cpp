#include "MCTargetDesc/SARCHInstPrinter.h"
#include "SARCH.h"
#include "SARCHSubtarget.h"
#include "SARCHTargetMachine.h"
#include "TargetInfo/SARCHTargetInfo.h"
#include "llvm/ADT/Statistic.h"
#include "llvm/CodeGen/AsmPrinter.h"
#include "llvm/CodeGen/MachineConstantPool.h"
#include "llvm/CodeGen/MachineFunctionPass.h"
#include "llvm/CodeGen/MachineInstr.h"
#include "llvm/CodeGen/MachineModuleInfo.h"
#include "llvm/MC/MCAsmInfo.h"
#include "llvm/MC/MCInst.h"
#include "llvm/MC/MCStreamer.h"
#include "llvm/MC/MCSymbol.h"
#include "llvm/MC/TargetRegistry.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

#define DEBUG_TYPE "asm-printer"

namespace {
class SARCHAsmPrinter : public AsmPrinter {
  const MCSubtargetInfo *STI;

public:
  explicit SARCHAsmPrinter(TargetMachine &TM,
                           std::unique_ptr<MCStreamer> Streamer)
      : AsmPrinter(TM, std::move(Streamer)), STI(TM.getMCSubtargetInfo()) {
    SARCH_DUMP_GREEN
  }

  void emitInstruction(const MachineInstr *MI) override;

  StringRef getPassName() const override { return "SARCH Assembly Printer"; }

  bool lowerPseudoInstExpansion(const MachineInstr *MI, MCInst &Inst);

  bool lowerOperand(const MachineOperand &MO, MCOperand &MCOp) const {
    return LowerSARCHMachineOperandToMCOperand(MO, MCOp, *this);
  }
};
} // namespace

#include "SARCHGenMCPseudoLowering.inc"

void SARCHAsmPrinter::emitInstruction(const MachineInstr *MI) {
  SARCH_DUMP_GREEN
  if (MCInst OutInst; lowerPseudoInstExpansion(MI, OutInst)) {
    EmitToStreamer(*OutStreamer, OutInst);
    return;
  }

  MCInst TmpInst;
  if (!lowerSARCHMachineInstrToMCInst(MI, TmpInst, *this)) {
    EmitToStreamer(*OutStreamer, TmpInst);
  }
}

extern "C" LLVM_EXTERNAL_VISIBILITY void LLVMInitializeSARCHAsmPrinter() {
  RegisterAsmPrinter<SARCHAsmPrinter> X(getTheSARCHTarget());
}
