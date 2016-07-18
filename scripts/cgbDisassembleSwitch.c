switch (op.digit0_1) {
case 0x0: switch (op.digit5_7) {
	case 0x0: switch (op.digit2_2) {
		case 0x0: switch (op.digit3_4) {
			case 0x0: std::cout << "NOP" << std::endl; break;
			case 0x1: std::cout << "LD N, SP" << std::endl; break;
			case 0x2: std::cout << "STOP" << std::endl; break;
			case 0x3: std::cout << "JR N" << std::endl; break;
			default: /*Default*/ break;
			} break;
		case 0x1: std::cout << "JR F, N" << std::endl; break;
		default: /*Default*/ break;
		} break;
	case 0x1: switch (op.digit4_4) {
		case 0x0: std::cout << "LD R, N" << std::endl; break;
		case 0x1: std::cout << "ADD HL, R" << std::endl; break;
		default: /*Default*/ break;
		} break;
	case 0x2: switch (op.digit2_2) {
		case 0x0: switch (op.digit4_4) {
			case 0x0: std::cout << "LD R, A" << std::endl; break;
			case 0x1: std::cout << "LD A, R" << std::endl; break;
			default: /*Default*/ break;
			} break;
		case 0x1: switch (op.digit3_4) {
			case 0x0: std::cout << "LDD HL, A" << std::endl; break;
			case 0x1: std::cout << "LDI A, HL" << std::endl; break;
			case 0x3: std::cout << "LDD A, HL" << std::endl; break;
			default: /*Default*/ break;
			} break;
		default: /*Default*/ break;
		} break;
	case 0x3: switch (op.digit4_4) {
		case 0x0: std::cout << "INC R" << std::endl; break;
		case 0x1: std::cout << "DEC R" << std::endl; break;
		default: /*Default*/ break;
		} break;
	case 0x4: std::cout << "INC D" << std::endl; break;
	case 0x5: std::cout << "DEC D" << std::endl; break;
	case 0x6: std::cout << "LD D, N" << std::endl; break;
	case 0x7: switch (op.digit2_3) {
		case 0x0: std::cout << "RdCA" << std::endl; break;
		case 0x1: std::cout << "RdA" << std::endl; break;
		case 0x2: switch (op.digit4_4) {
			case 0x0: std::cout << "DAA" << std::endl; break;
			case 0x1: std::cout << "CPL" << std::endl; break;
			default: /*Default*/ break;
			} break;
		case 0x3: switch (op.digit4_4) {
			case 0x0: std::cout << "SCF" << std::endl; break;
			case 0x1: std::cout << "CCF" << std::endl; break;
			default: /*Default*/ break;
			} break;
		default: /*Default*/ break;
		} break;
	default: /*Default*/ break;
	} break;
case 0x1: std::cout << "LD D, D" << std::endl; break;
case 0x2: std::cout << "ALU A, D" << std::endl; break;
case 0x3: switch (op.digit5_7) {
	case 0x0: switch (op.digit2_2) {
		case 0x0: std::cout << "RET F" << std::endl; break;
		case 0x1: switch (op.digit3_4) {
			case 0x0: std::cout << "LD FF00 + N, A" << std::endl; break;
			case 0x1: std::cout << "ADD SP, N" << std::endl; break;
			case 0x2: std::cout << "LD A, FF00 + N" << std::endl; break;
			case 0x3: std::cout << "LD HL, SP+N" << std::endl; break;
			default: /*Default*/ break;
			} break;
		default: /*Default*/ break;
		} break;
	case 0x1: switch (op.digit4_4) {
		case 0x0: std::cout << "POP R" << std::endl; break;
		case 0x1: switch (op.digit2_3) {
			case 0x0: std::cout << "RET" << std::endl; break;
			case 0x1: std::cout << "RETI" << std::endl; break;
			case 0x2: std::cout << "JP HL" << std::endl; break;
			case 0x3: std::cout << "LD SP, HL" << std::endl; break;
			default: /*Default*/ break;
			} break;
		default: /*Default*/ break;
		} break;
	case 0x2: switch (op.digit2_2) {
		case 0x0: std::cout << "JP F, N" << std::endl; break;
		case 0x1: switch (op.digit3_4) {
			case 0x0: std::cout << "LD C, A" << std::endl; break;
			case 0x1: std::cout << "LD N, A" << std::endl; break;
			case 0x2: std::cout << "LD A, C" << std::endl; break;
			case 0x3: std::cout << "LD A, N" << std::endl; break;
			default: /*Default*/ break;
			} break;
		default: /*Default*/ break;
		} break;
	case 0x3: switch (op.digit2_4) {
		case 0x0: std::cout << "JP N" << std::endl; break;
		case 0x1: std::cout << "MB" << std::endl; break;
		case 0x6: std::cout << "DI" << std::endl; break;
		case 0x7: std::cout << "EI" << std::endl; break;
		default: /*Default*/ break;
		} break;
	case 0x4: std::cout << "CALL F, N" << std::endl; break;
	case 0x5: switch (op.digit4_4) {
		case 0x0: std::cout << "PUSH R" << std::endl; break;
		case 0x1: std::cout << "CALL N" << std::endl; break;
		default: /*Default*/ break;
		} break;
	case 0x6: std::cout << "ALU A, N" << std::endl; break;
	case 0x7: std::cout << "RST RST" << std::endl; break;
	default: /*Default*/ break;
	} break;
default: /*Default*/ break;
}
