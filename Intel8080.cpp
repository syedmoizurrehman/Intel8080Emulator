#include "Intel8080.h"
#include <iostream>

Intel8080::Intel8080()
{
	A = B = C = D = E = H = L = 0;
	SP = 0xFFFF;				// 0x10000 - 1, SP starts from maximum address available.
	PC = 0;
	Z = S = P = CY = AC = 0;
}

void Intel8080::Execute()
{
	while (1)
		ExecuteInstruction();
}

void Intel8080::ExecuteInstruction()
{
	uint8_t* Opcode = &Memory[PC++];		// PC: Point to next instruction.
	unsigned TempCY = 0;
	uint8_t Temp8 = 0;
	uint32_t Result32 = 0;
	switch (*Opcode)
	{
	case 0x00: break;																						// NOP

	// LXI rp, data16 : rp = data16

	case 0x01: B = Opcode[2]; C = Opcode[1]; PC += 2; break;										// LXI BC, data16
	case 0x02: UnimplementedInstructionDetected(Opcode); break;												// STAX BC
	case 0x03: C++; if (C == 0) ++B; break;																	// INX BC

	case 0x04: TempCY = CY; RefreshFlags(B, 1); CY = TempCY; B += 1; break;									// INR B							
	case 0x05: TempCY = CY; RefreshFlags(B, 1, CPUOperation::Subtract); CY = TempCY; B -= 1; break;			// DCR B							

	case 0x06: B = Opcode[1]; PC += 1; break;															// MVI B, data

	case 0x07: UnimplementedInstructionDetected(Opcode); break;
	case 0x08: UnimplementedInstructionDetected(Opcode); break;

	case 0x09: Result32 = HL() + BC(); CY = (Result32 > 0x0000FFFF); H = (Result32 & 0x0000FF00) >> 8; L = Result32 & 0x000000FF; break;	// DAD BC : HL += BC

	case 0x0A: UnimplementedInstructionDetected(Opcode); break;
	case 0x0B: UnimplementedInstructionDetected(Opcode); break;

	case 0x0C: TempCY = CY; RefreshFlags(C, 1); CY = TempCY; C += 1; break;									// INR C
	case 0x0D: TempCY = CY; RefreshFlags(C, 1, CPUOperation::Subtract); CY = TempCY; C -= 1; break;			// DCR C							

	case 0x0E: C = Opcode[1]; PC += 1; break;															// MVI C, data
	case 0x0F: CY = ((A & 0x1) == 0x1); A = ((A & 0x1) << 0x7) | (A >> 0x1); break;							// RRC

	case 0x10: UnimplementedInstructionDetected(Opcode); break;

	case 0x11: D = Opcode[2]; E = Opcode[1]; PC += 2; break;										// LXI DE, data16

	case 0x12: UnimplementedInstructionDetected(Opcode); break;

	case 0x13: ++E; if (E == 0) ++D; break;																	// INX DE
	case 0x14: TempCY = CY; RefreshFlags(D, 1); CY = TempCY; D += 1; break;									// INR D
	case 0x15: TempCY = CY; RefreshFlags(D, 1, CPUOperation::Subtract); CY = TempCY; D -= 1; break;			// DCR D
	case 0x16: D = Opcode[1]; PC += 1; break;															// MVI D, data

	case 0x17: UnimplementedInstructionDetected(Opcode); break;
	case 0x18: UnimplementedInstructionDetected(Opcode); break;

	case 0x19: Result32 = HL() + DE(); CY = (Result32 > 0x0000FFFF); D = (Result32 & 0x0000FF00) >> 8; E = Result32 & 0x000000FF; break;	// DAD DE : HL += DE

	case 0x1A: A = Memory[DE()]; break;																		// LDAX DE

	case 0x1B: UnimplementedInstructionDetected(Opcode); break;

	case 0x1C: TempCY = CY; RefreshFlags(E, 1); CY = TempCY; E += 1; break;									// INR E
	case 0x1D: TempCY = CY; RefreshFlags(E, 1, CPUOperation::Subtract); CY = TempCY; E -= 1; break;			// DCR E
	case 0x1E: E = Opcode[1]; PC += 1; break;															// MVI E, data

	case 0x1F: UnimplementedInstructionDetected(Opcode); break;
	case 0x20: UnimplementedInstructionDetected(Opcode); break;

	case 0x21: H = Opcode[2]; L = Opcode[1]; PC += 2; break;										// LXI HL, data16

	case 0x22: UnimplementedInstructionDetected(Opcode); break;

	case 0x23: ++L; if (L == 0) ++H; break;																	// INX HL

	case 0x24: TempCY = CY; RefreshFlags(H, 1); CY = TempCY; H += 1; break;									// INR H
	case 0x25: TempCY = CY; RefreshFlags(H, 1, CPUOperation::Subtract); CY = TempCY; H -= 1; break;			// DCR H

	case 0x26: H = Opcode[1]; PC += 1; break;															// MVI H, data

	case 0x27: UnimplementedInstructionDetected(Opcode); break;
	case 0x28: UnimplementedInstructionDetected(Opcode); break;

	case 0x29: Result32 = HL() + HL(); CY = (Result32 > 0x0000FFFF); H = (Result32 & 0x0000FF00) >> 8; L = Result32 & 0x000000FF; break;	// DAD DE : HL += HL

	case 0x2A: UnimplementedInstructionDetected(Opcode); break;
	case 0x2B: UnimplementedInstructionDetected(Opcode); break;

	case 0x2C: TempCY = CY; RefreshFlags(L, 1); CY = TempCY; L += 1; break;									// INR L
	case 0x2D: TempCY = CY; RefreshFlags(L, 1, CPUOperation::Subtract); CY = TempCY; L -= 1; break;			// DCR L
	case 0x2E: L = Opcode[1]; PC += 1; break;		// MVI L, data

	case 0x2F: UnimplementedInstructionDetected(Opcode); break;
	case 0x30: UnimplementedInstructionDetected(Opcode); break;

	case 0x31: SP = Opcode[2] << 8 | Opcode[1]; PC += 2; break;									// LXI SP, data16

	case 0x32: Memory[Opcode[2] << 8 | Opcode[1]] = A; PC += 2; break;							// STA data16

	case 0x33: UnimplementedInstructionDetected(Opcode); break;

	case 0x34: TempCY = CY; RefreshFlags(Memory[HL()], 1); CY = TempCY; Memory[HL()] += 1; break;			// INR [HL]
	case 0x35: TempCY = CY; RefreshFlags(Memory[HL()], 1, CPUOperation::Subtract); CY = TempCY; Memory[HL()] -= 1; break;		// DCR [HL]
	case 0x36: Memory[HL()] = Opcode[1]; PC += 1; break;		// MVI [HL], data

	case 0x37: UnimplementedInstructionDetected(Opcode); break;
	case 0x38: UnimplementedInstructionDetected(Opcode); break;
	case 0x39: UnimplementedInstructionDetected(Opcode); break;

	case 0x3A: A = Memory[Opcode[2] << 8 | Opcode[1]]; PC += 2; break;							 // LDA addr

	case 0x3B: UnimplementedInstructionDetected(Opcode); break;
	case 0x3C: UnimplementedInstructionDetected(Opcode); break;

	case 0x3D: TempCY = CY; RefreshFlags(A, 1, CPUOperation::Subtract); CY = TempCY; A -= 1; break;			// DCR A
	case 0x3E: A = Opcode[1]; PC += 1; break;															// MVI A, data

	case 0x3F: UnimplementedInstructionDetected(Opcode); break;

	case 0x40: B = B; break;															// MOV B, B
	case 0x41: B = C; break;															// MOV B, C
	case 0x42: B = D; break;															// MOV B, D
	case 0x43: B = E; break;															// MOV B, E
	case 0x44: B = H; break;															// MOV B, H
	case 0x45: B = L; break;															// MOV B, L
	case 0x46: B = Memory[HL()]; break;													// MOV B, [HL]
	case 0x47: B = A; break;															// MOV B, A

	case 0x48: C = B; break;															// MOV C, B
	case 0x49: C = C; break;															// MOV C, C
	case 0x4A: C = D; break;															// MOV C, D
	case 0x4B: C = E; break;															// MOV C, E
	case 0x4C: C = H; break;															// MOV C, H
	case 0x4D: C = L; break;															// MOV C, L
	case 0x4E: C = Memory[HL()]; break;													// MOV C, [HL]
	case 0x4F: C = A; break;															// MOV C, A

	case 0x50: D = B; break;															// MOV D, B
	case 0x51: D = C; break;															// MOV D, C
	case 0x52: D = D; break;															// MOV D, D
	case 0x53: D = E; break;															// MOV D, E
	case 0x54: D = H; break;															// MOV D, H
	case 0x55: D = L; break;															// MOV D, L
	case 0x56: D = Memory[HL()]; break;													// MOV D, [HL]
	case 0x57: D = A; break;															// MOV D, A

	case 0x58: E = B; break;															// MOV E, B
	case 0x59: E = C; break;															// MOV E, C
	case 0x5A: E = D; break;															// MOV E, D
	case 0x5B: E = E; break;															// MOV E, E
	case 0x5C: E = H; break;															// MOV E, H
	case 0x5D: E = L; break;															// MOV E, L
	case 0x5E: E = Memory[HL()]; break;													// MOV E, [HL]
	case 0x5F: E = A; break;															// MOV E, A

	case 0x60: H = B; break;															// MOV H, B
	case 0x61: H = C; break;															// MOV H, C
	case 0x62: H = D; break;															// MOV H, D
	case 0x63: H = E; break;															// MOV H, E
	case 0x64: H = H; break;															// MOV H, H
	case 0x65: H = L; break;															// MOV H, L
	case 0x66: H = Memory[HL()]; break;													// MOV H, [HL]
	case 0x67: H = A; break;															// MOV H, A

	case 0x68: L = B; break;															// MOV L, B
	case 0x69: L = C; break;															// MOV L, C
	case 0x6A: L = D; break;															// MOV L, D
	case 0x6B: L = E; break;															// MOV L, E
	case 0x6C: L = H; break;															// MOV L, H
	case 0x6D: L = L; break;															// MOV L, L
	case 0x6E: L = Memory[HL()]; break;													// MOV L, [HL]
	case 0x6F: L = A; break;															// MOV L, A

	case 0x70: Memory[HL()] = B; break;													// MOV [HL], B
	case 0x71: Memory[HL()] = C; break;													// MOV [HL], C
	case 0x72: Memory[HL()] = D; break;													// MOV [HL], D
	case 0x73: Memory[HL()] = E; break;													// MOV [HL], E
	case 0x74: Memory[HL()] = H; break;													// MOV [HL], H
	case 0x75: Memory[HL()] = L; break;													// MOV [HL], L

	case 0x76: exit(0);																	// HLT

	case 0x77: Memory[HL()] = A; break;													// MOV [HL], A

	case 0x78: A = B; break;															// MOV A, B
	case 0x79: A = C; break;															// MOV A, C
	case 0x7A: A = D; break;															// MOV A, D
	case 0x7B: A = E; break;															// MOV A, E
	case 0x7C: A = H; break;															// MOV A, H
	case 0x7D: A = L; break;															// MOV A, L
	case 0x7E: A = Memory[HL()]; break;													// MOV A, [HL]
	case 0x7F: A = A; break;															// MOV A, A

	case 0x80: A += B; RefreshFlags(A, B); break;										// ADD B
	case 0x81: A += C; RefreshFlags(A, C); break;										// ADD C
	case 0x82: A += D; RefreshFlags(A, D); break;										// ADD D
	case 0x83: A += E; RefreshFlags(A, E); break;										// ADD E
	case 0x84: A += H; RefreshFlags(A, H); break;										// ADD H
	case 0x85: A += L; RefreshFlags(A, L); break;										// ADD L
	case 0x86: A += Memory[HL()]; RefreshFlags(A, Memory[HL()]); break;					// ADD [HL]
	case 0x87: A += A; RefreshFlags(A, A); break;										// ADD A

	// ADC r: A = A + r + CY | ADC M: A = A + [HL] + CY 

	case 0x88: A += B + CY; RefreshFlags(A, B, CY);	break;								// ADC B
	case 0x89: A += C + CY; RefreshFlags(A, C, CY);	break;								// ADC C
	case 0x8A: A += D + CY; RefreshFlags(A, D, CY);	break;								// ADC D
	case 0x8B: A += E + CY; RefreshFlags(A, E, CY);	break;								// ADC E
	case 0x8C: A += H + CY; RefreshFlags(A, H, CY);	break;								// ADC H
	case 0x8D: A += L + CY; RefreshFlags(A, L, CY);	break;								// ADC L
	case 0x8E: A += Memory[HL()] + CY; RefreshFlags(A, Memory[HL()], CY); break;		// ADC [HL]
	case 0x8F: A += A + CY; RefreshFlags(A, A, CY); break;								// ADC A

	// SUB r: A = A - r

	case 0x90: A -= B; RefreshFlags(A, B); break;										// SUB B
	case 0x91: A -= C; RefreshFlags(A, C); break;										// SUB C
	case 0x92: A -= D; RefreshFlags(A, D); break;										// SUB D
	case 0x93: A -= E; RefreshFlags(A, E); break;										// SUB E
	case 0x94: A -= H; RefreshFlags(A, H); break;										// SUB H
	case 0x95: A -= L; RefreshFlags(A, L); break;										// SUB L
	case 0x96: A -= Memory[HL()]; RefreshFlags(A, Memory[HL()]); break;					// SUB [HL]
	case 0x97: A -= A; RefreshFlags(A, A); break;										// SUB A

	// SBB r: A = A - r - CY

	case 0x98: A -= B - CY; RefreshFlags(A, B, CY); break;								// SBB B
	case 0x99: A -= C - CY; RefreshFlags(A, C, CY); break;								// SBB C
	case 0x9a: A -= D - CY; RefreshFlags(A, D, CY); break;								// SBB D
	case 0x9b: A -= E - CY; RefreshFlags(A, E, CY); break;								// SBB E
	case 0x9c: A -= H - CY; RefreshFlags(A, H, CY); break;								// SBB H
	case 0x9d: A -= L - CY; RefreshFlags(A, L, CY); break;								// SBB L
	case 0x9e: A -= Memory[HL()] - CY; RefreshFlags(A, Memory[HL()], CY); break;		// SBB [HL]
	case 0x9f: A -= A - CY; RefreshFlags(A, A, CY); break;								// SBB A

	// ANA r: A = A & r, CY = 0

	case 0xA0: A &= B; RefreshFlags(A, B, CPUOperation::And); CY = 0; break;					// ANA B
	case 0xA1: A &= C; RefreshFlags(A, C, CPUOperation::And); CY = 0; break;					// ANA C
	case 0xA2: A &= D; RefreshFlags(A, D, CPUOperation::And); CY = 0; break;					// ANA D
	case 0xA3: A &= E; RefreshFlags(A, E, CPUOperation::And); CY = 0; break;					// ANA E
	case 0xA4: A &= H; RefreshFlags(A, H, CPUOperation::And); CY = 0; break;					// ANA H
	case 0xA5: A &= L; RefreshFlags(A, L, CPUOperation::And); CY = 0; break;					// ANA L
	case 0xA6: A &= Memory[HL()]; RefreshFlags(A, Memory[HL()], CPUOperation::And); CY = 0; break;		// ANA [HL]
	case 0xA7: A &= A; RefreshFlags(A, A, CPUOperation::And); CY = 0; break;					// ANA A

	// XRA r: A = A ^ r, CY = AC = 0

	case 0xA8: A ^= B; RefreshFlags(A, B, CPUOperation::Xor); CY = AC = 0; break;				// XRA B
	case 0xA9: A ^= C; RefreshFlags(A, C, CPUOperation::Xor); CY = AC = 0; break;				// XRA C
	case 0xAA: A ^= D; RefreshFlags(A, D, CPUOperation::Xor); CY = AC = 0; break;				// XRA D
	case 0xAB: A ^= E; RefreshFlags(A, E, CPUOperation::Xor); CY = AC = 0; break;				// XRA E
	case 0xAC: A ^= H; RefreshFlags(A, H, CPUOperation::Xor); CY = AC = 0; break;				// XRA H
	case 0xAD: A ^= L; RefreshFlags(A, L, CPUOperation::Xor); CY = AC = 0; break;				// XRA L
	case 0xAE: A ^= Memory[HL()]; RefreshFlags(A, Memory[HL()], CPUOperation::Xor); CY = 0; break;			// XRA [HL]
	case 0xAF: A ^= A; RefreshFlags(A, A, CPUOperation::Xor); CY = 0; break;					// XRA A

	// ORA r: A = A | r, CY = AC = 0

	case 0xB0: A |= B; RefreshFlags(A, B, CPUOperation::Or); CY = AC = 0; break;				// ORA B
	case 0xB1: A |= C; RefreshFlags(A, C, CPUOperation::Or); CY = AC = 0; break;				// ORA C
	case 0xB2: A |= D; RefreshFlags(A, D, CPUOperation::Or); CY = AC = 0; break;				// ORA D
	case 0xB3: A |= E; RefreshFlags(A, E, CPUOperation::Or); CY = AC = 0; break;				// ORA E
	case 0xB4: A |= H; RefreshFlags(A, H, CPUOperation::Or); CY = AC = 0; break;				// ORA H
	case 0xB5: A |= L; RefreshFlags(A, L, CPUOperation::Or); CY = AC = 0; break;				// ORA L
	case 0xB6: A |= Memory[HL()]; RefreshFlags(A, Memory[HL()], CPUOperation::Or); CY = 0; break;			// ORA [HL]
	case 0xB7: A |= A; RefreshFlags(A, A, CPUOperation::Or); CY = 0; break;						// ORA A

	// CMP r: A - r

	case 0xB8: RefreshFlags(A, B, CPUOperation::Subtract); break;								// CMP B : A - B
	case 0xB9: RefreshFlags(A, C, CPUOperation::Subtract); break;								// CMP C : A - C
	case 0xBA: RefreshFlags(A, D, CPUOperation::Subtract); break;								// CMP D : A - D
	case 0xBB: RefreshFlags(A, E, CPUOperation::Subtract); break;								// CMP E : A - E
	case 0xBC: RefreshFlags(A, H, CPUOperation::Subtract); break;								// CMP H : A - H
	case 0xBD: RefreshFlags(A, L, CPUOperation::Subtract); break;								// CMP L : A - L
	case 0xBE: RefreshFlags(A, Memory[HL()], CPUOperation::Subtract); CY = 0; break;			// CMP [HL] : A - [HL]
	case 0xBF: RefreshFlags(A, A, CPUOperation::Subtract); CY = 0; break;						// CMP A

	case 0xC0: if (!Z) { PC = Memory[SP + 1] << 8 | Memory[SP]; SP += 2; } break;				// RNZ

	case 0xC1: POP(B, C); break;																// POP BC

	case 0xC2: PC += 2; if (!Z) PC = Opcode[2] << 8 | Opcode[1]; break;							// JNZ addr

	case 0xC3: PC = Opcode[2] << 8 | Opcode[1]; break;  										// JMP addr

	case 0xC4: if (!Z) { Memory[SP - 1] = PC >> 8; Memory[SP - 2] = PC << 8; SP -= 2; PC = Opcode[1] << 8 | Memory[PC]; } break;	// CNZ addr

	case 0xC5: PUSH(B, C); break;																// PUSH B : Push BC

	case 0xC6: A += Opcode[1]; RefreshFlags(A, Opcode[1]); PC += 1; break;						// ADI data

	case 0xC7: Memory[SP - 1] = PC >> 8; Memory[SP - 2] = PC << 8; SP -= 2; PC = 0; break;		// RST 0

	case 0xC8: if (Z) { PC = Memory[SP + 1] << 8 | Memory[SP]; SP += 2; } break;				// RZ

	case 0xC9: PC = Memory[SP + 1] << 8 | Memory[SP]; SP += 2; break;							// RET

	case 0xCA: PC += 2; if (Z) PC = Opcode[2] << 8 | Opcode[1]; break;							// JZ addr

	case 0xCB: UnimplementedInstructionDetected(Opcode); break;

	case 0xCC: if (Z) { Memory[SP - 1] = PC >> 8; Memory[SP - 2] = PC << 8; SP -= 2; PC = Opcode[1] << 8 | Memory[PC]; } break;		// CZ addr

	case 0xCD:
		CALL(Opcode);
		break;

	case 0xCE: A += Opcode[1] + CY; RefreshFlags(A, Opcode[1], CY); PC += 1; break;				// ACI data

	case 0xCF: Memory[SP - 1] = PC >> 8; Memory[SP - 2] = PC << 8; SP -= 2; PC = 0x0008; break;	// RST 1

	case 0xD0: if (!CY) { PC = Memory[SP + 1] << 8 | Memory[SP]; SP += 2; } break;				// RNC

	case 0xD1: E = Memory[SP]; D = Memory[SP + 1]; SP += 2; break;								// POP DE

	case 0xD2: PC += 2; if (!CY) PC = Opcode[2] << 8 | Opcode[1]; break;									// JNC addr

	case 0xD3: break; /*port is Opcode[1]*/ PC += 1;											// OUT port

	case 0xD4: if (!CY) { Memory[SP - 1] = PC >> 8; Memory[SP - 2] = PC << 8; SP -= 2; PC = Opcode[1] << 8 | Memory[PC]; } break;	// CNC addr

	case 0xD5: Memory[SP - 2] = E; Memory[SP - 1] = D; SP -= 2; break;							// PUSH DE

	case 0xD6: A -= Opcode[1]; RefreshFlags(A, Opcode[1]); PC += 1; break;			// SUI data

	case 0xD7: Memory[SP - 1] = PC >> 8; Memory[SP - 2] = PC << 8; SP -= 2; PC = 0x0010; break;	// RST 2

	case 0xD8: if (CY) { PC = Memory[SP + 1] << 8 | Memory[SP]; SP += 2; } break;				// RC

	case 0xD9: UnimplementedInstructionDetected(Opcode); break;

	case 0xDA: PC += 2; if (CY) PC = Opcode[2] << 8 | Opcode[1]; break;						// JC addr

	case 0xDB: UnimplementedInstructionDetected(Opcode); break;

	case 0xDC:																							// CC addr
		if (CY)
			CALL(Opcode);
		break;

	case 0xDD: UnimplementedInstructionDetected(Opcode); break;

	case 0xDF: Memory[SP - 1] = PC >> 8; Memory[SP - 2] = PC << 8; SP -= 2; PC = 0x0018; break;	// RST 3

	case 0xE0: if (!P) { PC = Memory[SP + 1] << 8 | Memory[SP]; SP += 2; } break;				// RPO

	case 0xE1: L = Memory[SP]; H = Memory[SP + 1]; SP += 2; break;								// POP HL

	case 0xE2: PC += 2; if (!P) PC = Opcode[2] << 8 | Opcode[1]; break;							// JPO addr

	case 0xE3: UnimplementedInstructionDetected(Opcode); break;

	case 0xE4:																					// CPO addr 
		if (!P)
			CALL(Opcode);
		break;

	case 0xE5: Memory[SP - 2] = L; Memory[SP - 1] = H; SP -= 2; break;							// PUSH HL

	case 0xE6: A = A & Opcode[1]; PC += 1; CY = AC = 0; break;									// ANI data8

	case 0xE7: Memory[SP - 1] = PC >> 8; Memory[SP - 2] = PC << 8; SP -= 2; PC = 0x0020; break;	// RST 4

	case 0xE8: if (P) { PC = Memory[SP + 2] << 8 | Memory[SP + 1]; SP += 2; } break;			// RPE

	case 0xE9: UnimplementedInstructionDetected(Opcode); break;

	case 0xEA: PC += 2; if (P) SP = Opcode[2] << 8 | Opcode[1]; break;									// JPE addr

	case 0xEB: Temp8 = H; H = D; D = Temp8; Temp8 = L; L = E; E = Temp8; break;					// XCHG : HL <-> DE

	case 0xEC:																					// CPE addr 
		if (P)
			CALL(Opcode);
		break;

	case 0xED: UnimplementedInstructionDetected(Opcode); break;

	case 0xEE: UnimplementedInstructionDetected(Opcode); break;

	case 0xEF: Memory[SP - 1] = PC >> 8; Memory[SP - 2] = PC << 8; SP -= 2; PC = 0x0028; break;	// RST 5

	case 0xF0: if (!S) { PC = Memory[SP + 1] << 8 | Memory[SP]; SP += 2; } break;				// RP

	case 0xF1: OverwriteFLAGS(Memory[SP]); A = Memory[SP + 1]; SP += 2; break;					// POP PSW

	case 0xF2: PC += 2; if (!S) PC = Opcode[2] << 8 | Opcode[1]; break;									// JP addr

	case 0xF3: InterruptsEnabled = false; break;												// DI

	case 0xF4: UnimplementedInstructionDetected(Opcode); break;									// CP addr : CALL if +ve

	case 0xF5: Memory[SP - 2] = GetFLAGS(); Memory[SP - 1] = A; SP -= 2; break;					// PUSH PSW

	case 0xF7: Memory[SP - 1] = PC >> 8; Memory[SP - 2] = PC << 8; SP -= 2; PC = 0x0030; break;	// RST 6

	case 0xF8: if (S) { PC = Memory[SP + 1] << 8 | Memory[SP]; SP += 2; } break;				// RM

	case 0xFA: PC += 2; if (S) PC = Opcode[2] << 8 | Opcode[1]; break;							// JM addr

	case 0xFB: InterruptsEnabled = true; break;													// EI

	case 0xFC:																					// CM addr
		if (S)
			CALL(Opcode);
		break;

	case 0xFD: UnimplementedInstructionDetected(Opcode); break;

	case 0xFE: RefreshFlags(A, Opcode[1], CPUOperation::Subtract); break;						// CPI data8

	case 0xFF: Memory[SP - 1] = PC >> 8; Memory[SP - 2] = PC << 8; SP -= 2; PC = 0x0038; break;	// RST 7

	default:
		break;
	}

	PrintRegisters();
}

void Intel8080::CALL(uint8_t* Opcode)
{
	PC += 2;	// CALL is 3 bytes instruction; skip next two bytes to point to next instruction.
	if (((Opcode[2] << 8) | Opcode[1]) == 5)
	{
		if (C == 9)
		{
			uint16_t offset = DE();
			uint8_t* str = &Memory[offset + 3];  //skip the prefix bytes
			while (*str != '$')
				std::cout << (char)(*(str++));
			//printf("%c", *str++);

			std::cout << "\n";
		}
		else if (C == 2)
			std::cout << "print char routine called\n";
	}
	else if (((Opcode[2] << 8) | Opcode[1]) == 0)
		exit(0);
	else
	{
		Memory[SP - 1] = (PC) >> 8 & 0xFF;
		Memory[SP - 2] = (PC) & 0xFF;
		SP -= 2;
		PC = Opcode[2] << 8 | Opcode[1];
	}
}

void Intel8080::PUSH(uint8_t Upper, uint8_t Lower)
{
	Memory[SP - 1] = Upper;
	Memory[SP - 2] = Lower;
	SP -= 2;
}

void Intel8080::POP(uint8_t& Upper, uint8_t& Lower)
{
	Upper = Memory[SP + 1];
	Lower = Memory[SP];
	SP += 2;
}

void Intel8080::RefreshFlags(uint8_t Operand1, uint8_t Operand2, CPUOperation Operation)
{
	uint16_t Result = 0;
	switch (Operation)
	{
	case CPUOperation::Add:			Result = (uint16_t)Operand1 + (uint16_t)Operand2; break;
	case CPUOperation::Subtract:
		Operand2 = Get2sComp(Operand2);
		Result = (uint16_t)Operand1 + (uint16_t)Operand2;
		break;
	case CPUOperation::And:			Result = (uint16_t)Operand1 & (uint16_t)Operand2; break;
	case CPUOperation::Xor:			Result = (uint16_t)Operand1 ^ (uint16_t)Operand2; break;
	}

	Z = (Result & 0xFF) == 0;
	//if (Operation == CPUOperation::Subtract && Operand1 == Operand2) Z = 1;					// Set Z to 1 if both operands are same.


	S = (Result & 0x0080) != 0;							// Result AND 0x0080 will discard all bits except MSB of second nibble from right.

	P = CalculateParity(Result & 0xFF);

	CY = (Result > 0xFF);
	if (Operation == CPUOperation::Subtract) CY = ~CY;					// Invert carry flag in case of subtraction.

	AC = ((Operand1 & 0xF) + (Operand2 & 0xF)) > 0xF;					// If there's a carry from D3 to D4, set AC, else reset AC.
	if (Operation == CPUOperation::Subtract) AC = ~AC;					// Invert aux. carry in case of subtraction??
}

void Intel8080::RefreshFlags(uint8_t Operand1, uint8_t Operand2, uint8_t Operand3, CPUOperation Operation)
{
	uint16_t Result;
	switch (Operation)
	{
	case CPUOperation::Add: Result = (uint16_t)Operand1 + (uint16_t)Operand2 + (uint16_t)Operand3; break;
	case CPUOperation::Subtract: (uint16_t)Operand1 - (uint16_t)Operand2; break;

	default: break;
	}
	Z = (Result & 0xFF) == 0;
	S = (Result & 0x0080) != 0;							// Result AND 0x0080 will discard all bits except MSB of second nibble from right.
	P = CalculateParity(Result & 0xFF);
	CY = (Result > 0xFF);
	AC = ((Operand1 & 0xF) + (Operand2 & 0xF) + (Operand3 & 0xF)) > 0xF;					// If there's a carry from D3 to D4, set AC, else reset AC.
}

void Intel8080::OverwriteFLAGS(uint8_t Value)
{
	S = (Value & 0x80) == 0x80;		// Bit 7
	Z = (Value & 0x40) == 0x40;		// Bit 6
	AC = (Value & 0x10) == 0x10;		// Bit 4
	P = (Value & 0x4) == 0x4;			// Bit 2
	CY = (Value & 0x1) == 0x1;			// Bit 0
}

int Intel8080::CalculateParity(uint8_t Value)
{
	Value ^= Value >> 4;
	Value &= 0xf;
	return !((0x6996 >> Value) & 1);
}

uint16_t Intel8080::GetRegAddr(uint8_t RegA, uint8_t RegB)
{
	return RegA * 0x100 + RegB;
}

void Intel8080::PrintRegisters()
{
	using namespace	std;
	cout << "A = " << +A << "\n";
	cout << "B = " << +B << "\n";
	cout << "C = " << +C << "\n";
	cout << "D = " << +D << "\n";
	cout << "E = " << +E << "\n";
	cout << "H = " << +H << "\n";
	cout << "L = " << +L << "\n";

	cout << "Z = " << Z << ", S = " << S << ", P = " << P << ", CY = " << CY << ", AC = " << AC << "\n";
}