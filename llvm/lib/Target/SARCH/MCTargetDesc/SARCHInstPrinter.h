#ifndef LLVM_LIB_TARGET_SARCH_INSTPRINTER_SARCHINSTPRINTER_H
#define LLVM_LIB_TARGET_SARCH_INSTPRINTER_SARCHINSTPRINTER_H

#include "llvm/MC/MCInstPrinter.h"
#include "llvm/MC/MCRegister.h"

namespace llvm {
class SARCHInstPrinter : public MCInstPrinter {
public:
  SARCHInstPrinter(const MCAsmInfo &MAI, const MCInstrInfo &MII,
                   const MCRegisterInfo &MRI)
      : MCInstPrinter(MAI, MII, MRI) {}

  std::pair<const char *, uint64_t>
  getMnemonic(const MCInst &MI) const override;

  void printInstruction(const MCInst *MI, uint64_t Address, raw_ostream &O);
  static const char *getRegisterName(MCRegister Reg);

  static void printOperand(const MCOperand &MO, raw_ostream &O);

  void printRegName(raw_ostream &O, MCRegister Reg) override;
  void printInst(const MCInst *MI, uint64_t Address, StringRef Annot,
                 const MCSubtargetInfo &STI, raw_ostream &O) override;

  void printOperand(const MCInst *MI, int OpNo, raw_ostream &OS);
};
} // namespace llvm

#endif
