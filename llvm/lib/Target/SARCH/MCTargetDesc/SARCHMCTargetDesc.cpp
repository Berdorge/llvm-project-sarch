#include "../SARCH.h"
#include "llvm/Support/raw_ostream.h"

// We need to define this function for linking to succeed
extern "C" LLVM_EXTERNAL_VISIBILITY void LLVMInitializeSARCHTargetMC() { SARCH_DUMP_MAGENTA }
