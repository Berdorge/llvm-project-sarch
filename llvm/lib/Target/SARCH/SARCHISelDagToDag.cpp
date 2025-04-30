#include "MCTargetDesc/SARCHMCTargetDesc.h"
#include "SARCH.h"
#include "SARCHISelLowering.h"
#include "SARCHTargetMachine.h"
#include "llvm/CodeGen/MachineFrameInfo.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/MachineRegisterInfo.h"
#include "llvm/CodeGen/SelectionDAG.h"
#include "llvm/CodeGen/SelectionDAGISel.h"
#include "llvm/CodeGen/TargetLowering.h"
#include "llvm/IR/CallingConv.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Intrinsics.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/Support/Compiler.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

#define DEBUG_TYPE "sarch-isel"

namespace {
class SARCHDAGToDAGISel : public SelectionDAGISel {

public:
  static char ID;
  SARCHDAGToDAGISel() = delete;
  explicit SARCHDAGToDAGISel(SARCHTargetMachine &TM, CodeGenOptLevel OptLevel)
      : SelectionDAGISel(TM, OptLevel) {
    SARCH_DUMP_RED
  }

  bool runOnMachineFunction(MachineFunction &MF) override {
    SARCH_DUMP_RED
    return SelectionDAGISel::runOnMachineFunction(MF);
  }

  void Select(SDNode *N) override;

  bool SelectAddr(SDValue Address, SDValue &Base);

#include "SARCHGenDAGISel.inc"
};

class SARCHDAGToDAGISelLegacy : public SelectionDAGISelLegacy {
public:
  static char ID;

  SARCHDAGToDAGISelLegacy(SARCHTargetMachine &TM, CodeGenOptLevel OptLevel)
      : SelectionDAGISelLegacy(ID, std::make_unique<SARCHDAGToDAGISel>(
                                       TM, OptLevel)){SARCH_DUMP_RED}

        StringRef getPassName() const override {
    return "SARCH DAG->DAG Pattern Instruction Selection";
  }
};
} // namespace

char SARCHDAGToDAGISelLegacy::ID = 0;

FunctionPass *llvm::createSARCHISelDag(SARCHTargetMachine &TM,
                                       CodeGenOptLevel OptLevel) {
  SARCH_DUMP_RED
  return new SARCHDAGToDAGISelLegacy(TM, OptLevel);
}

void SARCHDAGToDAGISel::Select(SDNode *Node) {
  SARCH_DUMP_RED
  if (Node->isMachineOpcode()) {
    Node->setNodeId(-1);
    return;
  }
  SDLoc DL(Node);
  SelectCode(Node);
}

bool SARCHDAGToDAGISel::SelectAddr(SDValue Address, SDValue &Base) {
  Base = Address;
  return true;
}
