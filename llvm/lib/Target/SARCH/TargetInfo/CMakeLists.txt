add_llvm_component_library(LLVMSARCHInfo
  SARCHTargetInfo.cpp

  LINK_COMPONENTS
  MC
  Support

  ADD_TO_COMPONENT
  SARCH
  )
