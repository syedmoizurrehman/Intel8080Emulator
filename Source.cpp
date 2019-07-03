#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <fstream>
#include <strstream>
#include <string>
#include <sstream>
#include <iomanip>
#include <bitset>
#include <vector>
#include "Intel8080.h"

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

vector<char> ReadObjectFile(string Filename)
{
	fstream::pos_type size;
	char* FileContent = nullptr;
	string FilePath = "Tests\\Loop Test\\";
	FilePath.append(Filename);
	fstream BinaryFile(FilePath.c_str(), ios::in | ios::binary | ios::ate);
	if (BinaryFile.is_open())
	{
		size = BinaryFile.tellg();
		FileContent = new char[size];
		BinaryFile.seekg(0, ios::beg);
		BinaryFile.read(FileContent, size);
		BinaryFile.close();

		vector<char> ReturnObject(size);
		for (size_t i = 0; i < size; i++)
			ReturnObject[i] = FileContent[i];

		return ReturnObject;
		//cout << hex << uppercase << fixed << setw(4) << setfill('0') << (int)code[4] << '\n';
		//cout << setw(4) << setfill('0') << (int)code[2];
	}
}

int main()
{
	Intel8080* CPU = new Intel8080;

	//CPU->Memory = new uint8_t[0x10000];

	vector<char> Temp = ReadObjectFile("LoopTest.bin");
	int i = 0;
	for (auto T : Temp)
		CPU->Memory[i++] = T;
	/*CPU->Memory[0] = 0xC3;	// JMP 0x1000
	CPU->Memory[1] = 0x00;
	CPU->Memory[2] = 0x10;


	// Fix the stack pointer from 0x6ad to 0x7ad
	// this 0x06 byte 112 in the code, which is
	// byte 112 + 0x100 = 368 in memory
	CPU->Memory[368] = 0x7;

	//Skip DAA test
	CPU->Memory[0x59C] = 0xc3;	//JMP 0x05C2
	CPU->Memory[0x59D] = 0xc2;
	CPU->Memory[0x59E] = 0x05;*/

	//CPU->Memory = new uint8_t[0x10000];
	////for (size_t i = 0; i < 0x10000; ++i)
	//	//CPU->Memory[i] = 0;
	//
	//CPU->Memory[0] = 00;	// NOP
	//
	//CPU->Memory[1] = 0xCD;	// CALL
	//	CPU->Memory[2] = 0x05;	// CALL
	//	CPU->Memory[3] = 0x00;	// CALL
	//
	//CPU->Memory[4] = 0x76;	// HLT
	//
	//CPU->Memory[5] = 0x00;	// SUBROUTINE NOP
	//CPU->Memory[6] = 0x00;	// SUBROUTINE NOP
	//CPU->Memory[7] = 0x00;	// SUBROUTINE NOP
	//CPU->Memory[8] = 0xC9;	// SUBROUTINE: ret
	//
	CPU->Execute();
}