include "llvm/Target/Target.td"
include "SARCHRegisterInfo.td"
include "SARCHInstrInfo.td"
include "SARCHCallingConv.td"

class Proc<string Name, list<SubtargetFeature> Features>
 : Processor<Name, NoItineraries, Features>;

def : Proc<"generic", []>;

def SARCHInstrInfo : InstrInfo;
def SARCH : Target {
    let InstructionSet = SARCHInstrInfo;
}
