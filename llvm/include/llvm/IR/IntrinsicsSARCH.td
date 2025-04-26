let TargetPrefix = "sarch" in {
def int_sarch_putpixel: ClangBuiltin<"simPutPixel">,
  Intrinsic<[], [llvm_i32_ty, llvm_i32_ty, llvm_i32_ty], [IntrNoMem, IntrHasSideEffects]>;

def int_sarch_flush: ClangBuiltin<"simFlush">,
  Intrinsic<[], [], [IntrNoMem, IntrHasSideEffects]>;
}
