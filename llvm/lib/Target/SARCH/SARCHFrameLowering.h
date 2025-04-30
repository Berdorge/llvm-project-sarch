#ifndef LLVM_LIB_TARGET_SARCH_SARCHFRAMELOWERING_H
#define LLVM_LIB_TARGET_SARCH_SARCHFRAMELOWERING_H

#include "SARCH.h"
#include "llvm/CodeGen/TargetFrameLowering.h"

namespace llvm {
class SARCHSubtarget;

class SARCHFrameLowering : public TargetFrameLowering {
  const SARCHSubtarget &subtarget;

public:
  explicit SARCHFrameLowering(const SARCHSubtarget &subtarget)
      : TargetFrameLowering(TargetFrameLowering::StackGrowsDown, Align(4), 0),
        subtarget(subtarget) {
    SARCH_DUMP_GREEN
  }

  void emitPrologue(MachineFunction &MF, MachineBasicBlock &MBB) const override;
  void emitEpilogue(MachineFunction &MF, MachineBasicBlock &MBB) const override;

  MachineBasicBlock::iterator
  eliminateCallFramePseudoInstr(MachineFunction &MF, MachineBasicBlock &MBB,
                                MachineBasicBlock::iterator I) const override;

  bool hasFPImpl(const MachineFunction &MF) const override;
};
} // namespace llvm

#endif
