import csv
from pprint import pprint
from string import Template, Formatter
from decoderGenerator import getDecodeTree, getSwitch
from subprocess import call

branches = set("01")
wildcard = "XDFRArNd*"
ignore = " ,'"

def leftPad(s, c, size):
    return (c*(size-len(s))+s)[:size]

def match(ref, test):
    assert len(ref) == len(test), "%s is not the same size as %s" % (ref, test)
    for i in range(len(ref)):
        if ref[i] != test[i] and ref[i] != "*":
            return False

    return True

def genCode(tree):
    # Find all hex codes which map to this instruction
    binString = tree.replace(" ", "")
    allBinStrings = [ (bin(a)[2:]).zfill(8) for a in range(256) ]
    hexValues = [ "0x%02x" % int(a, 2) for a in allBinStrings if match(binString, a) ]

    # Get all of the micro code operations which are needed for this instruction
    leafMicroOps = [microOpCode[x] for x in range(len(microOps)) if opCodes[tree][1][x]];

    # Get the Mnemonic which corresponds to this instruction
    opCodeMnemonic = opCodes[tree][0]
    assert hexValues, "No binary string can be decoded with %s, probably the error in this program" % tree
    return "//%s %s%s" % ( opCodeMnemonic,
                               ", ".join(hexValues),
                               "\n" + "\n".join(leafMicroOps) )

def genDisassemble(tree):
    d = {
        "ALU_OP": "%s",
        "lR": "%s",
        "dR": "%s",
        "sR": "%s",
        "d1": "%s",
        "d2": "%s",
        "dir": "%s",
        "carry": "%s",
        "F": "%s",
        "imm8": "0x%02x",
        "imm16": "0x%04x",
    }

    mapToFunc = {
        "ALU_OP": "getAluSymbol(op.alu)",
        "lR": "getLightRSymbol(op.R)",
        "dR": "getDarkRSymbol(op.R)",
        "sR": "getSmallRSymbol(op.sR)",
        "d1": "getTheDSymbol(op.d1)",
        "d2": "getTheDSymbol(op.d2)",
        "dir": "(op.dir) ? \"R\" : \"L\"",
        "carry": "(op.nC) ? \"\" : \"C\"",
        "F": "getFlagSymbol(op.F)",
        "imm8": "imm8",
        "imm16": "imm16",
    }

    out = "sprintf(buf, \"%s\"%s);" % (Template(opCodes[tree][0]).substitute(d), \
            "".join([ ", %s" % mapToFunc[x[1]] for x in Formatter().parse(opCodes[tree][0]) if x[1] ]) )

    if "${imm8}" in opCodes[tree][0]:
        return out + " return 2;"

    if "${imm16}" in opCodes[tree][0]:
        return out + " return 3;"

    return out + " return 1;"

def defFromDict(name, d):
    print """const char* get%sSymbol (u16 value) {
    switch (value) {
%s
        default: return "XXX";
    }
}""" % ( name[0].upper() + name[1:], "\n".join(["        case %s: return \"%s\";" % t for t in d.iteritems()]) )

if __name__ == "__main__":
    call("libreoffice --headless --convert-to csv cgb_microcode.ods", shell=True)
    f = open("cgb_microcode.csv")
    output = open("microcode.py", 'w')

    output.write("opCodes = {\n")

    lines = list(csv.reader(f, skipinitialspace=True))
    microOps = lines[0][3:]
    microOpShortForm = lines[1][3:]
    microOpCode = lines[2][3:]

    instructionLen = max([len(l[1]) for l in lines[3:]])

    for line in lines[3:]:
        output.write( "\t\"%s\": (\"%s\",%s [%s]),\n" % (line[0], line[1], " "*(instructionLen-len(line[1])),  ",".join("1" if c else "0" for c in line[3:])) )

    output.write("}\n")

    output.close()
    f.close()

    from microcode import opCodes

    decodeTree = getDecodeTree(opCodes, wildcard, ignore)

    d = {
        0b000: "B",
        0b001: "C",
        0b010: "D",
        0b011: "E",
        0b100: "H",
        0b101: "L",
        0b110: "(HL)",
        0b111: "A",
    }

    sR = {
        0: "BC",
        1: "DE",
    }

    lR = {
        0b00: "BC",
        0b01: "DE",
        0b10: "HL",
        0b11: "SP",
    }

    dR = {
        0b00: "BC",
        0b01: "DE",
        0b10: "HL",
        0b11: "AF",
    }

    f = {
        0b00: "NZ",
        0b01: "Z",
        0b10: "NC",
        0b11: "C",
    }

    direction = {
        0: "L",
        1: "R",
    }

    alu = {
        0b000: "ADD",
        0b001: "ADC",
        0b010: "SUB",
        0b011: "SBC",
        0b100: "AND",
        0b101: "XOR",
        0b110: "OR",
        0b111: "CP",
    }

    defFromDict("darkR", dR)
    defFromDict("smallR", sR)
    defFromDict("lightR", lR)
    defFromDict("flag", f)
    defFromDict("dir", direction)
    defFromDict("theD", d)
    defFromDict("alu", alu)

    print "int disassembleOpCode(char* buf, OpCode op, u8 imm8, u16 imm16) {\n\t",
    print getSwitch(genDisassemble, "/*Default*/", decodeTree, 1)
    print "}"
