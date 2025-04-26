#ifndef LLVM_LIB_TARGET_SARCH_MCTARGETDESC_SARCHMCTARGETDESC_H
#define LLVM_LIB_TARGET_SARCH_MCTARGETDESC_SARCHMCTARGETDESC_H

#include <memory>

namespace llvm {
class MCCodeEmitter;
class MCContext;
class MCInstrInfo;
class MCAsmBackend;
class MCObjectTargetWriter;
class MCRegisterInfo;
class MCSubtargetInfo;
class MCTargetOptions;
class Target;

MCCodeEmitter *createSARCHMCCodeEmitter(const MCInstrInfo &MCII,
                                        MCContext &Ctx);
MCAsmBackend *createSARCHAsmBackend(const Target &T, const MCSubtargetInfo &STI,
                                    const MCRegisterInfo &MRI,
                                    const MCTargetOptions &Options);
std::unique_ptr<MCObjectTargetWriter> createSARCHELFObjectWriter(bool Is64Bit,
                                                                 uint8_t OSABI);
} // namespace llvm

#define GET_REGINFO_ENUM
#include "SARCHGenRegisterInfo.inc"

#define GET_INSTRINFO_ENUM
#include "SARCHGenInstrInfo.inc"

#endif
