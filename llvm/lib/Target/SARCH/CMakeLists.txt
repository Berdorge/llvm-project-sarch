add_llvm_component_group(SARCH)

add_llvm_target(SARCHCodeGen
  SARCHTargetMachine.cpp

  LINK_COMPONENTS
  CodeGen
  Core
  Support
  Target
  SARCHInfo

  ADD_TO_COMPONENT
  SARCH
  )

add_subdirectory(MCTargetDesc)
add_subdirectory(TargetInfo)
