add_llvm_component_group(SARCH)

set(LLVM_TARGET_DEFINITIONS SARCH.td)

tablegen(LLVM SARCHGenRegisterInfo.inc -gen-register-info)
tablegen(LLVM SARCHGenInstrInfo.inc -gen-instr-info)
tablegen(LLVM SARCHGenSubtargetInfo.inc -gen-subtarget)
tablegen(LLVM SARCHGenMCPseudoLowering.inc -gen-pseudo-lowering)
tablegen(LLVM SARCHGenDAGISel.inc -gen-dag-isel)
tablegen(LLVM SARCHGenCallingConv.inc -gen-callingconv)
tablegen(LLVM SARCHGenAsmWriter.inc -gen-asm-writer)

add_public_tablegen_target(SARCHCommonTableGen)

add_llvm_target(SARCHCodeGen
  SARCHTargetMachine.cpp
  SARCHRegisterInfo.cpp
  SARCHInstrInfo.cpp
  SARCHSubtarget.cpp
  SARCHAsmPrinter.cpp
  SARCHISelDagToDag.cpp
  SARCHISelLowering.cpp
  SARCHMCInstLower.cpp

  LINK_COMPONENTS
  CodeGen
  Core
  Support
  Target
  SARCHInfo
  AsmPrinter
  SelectionDAG

  ADD_TO_COMPONENT
  SARCH
  )

add_subdirectory(MCTargetDesc)
add_subdirectory(TargetInfo)
