//===----------------------------------------------------------------------===//
// This describes the calling conventions for SARCH architecture.
//===----------------------------------------------------------------------===//

//===----------------------------------------------------------------------===//
// SARCH Return Value Calling Convention
//===----------------------------------------------------------------------===//
def RetCC_SARCH : CallingConv<[
  CCIfType<[i32, i64], CCAssignToReg<[R0, R1, R2, R3]>>,

  CCIfType<[i64], CCAssignToStack<8, 4>>,
  CCIfType<[i32], CCAssignToStack<4, 4>>
]>;

//===----------------------------------------------------------------------===//
// SARCH Argument Calling Conventions
//===----------------------------------------------------------------------===//
def CC_SARCH : CallingConv<[
  CCIfType<[i8, i16], CCPromoteToType<i32>>,

  CCIfType<[i32, i64], CCAssignToReg<[R0, R1, R2, R3]>>,

  CCIfType<[i64], CCAssignToStack<8, 4>>,
  CCIfType<[i32], CCAssignToStack<4, 4>>
]>;

def CSR_SARCH : CalleeSavedRegs<(add R4, R5, R6, R7)>;

