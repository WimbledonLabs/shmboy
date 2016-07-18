import csv
from pprint import pprint
from string import Template
from decoderGenerator import getDecodeTree, getSwitch
from subprocess import call

branches = set("01")
wildcard = "XDFRArNd*"
ignore = " ,'"

def genCode(tree):
    return "//" + opCodes[tree][0] + "\n" + "\n".join( [microOpCode[x] for x in range(3, len(microOps)) if opCodes[tree][1][x]] )

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
