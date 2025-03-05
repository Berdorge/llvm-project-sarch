add_llvm_component_library(LLVMSARCHDesc
  SARCHMCTargetDesc.cpp

  LINK_COMPONENTS
  MC
  Support
  SARCHInfo

  ADD_TO_COMPONENT
  SARCH
 )
