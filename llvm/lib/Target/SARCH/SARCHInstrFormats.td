//===----------------------------------------------------------------------===//
//  CPU INSTRUCTION FORMATS
//
//  FF FF FF FF
//        ^^ ^^ - r3_imm - third register or immediate
//      ^       - r2     - second register
//     ^        - r1     - first register
//  ^^          - opcode - operation code
//===----------------------------------------------------------------------===//

// Generic SARCH Format
class SARCHInst<bits<8> op, dag outs, dag ins, string asmstr, list<dag> pattern>: Instruction {
  let Namespace = "SARCH";
  let DecoderNamespace = "SARCH";
  let Size = 4;

  field bits<32> Inst;

  bits<8> Opcode = op;
  bits<4>  r1;
  bits<4>  r2;
  bits<16> r3_imm;

  let Inst{31-24} = Opcode;
  let Inst{23-20} = r1;
  let Inst{19-16} = r2;
  let Inst{15-0}  = r3_imm;

  let OutOperandList = outs;
  let InOperandList  = ins;
  let AsmString   = asmstr;
  let Pattern     = pattern;
}

// Pseudo instructions format
class SARCHPseudoInst<dag outs, dag ins, string asmstr, list<dag> pattern>
   : SARCHInst<0, outs, ins, asmstr, pattern> {
  let isPseudo = 1;
  let isCodeGenOnly = 1;
}
