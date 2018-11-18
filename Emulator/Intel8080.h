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

#pragma	region General Purpose Registers

	uint8_t A;
	uint8_t B;
	uint8_t C;
	uint8_t D;
	uint8_t E;
	uint8_t H;
	uint8_t L;

#pragma endregion

#pragma region Register Pairs

	uint16_t BC() { return B << 8 | C; }

	uint16_t DE() { return D << 8 | E; }

	uint16_t HL() { return H << 8 | L; }

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

			case 0x88:																			// ADC B

			case 0xFF: break;																	//

			default:
				break;
		}

		PC += 1;		// Advance to next byte.
	}

	void RefreshFlags(uint8_t Operand1, uint8_t Operand2)
	{
		uint16_t Result = (uint16_t)Operand1 + (uint16_t)Operand2;
		Z = (Result & 0xFF) == 0;
		S = (Result & 0x0080) != 0;							// Result AND 0x0080 will discard all bits except MSB of second nibble from right.
		P = CalculateParity(Result & 0xFF);
		CY = (Result > 0xFF);
		AC = ((Operand1 & 0xF) + (Operand2 & 0xF)) > 0xF;					// If there's a carry from D3 to D4, set AC, else reset AC.
	}

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

};