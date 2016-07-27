switch (op.digit0_1) {
case 0x0: {switch (op.digit5_7) {
	case 0x0: {switch (op.digit2_2) {
		case 0x0: {switch (op.digit3_4) {
			case 0x0: {//NOP 0x00
				} break;
			case 0x1: {//LD (N), SP 0x08
				u16 *s = &sp;
				u16 *d = (u16*) &mem[imm16];
				*d = *s;} break;
			case 0x2: {//STOP 0x10
				stop();} break;
			case 0x3: {//JR N 0x18
				u8 *s = &imm8;
				u16 *d = &pc;
				*d = (*d) + (*s);} break;
			default: /*Default*/ break;
			}} break;
		case 0x1: {//JR F, N 0x20, 0x28, 0x30, 0x38
			if (flag(op.F)) break;
			u8 *s = &imm8;
			u16 *d = &pc;
			*d = (*d) + (*s);} break;
		default: /*Default*/ break;
		}} break;
	case 0x1: {switch (op.digit4_4) {
		case 0x0: {//LD R, N 0x01, 0x11, 0x21, 0x31
			u16 *s = &imm16;
			u16 *d = (op.R ^ 0b11) ? &regPair[op.R] : &sp;
			*d = *s;} break;
		case 0x1: {//ADD HL, R 0x09, 0x19, 0x29, 0x39
			u16 *s = &regPair[REG_HL];
			u16 *d = (op.R ^ 0b11) ? &regPair[op.R] : &sp;
			auto *tmp = s; s = d; d=tmp;
			*d = (*d) + (*s);} break;
		default: /*Default*/ break;
		}} break;
	case 0x2: {switch (op.digit2_2) {
		case 0x0: {switch (op.digit4_4) {
			case 0x0: {//LD (R), A 0x02, 0x12
				u8 *s = &mem[regPair[op.sR]];
				u8 *d = &reg[0x7];
				auto *tmp = s; s = d; d=tmp;
				*d = *s;} break;
			case 0x1: {//LD A, (R) 0x0a, 0x1a
				u8 *s = &mem[regPair[op.sR]];
				u8 *d = &reg[0x7];
				*d = *s;} break;
			default: /*Default*/ break;
			}} break;
		case 0x1: {switch (op.digit3_4) {
			case 0x0: {//LDD (HL), A 0x22
				u8 *s = &mem[regPair[REG_HL]];
				u8 *d = &reg[0x7];
				auto *tmp = s; s = d; d=tmp;
				*d = *d - 1;} break;
			case 0x1: {//LDI A, (HL) 0x2a
				u8 *s = &mem[regPair[REG_HL]];
				u8 *d = &reg[0x7];
				*s = *s + 1;} break;
			case 0x3: {//LDD A, (HL) 0x3a
				u8 *s = &mem[regPair[REG_HL]];
				u8 *d = &reg[0x7];
				*s = *s - 1;} break;
			default: /*Default*/ break;
			}} break;
		default: /*Default*/ break;
		}} break;
	case 0x3: {switch (op.digit4_4) {
		case 0x0: {//INC R 0x03, 0x13, 0x23, 0x33
			u16 *d = (op.R ^ 0b11) ? &regPair[op.R] : &sp;
			*d = *d + 1;} break;
		case 0x1: {//DEC R 0x0b, 0x1b, 0x2b, 0x3b
			u16 *d = (op.R ^ 0b11) ? &regPair[op.R] : &sp;
			*d = *d - 1;} break;
		default: /*Default*/ break;
		}} break;
	case 0x4: {//INC D 0x04, 0x0c, 0x14, 0x1c, 0x24, 0x2c, 0x34, 0x3c
		u8 *d = (op.d1 ^ 0b110) ? &reg[op.d1] : &mem[regPair[0b10]];
		*d = *d + 1;} break;
	case 0x5: {//DEC D 0x05, 0x0d, 0x15, 0x1d, 0x25, 0x2d, 0x35, 0x3d
		u8 *d = (op.d1 ^ 0b110) ? &reg[op.d1] : &mem[regPair[0b10]];
		*d = *d - 1;} break;
	case 0x6: {//LD D, N 0x06, 0x0e, 0x16, 0x1e, 0x26, 0x2e, 0x36, 0x3e
		u8 *s = &imm8;
		u8 *d = (op.d1 ^ 0b110) ? &reg[op.d1] : &mem[regPair[0b10]];
		*d = *s;} break;
	case 0x7: {switch (op.digit2_2) {
		case 0x0: {//ROT A 0x07, 0x0f, 0x17, 0x1f
			u8 *d = &reg[0x7];
			*d = rot(*d, op.dir, op.nC);} break;
		case 0x1: {switch (op.digit3_4) {
			case 0x0: {//DAA 0x27
				u8 *d = &reg[0x7];
				*d = bcd(*d);} break;
			case 0x1: {//CPL 0x2f
				u8 *d = &reg[0x7];
				*d = ~(*d);} break;
			case 0x2: {//SCF 0x37
				reg[REG_F] = (reg[REG_F] & 0b10000000) | 0b00010000;} break;
			case 0x3: {//CCF 0x3f
				reg[REG_F] = (reg[REG_F] & 0b10010000) ^ 0b00010000;} break;
			default: /*Default*/ break;
			}} break;
		default: /*Default*/ break;
		}} break;
	default: /*Default*/ break;
	}} break;
case 0x1: {//LD D, D 0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4a, 0x4b, 0x4c, 0x4d, 0x4e, 0x4f, 0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5a, 0x5b, 0x5c, 0x5d, 0x5e, 0x5f, 0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6a, 0x6b, 0x6c, 0x6d, 0x6e, 0x6f, 0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7a, 0x7b, 0x7c, 0x7d, 0x7e, 0x7f
	if (checkHalt(op.value)) {halt(); break;}
	u8 *s = (op.d2 ^ 0b110) ? &reg[op.d2] : &mem[regPair[0b10]];
	u8 *d = (op.d1 ^ 0b110) ? &reg[op.d1] : &mem[regPair[0b10]];
	*d = *s;} break;
case 0x2: {//ALU OP, D 0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8a, 0x8b, 0x8c, 0x8d, 0x8e, 0x8f, 0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98, 0x99, 0x9a, 0x9b, 0x9c, 0x9d, 0x9e, 0x9f, 0xa0, 0xa1, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6, 0xa7, 0xa8, 0xa9, 0xaa, 0xab, 0xac, 0xad, 0xae, 0xaf, 0xb0, 0xb1, 0xb2, 0xb3, 0xb4, 0xb5, 0xb6, 0xb7, 0xb8, 0xb9, 0xba, 0xbb, 0xbc, 0xbd, 0xbe, 0xbf
	u8 *s = (op.d2 ^ 0b110) ? &reg[op.d2] : &mem[regPair[0b10]];
	u8 *d = &reg[0x7];
	*d = alu(*d, *s, op.d1);} break;
case 0x3: {switch (op.digit5_7) {
	case 0x0: {switch (op.digit2_2) {
		case 0x0: {//RET F 0xc0, 0xc8, 0xd0, 0xd8
			if (flag(op.F)) break;
			pc = *((u16*) &mem[sp]); sp -= 2;} break;
		case 0x1: {switch (op.digit3_4) {
			case 0x0: {//LD FF00+$N, A 0xe0
				u8 *s = &imm8;
				s = &mem[0xFF00 + *s];
				u8 *d = &reg[0x7];
				auto *tmp = s; s = d; d=tmp;
				*d = *s;} break;
			case 0x1: {//ADD SP, N 0xe8
				u16 *s = &imm16;
				u16 *d = &sp;
				*d = (*d) + (*s);} break;
			case 0x2: {//LD A, FF00+$N 0xf0
				u8 *s = &imm8;
				s = &mem[0xFF00 + *s];
				u8 *d = &reg[0x7];
				*d = *s;} break;
			case 0x3: {//TODO LD HL, SP+$N 0xf8
				u16 *s = &regPair[REG_HL];
				u16 *d = &sp;
				auto *tmp = s; s = d; d=tmp;} break;
			default: /*Default*/ break;
			}} break;
		default: /*Default*/ break;
		}} break;
	case 0x1: {switch (op.digit4_4) {
		case 0x0: {//POP R 0xc1, 0xd1, 0xe1, 0xf1
			u16 *s = &sp;
			u16 *d = &regPair[op.R];
			*d = mem[*s];
			*s = *s + 1;} break;
		case 0x1: {switch (op.digit2_3) {
			case 0x0: {//RET 0xc9
				pc = *((u16*) &mem[sp]); sp -= 2;} break;
			case 0x1: {//RETI 0xd9
				pc = *((u16*) &mem[sp]); sp -= 2;
				EI = 1;} break;
			case 0x2: {//JP HL 0xe9
				u16 *s = &regPair[REG_HL];
				pc = *s;} break;
			case 0x3: {//LD SP, HL 0xf9
				u16 *s = &regPair[REG_HL];
				u16 *d = &sp;
				*d = *s;} break;
			default: /*Default*/ break;
			}} break;
		default: /*Default*/ break;
		}} break;
	case 0x2: {switch (op.digit2_2) {
		case 0x0: {//JP F, N 0xc2, 0xca, 0xd2, 0xda
			if (flag(op.F)) break;
			u16 *s = &imm16;
			pc = *s;} break;
		case 0x1: {switch (op.digit3_4) {
			case 0x0: {//LD (C), A 0xe2
				u8 *s = &reg[0x1];
				s = &mem[0xFF00 + *s];
				u8 *d = &reg[0x7];
				auto *tmp = s; s = d; d=tmp;
				*d = *s;} break;
			case 0x1: {//LD (N), A 0xea
				u8 *s = &mem[imm16];
				u8 *d = &reg[0x7];
				auto *tmp = s; s = d; d=tmp;
				*d = mem[*s];} break;
			case 0x2: {//LD A, C 0xf2
				u8 *s = &reg[0x1];
				s = &mem[0xFF00 + *s];
				u8 *d = &reg[0x7];
				*d = *s;} break;
			case 0x3: {//LD A, (N) 0xfa
				u8 *s = &mem[imm16];
				u8 *d = &reg[0x7];
				*d = mem[*s];} break;
			default: /*Default*/ break;
			}} break;
		default: /*Default*/ break;
		}} break;
	case 0x3: {switch (op.digit2_4) {
		case 0x0: {//JP N 0xc3
			u16 *s = &imm16;
			pc = *s;} break;
		case 0x1: {//CB 0xcb
			cb();} break;
		case 0x6: {//DI 0xf3
			EI = 0;} break;
		case 0x7: {//EI 0xfb
			EI = 1;} break;
		default: /*Default*/ break;
		}} break;
	case 0x4: {//CALL F, N 0xc4, 0xcc, 0xd4, 0xdc
		if (flag(op.F)) break;
		u16 *s = &imm16;
		mem[sp] = pc; sp += 2; pc = *s;} break;
	case 0x5: {switch (op.digit4_4) {
		case 0x0: {//PUSH R 0xc5, 0xd5, 0xe5, 0xf5
			u16 *s = &sp;
			u16 *d = &regPair[op.R];
			auto *tmp = s; s = d; d=tmp;
			*d = mem[*s];
			*s = *s - 1;} break;
		case 0x1: {//CALL N 0xcd
			u16 *s = &imm16;
			mem[sp] = pc; sp += 2; pc = *s;} break;
		default: /*Default*/ break;
		}} break;
	case 0x6: {//ALU OP, N 0xc6, 0xce, 0xd6, 0xde, 0xe6, 0xee, 0xf6, 0xfe
		u8 *s = &imm8;
		u8 *d = &reg[0x7];
		*d = alu(*d, *s, op.d1);} break;
	case 0x7: {//RST 0xc7, 0xcf, 0xd7, 0xdf, 0xe7, 0xef, 0xf7, 0xff
		pc = op.d1 << 3; rst();} break;
	default: /*Default*/ break;
	}} break;
default: /*Default*/ break;
}
