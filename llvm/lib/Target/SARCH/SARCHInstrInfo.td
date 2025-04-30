//===----------------------------------------------------------------------===//
// SARCH profiles and nodes
//===----------------------------------------------------------------------===//

def SDT_SARCHCompare       : SDTypeProfile<0, 3, []>;
def SDT_SARCHCondJump      : SDTypeProfile<0, 1, []>;
def SDT_SARCHCall          : SDTypeProfile<0, -1, []>;
def SDT_SARCHCallSeqStart  : SDCallSeqStart<[]>;
def SDT_SARCHCallSeqEnd    : SDCallSeqEnd<[]>;
def SDT_SARCHWrapper       : SDTypeProfile<1, 1, []>;
def SDT_SARCHSelectCC      : SDTypeProfile<1, 2, []>;

def SARCHret : SDNode<"SARCHISD::RET", SDTNone, [SDNPHasChain, SDNPOptInGlue, SDNPVariadic]>;

def SARCHcmp : SDNode<"SARCHISD::CMP", SDT_SARCHCompare, [SDNPOutGlue]>;

def SARCHcjmp : SDNode<"SARCHISD::CJMP", SDT_SARCHCondJump, [SDNPHasChain, SDNPInGlue]>;

def SARCHcall : SDNode<"SARCHISD::CALL", SDT_SARCHCall,
                        [SDNPHasChain, SDNPOutGlue, SDNPOptInGlue, SDNPVariadic]>;
def SARCHcallseq_start : SDNode<"ISD::CALLSEQ_START", SDT_SARCHCallSeqStart,
                                 [SDNPHasChain, SDNPOutGlue]>;
def SARCHcallseq_end : SDNode<"ISD::CALLSEQ_END", SDT_SARCHCallSeqEnd,
                               [SDNPHasChain, SDNPOptInGlue, SDNPOutGlue]>;
def SARCHwrapper : SDNode<"SARCHISD::WRAPPER", SDT_SARCHWrapper>;
def SARCHselectcc : SDNode<"SARCHISD::SELECT_CC", SDT_SARCHSelectCC, [SDNPInGlue]>;

//===----------------------------------------------------------------------===//
// Instruction format superclass
//===----------------------------------------------------------------------===//

include "SARCHInstrFormats.td"

//===----------------------------------------------------------------------===//
// SARCH Operand, Complex Patterns and Transformations Definitions.
//===----------------------------------------------------------------------===//

def addr : ComplexPattern<iPTR, 1, "SelectAddr", [], []>;

def MemAsmOperand : AsmOperandClass {
  let Name = "Mem";
}

class SImmAsmOperand<int width> : AsmOperandClass {
  let Name = "SImm" # width;
  let RenderMethod = "addImmOperands";
  let DiagnosticType = !strconcat("Invalid", Name);
}

def simm16 : Operand<i32>, ImmLeaf<i32, [{return isUInt<16>(Imm);}]> {
  let ParserMatchClass = SImmAsmOperand<16>;
  let EncoderMethod = "getSImm16OpValue";
  let DecoderMethod = "decodeSImmOperand<16>";
  let MCOperandPredicate = [{
    int64_t Imm;
    if (MCOp.evaluateAsConstantImm(Imm))
      return isInt<16>(Imm);
    return MCOp.isBareSymbolRef();
  }];
  let OperandType = "OPERAND_SIMM16";
  let OperandNamespace = "SARCHOp";
}

def jtarget16 : Operand<OtherVT> {
  let ParserMatchClass = SImmAsmOperand<16>;
  let PrintMethod = "printJumpOperand";
  let EncoderMethod = "getJumpTarget16OpValue";
  let DecoderMethod = "decodeSImmOperand<16>";
  let MCOperandPredicate = [{
    int64_t Imm;
    if (MCOp.evaluateAsConstantImm(Imm)) {
      return isInt<16>(Imm);
    }
    return MCOp.isBareSymbolRef();
  }];
  let OperandType = "OPERAND_PCREL";
}

def memsrc : Operand<i32> {
  let PrintMethod = "printSrcMemOperand";
  let MIOperandInfo = (ops GPR);
  let ParserMatchClass = MemAsmOperand;
  let EncoderMethod = "getMemOpValue";
  let DecoderMethod = "DecodeMemOperand";
}

def memdst : Operand<i32> {
  let PrintMethod = "printSrcMemOperand";
  let MIOperandInfo = (ops GPR);
  let ParserMatchClass = MemAsmOperand;
  let EncoderMethod = "getMemOpValue";
  let DecoderMethod = "DecodeMemOperand";
}

def cc : Operand<i32> {
  let PrintMethod = "printCCOperand";
  let EncoderMethod = "getCCOpValue";
}

def LI16 : SDNodeXForm<imm, [{
  return CurDAG->getTargetConstant((uint64_t)N->getZExtValue() & 0xffff,
                                  SDLoc(N), MVT::i32);
}]>;

def HI16 : SDNodeXForm<imm, [{
  return CurDAG->getTargetConstant((uint64_t)N->getZExtValue() >> 16, SDLoc(N),
                                  MVT::i32);
}]>;

//===----------------------------------------------------------------------===//
// SARCH Instructions
//===----------------------------------------------------------------------===//

let r2 = 0,
    Defs = [FLAG] in
def CMP_IMM : SARCHInst<0x00, (outs), (ins GPR:$r1, simm16:$r3_imm, cc:$cond),
                          "cmp$cond $r1 $r3_imm",
                          [(SARCHcmp GPR:$r1, simm16:$r3_imm, simm16:$cond)]> {
    bits<8> cond;
    let Opcode = cond;
}

let hasSideEffects = 0, mayLoad = 0, mayStore = 0, isBarrier = 1, isBranch = 1, isTerminator = 1,
    r1 = 0, r2 = 0 in
def JUMP_IMM : SARCHInst<0x10, (outs), (ins jtarget16:$r3_imm), "jmp $r3_imm",
                         [(br bb:$r3_imm)]>;

let hasSideEffects = 0, mayLoad = 0, mayStore = 0, isCall = 1, isBranch = 1, isTerminator = 1,
    r2 = 0, r3_imm = 0 in
def JUMP_REG : SARCHInst<0x11, (outs), (ins GPR:$r1), "jmp $r1",
                         [(brind GPR:$r1)]>;

let hasSideEffects = 0, mayLoad = 0, mayStore = 0, isBarrier = 1, isBranch = 1, isTerminator = 1,
    r1 = 0, r2 = 0,
    Uses = [FLAG] in
def CJUMP : SARCHInst<0x12, (outs), (ins jtarget16:$r3_imm),
                      "cjmp $r3_imm",
                      [(SARCHcjmp bb:$r3_imm)]>;

let isCall = 1,
    r1 = 0, r2 = 0,
    Defs = [R0, R1, R2, R3],
    Uses = [RSP] in {
  def CALL_IMM : SARCHInst<0x30,
                           (outs), (ins simm16:$r3_imm),
                           "call $r3_imm", [(SARCHcall simm16:$r3_imm)]>;
  def CALL_REG : SARCHInst<0x31,
                           (outs), (ins GPR:$r3_imm),
                           "call $r3_imm", [(SARCHcall GPR:$r3_imm)]>;
}

let Defs = [RSP], Uses = [RSP] in {
def ADJCALLSTACKDOWN : SARCHPseudoInst<(outs), (ins simm16:$amt1, simm16:$amt2),
                              "#ADJCALLSTACKDOWN $amt1 $amt2",
                              [(SARCHcallseq_start i32:$amt1, i32:$amt2)]>;
def ADJCALLSTACKUP   : SARCHPseudoInst<(outs), (ins simm16:$amt1, simm16:$amt2),
                              "#ADJCALLSTACKUP $amt1 $amt2",
                              [(SARCHcallseq_end i32:$amt1, i32:$amt2)]>;
}

let hasSideEffects = 1, mayLoad = 1, mayStore = 0, isCall = 0, isBranch = 1, isReturn = 1,
    isBarrier = 1, isTerminator = 1, r1 = 0, r2 = 0, r3_imm = 0 in
def RETURN : SARCHInst<0x3F, (outs), (ins), "ret", [(SARCHret)]>;

let isAsCheapAsAMove = 1, isReMaterializable = 1,
    r2 = 0 in
def ASSIGN_IMM : SARCHInst<0x40, (outs GPR:$r1), (ins simm16:$r3_imm),
                           "asgn $r1 $r3_imm",
                           [(set GPR:$r1, simm16:$r3_imm)]>;

let isAsCheapAsAMove = 1, isReMaterializable = 1,
    r2 = 0 in
def ASSIGN_REG : SARCHInst<0x41, (outs GPR:$r1), (ins GPR:$r3_imm),
                           "asgn $r1 $r3_imm",
                           []>;

let isCodeGenOnly = 1,
    Uses = [FLAG] in
def SELECT : SARCHInst<0x42, (outs GPR:$r1), (ins GPR:$r2, GPR:$r3_imm),
                       "select $r1 $r2 $r3_imm",
                       [(set GPR:$r1, (SARCHselectcc GPR:$r2, GPR:$r3_imm))]>;

let mayStore = 1,
    r2 = 0 in
def STORE_REG : SARCHInst<0x48, (outs), (ins memdst:$r1, GPR:$r3_imm),
                          "store $r1 $r3_imm",
                          [(store GPR:$r3_imm, addr:$r1)]>;

let mayLoad = 1,
    r2 = 0 in
def LOAD : SARCHInst<0x49, (outs GPR:$r1), (ins memsrc:$r3_imm),
                     "load $r1 $r3_imm",
                     [(set GPR:$r1, (load addr:$r3_imm))]>;

let hasSideEffects = 0, mayLoad = 0, mayStore = 0,
    r2 = 0 in
def OR_IMM : SARCHInst<0x50, (outs GPR:$r1), (ins GPR:$source, simm16:$r3_imm),
                       "or $r1 $r3_imm", [(set GPR:$r1, (or GPR:$source, simm16:$r3_imm))]>;

let hasSideEffects = 0, mayLoad = 0, mayStore = 0,
    r2 = 0 in
def OR_REG : SARCHInst<0x51, (outs GPR:$r1), (ins GPR:$source, GPR:$r3_imm),
                       "or $r1 $r3_imm", [(set GPR:$r1, (or GPR:$source, GPR:$r3_imm))]>;

let hasSideEffects = 0, mayLoad = 0, mayStore = 0,
    r2 = 0 in
def AND_IMM : SARCHInst<0x52, (outs GPR:$r1), (ins GPR:$source, simm16:$r3_imm),
                       "and $r1 $r3_imm", [(set GPR:$r1, (and GPR:$source, simm16:$r3_imm))]>;

let hasSideEffects = 0, mayLoad = 0, mayStore = 0,
    r2 = 0 in
def AND_REG : SARCHInst<0x53, (outs GPR:$r1), (ins GPR:$source, GPR:$r3_imm),
                       "and $r1 $r3_imm",
                       [(set GPR:$r1, (and GPR:$source, GPR:$r3_imm))]>;

let hasSideEffects = 0, mayLoad = 0, mayStore = 0,
    r2 = 0 in
def SHL_IMM : SARCHInst<0x54, (outs GPR:$r1), (ins GPR:$source, simm16:$r3_imm),
                        "shl $r1 $r3_imm",
                        [(set GPR:$r1, (shl GPR:$source, simm16:$r3_imm))]>;

let hasSideEffects = 0, mayLoad = 0, mayStore = 0,
    r2 = 0 in
def SHR_IMM : SARCHInst<0x56, (outs GPR:$r1), (ins GPR:$source, simm16:$r3_imm),
                        "shr $r1 $r3_imm",
                        [(set GPR:$r1, (srl GPR:$source, simm16:$r3_imm))]>;

let hasSideEffects = 0, mayLoad = 0, mayStore = 0,
    r2 = 0 in
def ADD_REG : SARCHInst<0x60, (outs GPR:$r1), (ins GPR:$source, GPR:$r3_imm),
                        "add $r1 $r3_imm", [(set GPR:$r1, (add GPR:$source, GPR:$r3_imm))]>;

let hasSideEffects = 0, mayLoad = 0, mayStore = 0,
    r2 = 0 in
def ADD_IMM : SARCHInst<0x61, (outs GPR:$r1), (ins GPR:$source, simm16:$r3_imm),
                        "add $r1 $r3_imm", [(set GPR:$r1, (add GPR:$source, simm16:$r3_imm))]>;

let hasSideEffects = 0, mayLoad = 0, mayStore = 0,
    r2 = 0 in
def SUB_IMM : SARCHInst<0x63, (outs GPR:$r1), (ins GPR:$source, simm16:$r3_imm),
                        "sub $r1 $r3_imm", [(set GPR:$r1, (sub GPR:$source, simm16:$r3_imm))]>;

let hasSideEffects = 0, mayLoad = 0, mayStore = 0,
    r2 = 0 in
def MUL_REG : SARCHInst<0x64, (outs GPR:$r1), (ins GPR:$source, GPR:$r3_imm),
                        "mul $r1 $r3_imm", [(set GPR:$r1, (mul GPR:$source, GPR:$r3_imm))]>;

let r1 = 0, r2 = 0, r3_imm = 0 in
def FLUSH : SARCHInst<0xF0, (outs), (ins), "flush", [(int_sarch_flush)]>;

def PUTPIXEL : SARCHInst<0xF1, (outs), (ins GPR:$r1, GPR:$r2, GPR:$r3_imm),
                         "putpx $r1 $r2 $r3_imm",
                         [(int_sarch_putpixel GPR:$r1, GPR:$r2, GPR:$r3_imm)]>;

def i32hi16 : Operand<i32>, PatLeaf<(i32 imm), [{
    // i32hi16 predicate - true if the 32-bit immediate has only leftmost 16
    // bits set.
    return ((N->getZExtValue() & 0xFFFF0000UL) == N->getZExtValue());}], HI16> {
}

def : Pat<(i32 imm:$r3_imm),
          (OR_IMM (SHL_IMM (ASSIGN_IMM (HI16 imm:$r3_imm)), 16), (LI16 imm:$r3_imm))>;
def : Pat<(i32 i32hi16:$r3_imm),
          (SHL_IMM (ASSIGN_IMM (HI16 imm:$r3_imm)), 16)>;

def : Pat<(i32 (SARCHwrapper texternalsym:$dst)), (ASSIGN_IMM texternalsym:$dst)>;
