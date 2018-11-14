#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <fstream>
#include <strstream>
#include <string>
#include <sstream>
#include <iomanip>

using namespace std;

string ToHex(const string& s, bool upper_case = true)
{
	ostringstream ret;

	for (string::size_type i = 0; i < s.length(); ++i)
	{
		int z = s[i] & 0xff;
		ret << std::hex << std::setfill('0') << std::setw(2) << (upper_case ? std::uppercase : std::nouppercase) << z;
		ret.put(' ');
	}

	return ret.str();
}

struct Intel8080
{
	uint16_t SP;			// SP is 16-bit
	uint16_t PC;			// PC is 16-bit
	uint8_t Memory[65536];	// 8-bit array to represent memory/RAM. Since address bus' width is 16-bit, memory can only consist of 2^16 bytes.
}* CPU = new Intel8080;

void Execute()
{
	//unsigned char* Opcode = &CPU->Memory[CPU->PC];
	uint8_t Opcode = CPU->Memory[CPU->PC];

	switch (Opcode)
	{

		case 0x00:		// NOP
			break;

		case 0x01:		//
			break;

		case 0xFF:		//
			break;

		default:
			break;
	}

	CPU->PC += 1;		// Advance to next byte.

}

int Disassemble8080Opcode(unsigned char *codebuffer, int pc)
{
	unsigned char *code = &codebuffer[pc];
	int opbytes = 1;
	printf("%04x ", pc);
	switch (*code)
	{
		case 0x00: printf("NOP"); break;
		case 0x01: printf("LXI    B,#$%02x%02x", code[2], code[1]); opbytes = 3; break;
		case 0x02: printf("STAX   B"); break;
		case 0x03: printf("INX    B"); break;
		case 0x04: printf("INR    B"); break;
		case 0x05: printf("DCR    B"); break;
		case 0x06: printf("MVI    B,#$%02x", code[1]); opbytes = 2; break;
		case 0x07: printf("RLC"); break;
		case 0x08: printf("NOP"); break;
			/* ........ */
		case 0x3e: printf("MVI    A,#0x%02x", code[1]); opbytes = 2; break;
			/* ........ */
		case 0xc3: printf("JMP    $%02x%02x", code[2], code[1]); opbytes = 3; break;
		/* ........ */
	}

	printf("\n");

	return opbytes;
}

int main()
{
	fstream::pos_type size;
	unsigned char * InstructionMemory;

	fstream BinaryFile("C:\\Users\\Administrative\\Source\\Repos\\Intel8080Emulator\\Rom Files\\invaders.h", ios::in | ios::binary | ios::ate);
	if (BinaryFile.is_open())
	{
		size = BinaryFile.tellg();
		InstructionMemory = new unsigned char[size];
		BinaryFile.seekg(0, ios::beg);
		BinaryFile.read((char*)InstructionMemory, size);
		BinaryFile.close();

		unsigned char* code = InstructionMemory;
		//printf("MVI    B,#$%02x", code[4]);
		cout << hex << uppercase << fixed << setw(4) << setfill('0') << (int)code[4] << '\n';
		cout << setw(4) << setfill('0') << (int)code[2];
	}

}