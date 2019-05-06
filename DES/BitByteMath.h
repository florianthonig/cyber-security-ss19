#define DEBUG
#ifdef DEBUG
#include <iostream>
#endif

#include <string>
#include <cstring>

namespace Macros
{
	#define Firstbit(x) (x&1l)	//Returns the first bit
	#define GetNthBit(x,n) ((x&(1l<<n))>>n) //Returns the nth bit

	#define AND(x,y) (x&y) 						//Bitwise and
	#define NAND(x,y) (NOT(AND(x,y))) 			//Bitwise nand
	#define OR(x,y) (x|y)						//Bitwise or
	#define NOR(x,y) (NOT(OR(x,y)))				//Bitwise nor
	#define XOR(x,y) (x^y)						//Bitwise xor
	#define NOT(x) (~x)							//Bitwise not
	#define CHARTOBIT(x) (if(ISBIT(x)){x-'0'}else{x})		//Takes a char and return either 0 or 1
	#define ISBIT(x) (x==0||x==1||x=='0'||x=='1')	//Checks if the given value/symbol is a bit
}

//Defines an 8 bit wide Byte (will be printed as a character)
typedef unsigned char Byte;
//Defines a 64 bit wide datatype
typedef unsigned long long BigInt;

//Defines unsigned value types
namespace UnsignedValues
{
	//typedef Byte Word[2];
	typedef unsigned short Word;
	//typedef Byte Doubleword[4];
	typedef unsigned int Doubleword;
	//typedef Byte Quadrupelword[8];
	typedef unsigned long long Quadrupelword;

	//Defines a single bit state
	//typedef enum Bit{Low=0,High=1};

	//Alternative: Defines a single bit state
	//typedef enum Bit{zero=0,one=1};

	namespace AlternativeUnsigned
	{
		typedef unsigned short ushort;
		typedef unsigned int uint;
		typedef unsigned long ulong;
		typedef unsigned long long ulonglong;
	}


}

//Defines value types
namespace SignedValues
{
	//Defines an 8 bit wide signed Byte (will be printed as a character)
	typedef char sByte;
	typedef short sWord;
	typedef int sDoubleword;
	typedef long long sQuadrupelword;
}

namespace Parser
{
	//This method is a wrapper for BinToDecRightToLeft
	BigInt Parse(char*);
	//This method is a wrapper for BinToDecRightToLeft
	BigInt Parse(std::string);

	//Converts binary symbols to a decimal value (big endian)
	BigInt BinToDecLeftToRight(char*);
	//Converts binary symbols to a decimal value (small endian)
	BigInt BinToDecRightToLeft(char*);
	//Converts binary symbols to a decimal value (big endian)
	BigInt BinToDecLeftToRight(std::string);
	//Converts binary symbols to a decimal value (small endian)
	BigInt BinToDecRightToLeft(std::string);

	//Converts binary symbols to a decimal value (small endian only)
	Byte ParseToByte(char*);
	//Converts binary symbols to a decimal value (small endian only)
	Byte ParseToByte(std::string);

	//Converts a byte value to a hex value
	unsigned char BinToHex(Byte c);
	//Converts a hex value to a byte
	Byte HexToBin(unsigned char c);
	//Converts a given bitstring to a hex symbol
	char BitStringToHex(std::string);

	//Parses 16 symbols to 8 values
	void ParseSymbolsToByte(std::string, Byte b[8]);
	//Takes 8 bytes and stores them as 64 bits
	void ParseBytesToBits(Byte[8], Byte [64]);
	//Takes 8 symbols and stores them as 64 bits
	void ParseSymbolsToBits(std::string, Byte [64]);
}

namespace BitOperations
{
	//Gets a certain bit of a given byte (starting at 0)
	short GetBit(Byte, short);
	//Sets a certain bit of a given byte (starting at 0)
	Byte SetBit(Byte, short, short);
	//Sets a certain bit of a given byte (starting at 0)
	void SetBitPointer(Byte*, short, short);
}

namespace Bitstrings
{
	//Returns a bit string (e.g. 0000 0001 from 1)
	std::string GetBitString(Byte);
	//Returns a bit string (e.g. 0000 0001 from 1)
	std::string GetBitString(unsigned int);
	//Returns a bit string (e.g. 0000 0001 from 1)
	std::string GetBitString(BigInt);
	//Works like GetBitString but stores the information in a given character array
	void GetBitString(BigInt,char*);
	//Converts a given bitstring to a hexstring
	std::string ConvertBitStringToHexString(std::string);
	//Calculates the hamming distance between two strings
	int GetHammingDistance(std::string, std::string);
}

namespace StringOperations
{
	//Returns the length of a given (null terminated!) char pointer
	int GetLength(char*);

	//Reverses a given string
	std::string ReverseString(std::string);

	//Stores the information of the string in the character pointer (take care of the size of the character array!)
	void StringToCharPointer(std::string,char*);

	//Returns a substring from the positions a to b (beginning at 0!)
	//E.g. GetSubString("00011000",2,6) => 01100
	//This can be used to get n bits of a given bitstring
	std::string GetSubString(std::string,unsigned short, unsigned short);

	//Splits a 32 bit value into 4 byte segments
	void Splice32BitValue(unsigned int, std::string*);

	//Fills the string up with zeros (on the left) until it has the given length
	std::string FillWithZerosLeft(std::string, unsigned int);
	//Fills the string up with zeros (on the right) until it has the given length
	std::string FillWithZerosRight(std::string, unsigned int);
}

namespace Other{
	//value^pot
	int Power(int, int);

	//Divides 48 bits into 8 6 bit values
	void Divide48Bits(Byte[48],Byte[8]);

	//Combines 8 4-Bit values to one 32-Bit value
	UnsignedValues::Doubleword CombineTo32Bits(Byte[8]);

	//Copies a given arrays content into another array
	//b will be stored in a
	void CopyArray(Byte a[],Byte b[],int);

	//Flips every nibble in 8-Bytes
	void Flip4Bits(Byte b1[64]);

	//Shifts the content of the given array n position to the left (cyclic)
	void CyclicLeftShift(Byte[],int,int length);
	//Shifts the content of the given array n position to the right (cyclic)
	void CyclicRightShift(Byte[],int,int length);

	//Converts a char into a hexstring
	std::string ConvertCharToHex(unsigned char c);
	//Converts a string of characters to a string of hexadecimal values
	//acralf: Add carriage return and linefeed (0 = false, else true)
	std::string ConvertStringToHex(std::string str, int acralf);
	//Converts a given hexstring to a character string
	std::string ConvertHexStringToCharacterString(std::string);
	//Compares n characters of the second string with n characters of the first string
	//n is the length of the first string
	int CompareStrings(std::string, std::string);
}
