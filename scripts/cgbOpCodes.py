import csv
from pprint import pprint
from string import Template
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
        "ALU": "ALU",
        "R": "R",
        "r": "r",
        "D": "D",
        "D2": "D",
        "F": "F",
        "N": "N",
        "RST": "RST",
    }
    return "std::cout << \"%s\" << std::endl;" % Template(opCodes[tree][1]).substitute(d)

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
    print getSwitch(genCode, "/*Default*/", decodeTree, 0)
