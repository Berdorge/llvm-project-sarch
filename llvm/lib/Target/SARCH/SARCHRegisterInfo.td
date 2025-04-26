class SARCHReg<bits<16> Encoding, string name> : Register<name> {
    let HWEncoding = Encoding;
    let Namespace = "SARCH";
}

foreach I = 0-7 in {
    def R#I : SARCHReg<I, "r"#I>;
}

def GPR : RegisterClass<"SARCH", [i32], 32, (add R0, R1, R2, R3, R4, R5, R6, R7)>;
