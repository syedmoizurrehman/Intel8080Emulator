#pragma once

#include <iostream>

using namespace std;

struct EightBits
{
	uint8_t D7 : 1;
	uint8_t D6 : 1;
	uint8_t D5 : 1;
	uint8_t D4 : 1;
	uint8_t D3 : 1;
	uint8_t D2 : 1;
	uint8_t D1 : 1;
	uint8_t D0 : 1;
};

struct SixteenBits
{
	uint8_t D15 : 1;
	uint8_t D14 : 1;
	uint8_t D13 : 1;
	uint8_t D12 : 1;
	uint8_t D11 : 1;
	uint8_t D10 : 1;
	uint8_t D9 : 1;
	uint8_t D8 : 1;
	uint8_t D7 : 1;
	uint8_t D6 : 1;
	uint8_t D5 : 1;
	uint8_t D4 : 1;
	uint8_t D3 : 1;
	uint8_t D2 : 1;
	uint8_t D1 : 1;
	uint8_t D0 : 1;
};

union Reg16
{
	uint8_t Value;
	SixteenBits Bits;

	// Read-only
	uint8_t operator[](int Index)
	{
		switch (Index)
		{
			case 0:		return Bits.D0;
			case 1:		return Bits.D1;
			case 2:		return Bits.D2;
			case 3:		return Bits.D3;
			case 4:		return Bits.D4;
			case 5:		return Bits.D5;
			case 6:		return Bits.D6;
			case 7:		return Bits.D7;
			case 8:		return Bits.D8;
			case 9:		return Bits.D9;
			case 10:	return Bits.D10;
			case 11:	return Bits.D11;
			case 12:	return Bits.D12;
			case 13:	return Bits.D13;
			case 14:	return Bits.D14;
			case 15:	return Bits.D15;

			default:
				throw exception("Index out of bound.");
		}
	}

	void Set(uint8_t Index, uint8_t Value)
	{
		switch (Index)
		{
			case 0: Bits.D0 = Value; break;
			case 1: Bits.D1 = Value; break;
			case 2: Bits.D2 = Value; break;
			case 3: Bits.D3 = Value; break;
			case 4: Bits.D4 = Value; break;
			case 5: Bits.D5 = Value; break;
			case 6: Bits.D6 = Value; break;
			case 7: Bits.D7 = Value; break;

			default:
				throw exception("Index out of bound");
		}
	}
};

union Reg8
{
	uint8_t Value;
	EightBits Bits;

	Reg8() { }

	Reg8(uint8_t value) { Value = value; }

	// Read-only
	uint8_t operator[](int Index)
	{
		switch (Index)
		{
			case 0:	return Bits.D0;
			case 1:	return Bits.D1;
			case 2:	return Bits.D2;
			case 3:	return Bits.D3;
			case 4:	return Bits.D4;
			case 5:	return Bits.D5;
			case 6:	return Bits.D6;
			case 7:	return Bits.D7;

			default:
				throw exception("Index out of bound.");
		}
	}

	// Implicit converter for Reg8 to uint8_t
	operator Reg8() { return Value; }

	void operator=(Reg8 Rhs) { Value = Rhs.Value; }

	void Set(uint8_t Index, uint8_t Value)
	{
		switch (Index)
		{
			case 0: Bits.D0 = Value; break;
			case 1: Bits.D1 = Value; break;
			case 2: Bits.D2 = Value; break;
			case 3: Bits.D3 = Value; break;
			case 4: Bits.D4 = Value; break;
			case 5: Bits.D5 = Value; break;
			case 6: Bits.D6 = Value; break;
			case 7: Bits.D7 = Value; break;

			default:
				throw exception("Index out of bound");
		}
	}
};

class Intel8080
{
public:

	enum CPUOperation
	{
		Add, Subtract, And, Or, Not, Xor
	};

#pragma	region General Purpose Registers

	uint8_t A;
	uint8_t B;
	uint8_t C;
	uint8_t D;
	uint8_t E;
	uint8_t H;
	uint8_t L;

#pragma endregion

#pragma region Register Pairs	// Fetch the byte addressed by register pairs.

	uint8_t BC() { return B << 8 | C; }

	uint8_t DE() { return D << 8 | E; }

	uint8_t HL() { return H << 8 | L; }

#pragma endregion

#pragma region Special Purpose Registers

	uint16_t SP;				// SP is 16-bit
	uint16_t PC;				// PC is 16-bit

#pragma endregion

	// Memory

	uint8_t Memory[0x10000];	// Byte array to represent memory/RAM. Since address bus' width is 16-bit, memory can only consist of 2^16 bytes.

#pragma region Condition Flags

	// Zero Flag
	unsigned Z : 1;

	// Sign Flag
	unsigned S : 1;

	// Parity Flag
	unsigned P : 1;

	// Carry Flag
	unsigned CY : 1;

	// Auxiliary Carry Flag
	unsigned AC : 1;

#pragma endregion

	bool InterruptsEnabled;

	Intel8080()
	{
		SP = PC = 0;
		Z = S = P = CY = AC = 0;
	}

	void Execute()
	{
		//unsigned char* Opcode = &CPU->Memory[CPU->PC];
		uint8_t Opcode = Memory[PC];

		switch (Opcode)
		{
			case 0x00: break;																	// NOP

			case 0x01: break;																	// LXI...

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

			case 0x76: break;																	// HLT

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

			case 0xA0: A &= B; RefreshFlags(A, B, CPUOperation::And); CY = 0; break;			// ANA B
			case 0xA1: A &= C; RefreshFlags(A, C, CPUOperation::And); CY = 0; break;			// ANA C
			case 0xA2: A &= D; RefreshFlags(A, D, CPUOperation::And); CY = 0; break;			// ANA D
			case 0xA3: A &= E; RefreshFlags(A, E, CPUOperation::And); CY = 0; break;			// ANA E
			case 0xA4: A &= H; RefreshFlags(A, H, CPUOperation::And); CY = 0; break;			// ANA H
			case 0xA5: A &= L; RefreshFlags(A, L, CPUOperation::And); CY = 0; break;			// ANA L
			case 0xA6: A &= Memory[HL()]; RefreshFlags(A, Memory[HL()], CPUOperation::And); CY = 0; break;		// ANA [HL]
			case 0xA7: A &= A; RefreshFlags(A, A, CPUOperation::And); CY = 0; break;			// ANA A

			// XRA r: A = A ^ r, CY = AC = 0
			
			case 0xA8: A ^= B; RefreshFlags(A, B, CPUOperation::Xor); CY = AC = 0; break;			// XRA B
			case 0xA9: A ^= C; RefreshFlags(A, C, CPUOperation::Xor); CY = AC = 0; break;			// XRA C
			case 0xAA: A ^= D; RefreshFlags(A, D, CPUOperation::Xor); CY = AC = 0; break;			// XRA D
			case 0xAB: A ^= E; RefreshFlags(A, E, CPUOperation::Xor); CY = AC = 0; break;			// XRA E
			case 0xAC: A ^= H; RefreshFlags(A, H, CPUOperation::Xor); CY = AC = 0; break;			// XRA H
			case 0xAD: A ^= L; RefreshFlags(A, L, CPUOperation::Xor); CY = AC = 0; break;			// XRA L
			case 0xAE: A ^= Memory[HL()]; RefreshFlags(A, Memory[HL()], CPUOperation::Xor); CY = 0; break;			// XRA [HL]
			case 0xAF: A ^= A; RefreshFlags(A, A, CPUOperation::Xor); CY = 0; break;				// XRA A

			// ORA r: A = A | r, CY = AC = 0
			
			case 0xB0: A |= B; RefreshFlags(A, B, CPUOperation::Or); CY = AC = 0; break;			// ORA B
			case 0xB1: A |= C; RefreshFlags(A, C, CPUOperation::Or); CY = AC = 0; break;			// ORA C
			case 0xB2: A |= D; RefreshFlags(A, D, CPUOperation::Or); CY = AC = 0; break;			// ORA D
			case 0xB3: A |= E; RefreshFlags(A, E, CPUOperation::Or); CY = AC = 0; break;			// ORA E
			case 0xB4: A |= H; RefreshFlags(A, H, CPUOperation::Or); CY = AC = 0; break;			// ORA H
			case 0xB5: A |= L; RefreshFlags(A, L, CPUOperation::Or); CY = AC = 0; break;			// ORA L
			case 0xB6: A |= Memory[HL()]; RefreshFlags(A, Memory[HL()], CPUOperation::Or); CY = 0; break;			// ORA [HL]
			case 0xB7: A |= A; RefreshFlags(A, A, CPUOperation::Or); CY = 0; break;					// ORA A

			// CMP r: A - r

			case 0xB8: RefreshFlags(A, B, CPUOperation::Subtract); break;							// CMP B
			case 0xB9: RefreshFlags(A, C, CPUOperation::Subtract); break;							// CMP C
			case 0xBA: RefreshFlags(A, D, CPUOperation::Subtract); break;							// CMP D
			case 0xBB: RefreshFlags(A, E, CPUOperation::Subtract); break;							// CMP E
			case 0xBC: RefreshFlags(A, H, CPUOperation::Subtract); break;							// CMP H
			case 0xBD: RefreshFlags(A, L, CPUOperation::Subtract); break;							// CMP L
			case 0xBE: RefreshFlags(A, Memory[HL()], CPUOperation::Subtract); CY = 0; break;		// CMP [HL]
			case 0xBF: RefreshFlags(A, A, CPUOperation::Subtract); CY = 0; break;					// CMP A
			
			case 0xC0: if (!Z) {PC = Memory[SP + 1] << 8 | Memory[SP]; SP += 2;} break;				// RNZ

			case 0xC1: C = Memory[SP]; B = Memory[SP + 1]; SP += 2; break;							// POP BC

			case 0xC2: if (!Z) PC = Memory[PC + 1] << 8 | Memory[PC]; break;						// JNZ addr

			case 0xC3: PC = Memory[PC + 1] << 8 | Memory[PC]; break;  								// JMP addr

			case 0xC4: if (!Z) {Memory[SP - 1] = PC >> 8; Memory[SP - 2] = PC << 8; SP -= 2; PC = Memory[PC + 1] << 8 | Memory[PC];} break;	// CNZ addr

			case 0xC5: Memory[SP - 2] = C; Memory[SP - 1] = B; SP -= 2; break;						// PUSH BC

			case 0xC6: A += Memory[PC + 1]; RefreshFlags(A, Memory[PC + 1]); PC += 1; break;		// ADI data

			case 0xC7: Memory[SP - 1] = PC >> 8; Memory[SP - 2] = PC << 8; SP -= 2; PC = 0; break;	// RST 0

			case 0xC8: if (Z) { PC = Memory[SP + 1] << 8 | Memory[SP]; SP += 2; } break;			// RZ

			case 0xC9: PC = Memory[SP + 1] << 8 | Memory[SP]; SP += 2; break;						// RET

			case 0xCA: if (Z) PC = Memory[PC + 1] << 8 | Memory[PC]; break;							// JZ addr

			case 0xCB: break;

			case 0xCC: if (Z) {Memory[SP - 1] = PC >> 8; Memory[SP - 2] = PC << 8; SP -= 2; PC = Memory[PC + 1] << 8 | Memory[PC];} break;		// CZ addr

			case 0xCD: Memory[SP - 1] = PC >> 8; Memory[SP - 2] = PC << 8; SP -= 2; PC = Memory[PC + 1] << 8 | Memory[PC]; break; 			// CALL addr

			case 0xCE: A += Memory[PC + 1] + CY; RefreshFlags(A, Memory[PC + 1], CY); PC += 1; break;// ACI data

			case 0xCF: Memory[SP - 1] = PC >> 8; Memory[SP - 2] = PC << 8; SP -= 2; PC = 0x0008; break;	// RST 1

			case 0xD0: if (!CY) { PC = Memory[SP + 1] << 8 | Memory[SP]; SP += 2; } break;			// RNC

			case 0xD1: E = Memory[SP]; D = Memory[SP + 1]; SP += 2; break;							// POP DE

			case 0xD2: if (!CY) PC = Memory[PC + 1] << 8 | Memory[PC]; break;						// JNC addr

			case 0xD3: break; /*port is Memory[PC + 1]*/ PC += 1;									// OUT port

			case 0xD4: if (!CY) {Memory[SP - 1] = PC >> 8; Memory[SP - 2] = PC << 8; SP -= 2; PC = Memory[PC + 1] << 8 | Memory[PC];} break;	// CNC addr

			case 0xD5: Memory[SP - 2] = E; Memory[SP - 1] = D; SP -= 2; break;						// PUSH DE

			case 0xD6: A -= Memory[PC + 1]; RefreshFlags(A, Memory[PC + 1]); PC += 1; break;		// SUI data

			case 0xD7: Memory[SP - 1] = PC >> 8; Memory[SP - 2] = PC << 8; SP -= 2; PC = 0x0010; break;	// RST 2

			case 0xD8: if (CY) { PC = Memory[SP + 1] << 8 | Memory[SP]; SP += 2; } break;			// RC

			case 0xD9: break;

			case 0xDA: if (CY) PC = Memory[PC + 1] << 8 | Memory[PC]; break;						// JC addr

			case 0xDB: break;

			case 0xDC: if (CY) {Memory[SP - 1] = PC >> 8; Memory[SP - 2] = PC << 8; SP -= 2; PC = Memory[PC + 1] << 8 | Memory[PC];} break;	// CC addr

			case 0xDF: Memory[SP - 1] = PC >> 8; Memory[SP - 2] = PC << 8; SP -= 2; PC = 0x0018; break;	// RST 3

			case 0xE0: if (!P) { PC = Memory[SP + 1] << 8 | Memory[SP]; SP += 2; } break;			// RPO

			case 0xE1: L = Memory[SP]; H = Memory[SP + 1]; SP += 2; break;							// POP HL

			case 0xE2: if (!P) PC = Memory[PC + 1] << 8 | Memory[PC]; break;						// JPO addr

			case 0xE3: break;

			case 0xE4: if (!P) {Memory[SP - 1] = PC >> 8; Memory[SP - 2] = PC << 8; SP -= 2; PC = Memory[PC + 1] << 8 | Memory[PC];} break;	// CPO addr

			case 0xE5: Memory[SP - 2] = L; Memory[SP - 1] = H; SP -= 2; break;						// PUSH HL

			case 0xE6: break;

			case 0xE7: Memory[SP - 1] = PC >> 8; Memory[SP - 2] = PC << 8; SP -= 2; PC = 0x0020; break;	// RST 4

			case 0xE8: if (P) { PC = Memory[SP + 1] << 8 | Memory[SP]; SP += 2; } break;			// RPE

			case 0xE9: break;

			case 0xEA: if (P) SP = Memory[PC + 1] << 8 | Memory[PC]; break;							// JPE addr

			case 0xEB: break;

			case 0xEC: if (P) {Memory[SP - 1] = PC >> 8; Memory[SP - 2] = PC << 8; SP -= 2; PC = Memory[PC + 1] << 8 | Memory[PC];} break;		// CPE addr

			case 0xED: break;

			case 0xEF: Memory[SP - 1] = PC >> 8; Memory[SP - 2] = PC << 8; SP -= 2; PC = 0x0028; break;	// RST 5

			case 0xF0: if (!S) { PC = Memory[SP + 1] << 8 | Memory[SP]; SP += 2; } break;			// RP

			case 0xF1: OverwriteFLAGS(Memory[SP]); A = Memory[SP + 1]; SP += 2; break;				// POP PSW

			case 0xF2: if (!S) PC = Memory[PC + 1] << 8 | Memory[PC]; break;						// JP addr

			case 0xF3: InterruptsEnabled = false; break;											// DI

			case 0xF4: if (!S) {Memory[SP - 1] = PC >> 8; Memory[SP - 2] = PC << 8; SP -= 2; PC = Memory[PC + 1] << 8 | Memory[PC];} break;	// CP addr

			case 0xF5: Memory[SP - 2] = GetFLAGS(); Memory[SP - 1] = A; SP -= 2; break;				// PUSH PSW

			case 0xF7: Memory[SP - 1] = PC >> 8; Memory[SP - 2] = PC << 8; SP -= 2; PC = 0x0030; break;	// RST 6

			case 0xF8: if (S) { PC = Memory[SP + 1] << 8 | Memory[SP]; SP += 2; } break;			// RM

			case 0xFA: if (S) PC = Memory[PC + 1] << 8 | Memory[PC]; break;							// JM addr

			case 0xFB: InterruptsEnabled = true; break;												// EI

			case 0xFC: if (S) {Memory[SP - 1] = PC >> 8; Memory[SP - 2] = PC << 8; SP -= 2; PC = Memory[PC + 1] << 8 | Memory[PC];} break;		// CM addr

			case 0xFF: Memory[SP - 1] = PC >> 8; Memory[SP - 2] = PC << 8; SP -= 2; PC = 0x0038; break;	// RST 7

			default:
				break;
		}

		PC += 1;		// Advance to next byte.
	}

	// Overload for 2 operands.
	void RefreshFlags(uint8_t Operand1, uint8_t Operand2, CPUOperation Operation = CPUOperation::Add)
	{
		uint16_t Result = 0;
		switch (Operation)
		{
			case CPUOperation::Add:			Result = (uint16_t)Operand1 + (uint16_t)Operand2; break;
			case CPUOperation::Subtract:	Operand2 = Get2sComp(Operand2); Result = (uint16_t)Operand1 + (uint16_t)Operand2; break;
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

	// Overload for 3 operands.
	void RefreshFlags(uint8_t Operand1, uint8_t Operand2, uint8_t Operand3, CPUOperation Operation = CPUOperation::Add)
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

	uint8_t GetFLAGS(){ return (((((((S << 1 | Z) << 1 | 0) << 1 | AC) << 1 | 0) << 1 | P) << 1 | 1) << 1 | CY); }

	void OverwriteFLAGS(uint8_t Value)
	{
		S = Value & 0x80 == 0x80;		// Bit 7
		Z = Value & 0x40 == 0x40;		// Bit 6
		AC = Value & 0x10 == 0x10;		// Bit 4
		P = Value & 0x4 == 0x4;			// Bit 2
		C =	Value &	0x1 == 0x1;			// Bit 0
	}

	uint8_t Get2sComp(uint8_t Operand) { return (~Operand) + 1; }

	int CalculateParity(uint8_t Value)
	{
		Value ^= Value >> 4;
		Value &= 0xf;
		return !((0x6996 >> Value) & 1);
	}

	// Concatenates contents of RegA and RegB with RegA being higher order and RegB being lower order.
	uint16_t GetRegAddr(uint8_t RegA, uint8_t RegB)
	{
		return RegA * 0x100 + RegB;
	}

	void Test()
	{
	}
};