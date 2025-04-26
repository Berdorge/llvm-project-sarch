#include "SARCHInstrInfo.h"
#include "SARCH.h"
#include "llvm/CodeGen/MachineFrameInfo.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/MachineMemOperand.h"
#include "llvm/MC/TargetRegistry.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/ErrorHandling.h"

using namespace llvm;

#define GET_INSTRINFO_CTOR_DTOR
#include "SARCHGenInstrInfo.inc"

#define DEBUG_TYPE "SARCH-inst-info"

SARCHInstrInfo::SARCHInstrInfo() : SARCHGenInstrInfo() { SARCH_DUMP_GREEN }
