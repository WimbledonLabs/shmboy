const char* getDarkRSymbol (u16 value) {
    switch (value) {
        case 0: return "BC";
        case 1: return "DE";
        case 2: return "HL";
        case 3: return "AF";
        default: return "XXX";
    }
}
const char* getSmallRSymbol (u16 value) {
    switch (value) {
        case 0: return "BC";
        case 1: return "DE";
        default: return "XXX";
    }
}
const char* getLightRSymbol (u16 value) {
    switch (value) {
        case 0: return "BC";
        case 1: return "DE";
        case 2: return "HL";
        case 3: return "SP";
        default: return "XXX";
    }
}
const char* getFlagSymbol (u16 value) {
    switch (value) {
        case 0: return "NZ";
        case 1: return "Z";
        case 2: return "NC";
        case 3: return "C";
        default: return "XXX";
    }
}
const char* getDirSymbol (u16 value) {
    switch (value) {
        case 0: return "L";
        case 1: return "R";
        default: return "XXX";
    }
}
const char* getTheDSymbol (u16 value) {
    switch (value) {
        case 0: return "B";
        case 1: return "C";
        case 2: return "D";
        case 3: return "E";
        case 4: return "H";
        case 5: return "L";
        case 6: return "(HL)";
        case 7: return "A";
        default: return "XXX";
    }
}
const char* getAluSymbol (u16 value) {
    switch (value) {
        case 0: return "ADD";
        case 1: return "ADC";
        case 2: return "SUB";
        case 3: return "SBC";
        case 4: return "AND";
        case 5: return "XOR";
        case 6: return "OR";
        case 7: return "CP";
        default: return "XXX";
    }
}
int disassembleOpCode(char* buf, OpCode op, u8 imm8, u16 imm16) {
	switch (op.digit0_1) {
	case 0x0: {switch (op.digit5_7) {
		case 0x0: {switch (op.digit2_2) {
			case 0x0: {switch (op.digit3_4) {
				case 0x0: {sprintf(buf, "NOP"); return 1;} break;
				case 0x1: {sprintf(buf, "LD (0x%04x), SP", imm16); return 3;} break;
				case 0x2: {sprintf(buf, "STOP"); return 1;} break;
				case 0x3: {sprintf(buf, "JR 0x%02x", imm8); return 2;} break;
				default: /*Default*/ break;
				}} break;
			case 0x1: {sprintf(buf, "JR %s, 0x%02x", getFlagSymbol(op.F), imm8); return 2;} break;
			default: /*Default*/ break;
			}} break;
		case 0x1: {switch (op.digit4_4) {
			case 0x0: {sprintf(buf, "LD %s, 0x%04x", getLightRSymbol(op.R), imm16); return 3;} break;
			case 0x1: {sprintf(buf, "ADD HL, %s", getLightRSymbol(op.R)); return 1;} break;
			default: /*Default*/ break;
			}} break;
		case 0x2: {switch (op.digit2_2) {
			case 0x0: {switch (op.digit4_4) {
				case 0x0: {sprintf(buf, "LD (%s), A", getSmallRSymbol(op.sR)); return 1;} break;
				case 0x1: {sprintf(buf, "LD A, (%s)", getSmallRSymbol(op.sR)); return 1;} break;
				default: /*Default*/ break;
				}} break;
			case 0x1: {switch (op.digit3_4) {
				case 0x0: {sprintf(buf, "LDD (HL), A"); return 1;} break;
				case 0x1: {sprintf(buf, "LDI A, (HL)"); return 1;} break;
				case 0x3: {sprintf(buf, "LDD A, (HL)"); return 1;} break;
				default: /*Default*/ break;
				}} break;
			default: /*Default*/ break;
			}} break;
		case 0x3: {switch (op.digit4_4) {
			case 0x0: {sprintf(buf, "INC %s", getLightRSymbol(op.R)); return 1;} break;
			case 0x1: {sprintf(buf, "DEC %s", getLightRSymbol(op.R)); return 1;} break;
			default: /*Default*/ break;
			}} break;
		case 0x4: {sprintf(buf, "INC %s", getTheDSymbol(op.d1)); return 1;} break;
		case 0x5: {sprintf(buf, "DEC %s", getTheDSymbol(op.d1)); return 1;} break;
		case 0x6: {sprintf(buf, "LD %s, 0x%02x", getTheDSymbol(op.d1), imm8); return 2;} break;
		case 0x7: {switch (op.digit2_2) {
			case 0x0: {sprintf(buf, "R%s%sA", (op.dir) ? "R" : "L", (op.nC) ? "" : "C"); return 1;} break;
			case 0x1: {switch (op.digit3_4) {
				case 0x0: {sprintf(buf, "DAA"); return 1;} break;
				case 0x1: {sprintf(buf, "CPL"); return 1;} break;
				case 0x2: {sprintf(buf, "SCF"); return 1;} break;
				case 0x3: {sprintf(buf, "CCF"); return 1;} break;
				default: /*Default*/ break;
				}} break;
			default: /*Default*/ break;
			}} break;
		default: /*Default*/ break;
		}} break;
	case 0x1: {sprintf(buf, "LD %s, %s", getTheDSymbol(op.d1), getTheDSymbol(op.d2)); return 1;} break;
	case 0x2: {sprintf(buf, "%s A, %s", getAluSymbol(op.alu), getTheDSymbol(op.d2)); return 1;} break;
	case 0x3: {switch (op.digit5_7) {
		case 0x0: {switch (op.digit2_2) {
			case 0x0: {sprintf(buf, "RET %s", getFlagSymbol(op.F)); return 1;} break;
			case 0x1: {switch (op.digit3_4) {
				case 0x0: {sprintf(buf, "LD FF00+0x%02x, A", imm8); return 2;} break;
				case 0x1: {sprintf(buf, "ADD SP, 0x%04x", imm16); return 3;} break;
				case 0x2: {sprintf(buf, "LD A, FF00+0x%02x", imm8); return 2;} break;
				case 0x3: {sprintf(buf, "LD HL, SP+0x%02x", imm8); return 2;} break;
				default: /*Default*/ break;
				}} break;
			default: /*Default*/ break;
			}} break;
		case 0x1: {switch (op.digit4_4) {
			case 0x0: {sprintf(buf, "POP %s", getDarkRSymbol(op.R)); return 1;} break;
			case 0x1: {switch (op.digit2_3) {
				case 0x0: {sprintf(buf, "RET"); return 1;} break;
				case 0x1: {sprintf(buf, "RETI"); return 1;} break;
				case 0x2: {sprintf(buf, "JP HL"); return 1;} break;
				case 0x3: {sprintf(buf, "LD SP, HL"); return 1;} break;
				default: /*Default*/ break;
				}} break;
			default: /*Default*/ break;
			}} break;
		case 0x2: {switch (op.digit2_2) {
			case 0x0: {sprintf(buf, "JP %s, 0x%04x", getFlagSymbol(op.F), imm16); return 3;} break;
			case 0x1: {switch (op.digit3_4) {
				case 0x0: {sprintf(buf, "LD (C), A"); return 1;} break;
				case 0x1: {sprintf(buf, "LD (0x%04x), A", imm16); return 3;} break;
				case 0x2: {sprintf(buf, "LD A, C"); return 1;} break;
				case 0x3: {sprintf(buf, "LD A, (0x%04x)", imm16); return 3;} break;
				default: /*Default*/ break;
				}} break;
			default: /*Default*/ break;
			}} break;
		case 0x3: {switch (op.digit2_4) {
			case 0x0: {sprintf(buf, "JP 0x%04x", imm16); return 3;} break;
			case 0x1: {sprintf(buf, "CB"); return 1;} break;
			case 0x6: {sprintf(buf, "DI"); return 1;} break;
			case 0x7: {sprintf(buf, "EI"); return 1;} break;
			default: /*Default*/ break;
			}} break;
		case 0x4: {sprintf(buf, "CALL F, 0x%04x", imm16); return 3;} break;
		case 0x5: {switch (op.digit4_4) {
			case 0x0: {sprintf(buf, "PUSH %s", getDarkRSymbol(op.R)); return 1;} break;
			case 0x1: {sprintf(buf, "CALL 0x%04x", imm16); return 3;} break;
			default: /*Default*/ break;
			}} break;
		case 0x6: {sprintf(buf, "%s A, 0x%02x", getAluSymbol(op.alu), imm8); return 2;} break;
		case 0x7: {sprintf(buf, "RST N<<0x%02x", imm8); return 2;} break;
		default: /*Default*/ break;
		}} break;
	default: /*Default*/ break;
	}
}
