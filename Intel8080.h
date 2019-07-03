#pragma once

#include <array>

class Intel8080
{
public:

	enum CPUOperation
	{
		Add, Subtract, And, Or, Not, Xor
	};

#pragma	region General Purpose Registers

	// 111
	uint8_t A;

	// 000
	uint8_t B;

	// 001
	uint8_t C;

	// 010
	uint8_t D;

	// 011
	uint8_t E;

	// 100
	uint8_t H;

	// 101
	uint8_t L;

#pragma endregion

#pragma region Register Pairs	// Fetch the byte addressed by register pairs.

	uint16_t BC() { return B << 8 | C; }

	uint16_t DE() { return D << 8 | E; }

	uint16_t HL() { return H << 8 | L; }

#pragma endregion

#pragma region Special Purpose Registers

	uint16_t SP;				// SP is 16-bit
	uint16_t PC;				// PC is 16-bit, Contains address of NEXT instruction to be executed.

#pragma endregion

	// Memory

	//uint8_t Memory[0x10000];	// Byte array to represent memory/RAM. Since address bus' width is 16-bit, memory can only consist of 2^16 bytes.
	std::array<uint8_t, 0x10000> Memory{};	// Byte array to represent memory/RAM. Since address bus' width is 16-bit, memory can only consist of 2^16 bytes.

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

	Intel8080();

	void Execute();

	void ExecuteInstruction();

	void CALL(uint8_t* Opcode);

	void PUSH(uint8_t Upper, uint8_t Lower);

	void POP(uint8_t& Upper, uint8_t& Lower);

	// Overload for 2 operands.
	void RefreshFlags(uint8_t Operand1, uint8_t Operand2, CPUOperation Operation = CPUOperation::Add);

	// Overload for 3 operands.
	void RefreshFlags(uint8_t Operand1, uint8_t Operand2, uint8_t Operand3, CPUOperation Operation = CPUOperation::Add);

	uint8_t GetFLAGS() { return (((((((S << 1 | Z) << 1 | 0) << 1 | AC) << 1 | 0) << 1 | P) << 1 | 1) << 1 | CY); }

	// Assigns each flag corresponding bit from the Value sent as parameter.
	void OverwriteFLAGS(uint8_t Value);

	uint8_t Get2sComp(uint8_t Operand) { return (~Operand) + 1; }

	int CalculateParity(uint8_t Value);

	// Concatenates contents of RegA and RegB with RegA being higher order and RegB being lower order.
	uint16_t GetRegAddr(uint8_t RegA, uint8_t RegB);

	void UnimplementedInstructionDetected(uint8_t* Opcode)
	{
		exit(1);
	}

	void PrintRegisters();

};