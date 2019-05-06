#include "BitByteMath.h"

int Other::Power(int value, int pot)
{
	if(pot == 0)
		return 1;

	int val = value;
	for(int i = 0;i<pot-1;i++)
	{
		val *= value;
	}

	return val;
}

int StringOperations::GetLength(char* str)
{
	int i = 0;
	while(str[i]!='\0'){i++;} //Every character array has to be null terminated!
	return i;
}

BigInt Parser::Parse(char* c)
{
	return Parser::BinToDecRightToLeft(c);
}

BigInt Parser::Parse(std::string s)
{
	return Parser::BinToDecRightToLeft(s);
}


BigInt Parser::BinToDecLeftToRight(char* byte)
{
	BigInt c = 0;
	char currBit = 0;

	for(int i = 0;byte[i]!='\0';i++)
	{
		if(byte[i] == ' '|| byte[i] == '\t'){continue;}

		currBit = ((byte[i]-'0'));	//The current bit
		c += Other::Power(2,i)*((int)currBit);
	}
	return c;
}
BigInt Parser::BinToDecRightToLeft(char* byte)
{
	int len = StringOperations::GetLength(byte);
	char currBit;
	BigInt c = 0;

	int j = 0;
	for(int i = len-1;i>=0;i--)
	{
		if(byte[i] == ' '|| byte[i] == '\t'){continue;}

		currBit = ((byte[i]-'0'));	//The current bit
		c += Other::Power(2,j)*((int)currBit);
		j++;
	}

	return c;
}
BigInt Parser::BinToDecLeftToRight(std::string byte)
{
	char c[byte.size()+1];
	StringOperations::StringToCharPointer(byte,c);
	return BinToDecLeftToRight(c);
}
BigInt Parser::BinToDecRightToLeft(std::string byte)
{
	char c[byte.size()+1];
	StringOperations::StringToCharPointer(byte,c);
	return BinToDecRightToLeft(c);
}

Byte Parser::ParseToByte(char* byte)
{
	int len = StringOperations::GetLength(byte);

	if(len > 8)
		return 0; //The value is not allowed to be bigger than 8 bits!

	char currBit;
	long c = 0;

	int j = 0;
	for(int i = len-1;i>=0;i--)
	{
		if(byte[i] == ' '|| byte[i] == '\t'){continue;}

		currBit = ((byte[i]-'0'));	//The current bit
		c += Other::Power(2,j)*((int)currBit);
		j++;
	}

	return c;
}
Byte Parser::ParseToByte(std::string byte)
{
	char c[byte.size()+1];
	StringOperations::StringToCharPointer(byte,c);
	return ParseToByte(c);
}

unsigned char Parser::BinToHex(Byte c)
{
	switch(c){
	case 0: return '0';
	case 1: return '1';
	case 2: return '2';
	case 3: return '3';
	case 4: return '4';
	case 5: return '5';
	case 6: return '6';
	case 7: return '7';
	case 8: return '8';
	case 9: return '9';
	case 10: return 'A';
	case 11: return 'B';
	case 12: return 'C';
	case 13: return 'D';
	case 14: return 'E';
	case 15: return 'F';
	default: return '0';
	}
}

Byte Parser::HexToBin(unsigned char c)
{
	switch(c){
	case '0': return 0;
	case '1': return 1;
	case '2': return 2;
	case '3': return 3;
	case '4': return 4;
	case '5': return 5;
	case '6': return 6;
	case '7': return 7;
	case '8': return 8;
	case '9': return 9;
	case 'A': return 10;
	case 'B': return 11;
	case 'C': return 12;
	case 'D': return 13;
	case 'E': return 14;
	case 'F': return 15;
	case 'a': return 10;
	case 'b': return 11;
	case 'c': return 12;
	case 'd': return 13;
	case 'e': return 14;
	case 'f': return 15;
	default: return 0;
	}
}

char Parser::BitStringToHex(std::string str)
{
	if(str=="0000") return '0';
	else if(str=="0001") return '1';
	else if(str =="0010") return '2';
	else if(str=="0011") return '3';
	else if(str=="0100") return '4';
	else if(str=="0101") return '5';
	else if(str=="0110") return '6';
	else if(str=="0111")return '7';
	else if(str=="1000")return '8';
	else if(str=="1001") return '9';
	else if(str=="1010") return 'A';
	else if(str=="1011") return 'B';
	else if(str=="1100") return 'C';
	else if(str=="1101") return 'D';
	else if(str=="1110") return 'E';
	else if(str=="1111") return 'F';
	else return '0';
}

//Parses 8 symbols to 8 values
void Parser::ParseSymbolsToByte(std::string str, Byte b[8])
{
	if(str.length()!=16)
		return;

	int j = 0; //j [0-8], i [0-15]
	for(int i = 0;i<16;i++)
	{
		b[j] = HexToBin(str[i])|HexToBin(str[i+1])<<4;

		i++;
		j++;
	}
}

void Parser::ParseBytesToBits(Byte b[8], Byte b1[64])
{
	for(int i = 0;i<8;i++)
	{
		for(int j = 0;j<8;j++){
			b1[i*8+j] = BitOperations::GetBit(b[i],j);
		}
	}

	Other::Flip4Bits(b1);
}

void Parser::ParseSymbolsToBits(std::string str, Byte b[64])
{
	Byte b2[8];
	Parser::ParseSymbolsToByte(str,b2);
	Parser::ParseBytesToBits(b2,b);
}


short BitOperations::GetBit(Byte byte, short bit)
{
	return (byte&(1<<bit))>>bit;
}

Byte BitOperations::SetBit(Byte byte, short nbit, short value)
{
	if (GetNthBit(byte, nbit) == value || value > 1)
		return byte;

	if (value == 0) {
		char zeroMask = ~(1 << nbit);	//1111 1111 with a zero at the nth position
		byte = byte & zeroMask;
	}
	else if (value == 1) {
		char oneMask = 1 << nbit;	//0000 0000 with a one at the nth position
		byte = byte | oneMask;
	}

	return byte;
}

void BitOperations::SetBitPointer(Byte* byte, short nbit, short value)
{
	if (GetNthBit(*byte, nbit) == value || value > 1)
		return;

	if (value == 0) {
		char zeroMask = ~(1 << nbit);	//1111 1111 with a zero at the nth position
		*byte = *byte & zeroMask;
	}
	else if (value == 1) {
		char oneMask = 1 << nbit;	//0000 0000 with a one at the nth position
		*byte = *byte | oneMask;
	}
}

std::string StringOperations::ReverseString(std::string str)
{
	std::string tmp = "";

	for(int i = str.length()-1;i>=0;i--)
	{
		tmp += str[i];
	}

	return tmp;
}

std::string Bitstrings::GetBitString(Byte byte)
{
	std::string str = "";
	for(int i = 0;i<8;i++)
	{
		if(GetNthBit(byte,i)==1)
			str += "1";
		else if(GetNthBit(byte,i)==0)
			str += "0";
		else
			return nullptr;
	}
	return StringOperations::ReverseString(str);
}
std::string Bitstrings::GetBitString(unsigned int byte)
{
	std::string str = "";
	for(int i = 0;i<32;i++)
	{
		if(GetNthBit(byte,i)==1)
			str += "1";
		else if(GetNthBit(byte,i)==0)
			str += "0";
		else
			return nullptr;
	}
	return StringOperations::ReverseString(str);
}
std::string Bitstrings::GetBitString(BigInt byte)
{
	std::string str = "";
	for(int i = 0;i<64;i++)
	{
		if(GetNthBit(byte,i)==1)
			str += "1";
		else if(GetNthBit(byte,i)==0)
			str += "0";
		else
			return nullptr;
	}
	return StringOperations::ReverseString(str);
}
void Bitstrings::GetBitString(BigInt byte, char* to)
{
	StringOperations::StringToCharPointer(GetBitString(byte),to);
}

std::string Bitstrings::ConvertBitStringToHexString(std::string str)
{
	if(str.length()%4 != 0)	//Make the string a multiple of 4 in length
	{
		while(str.length()%4 != 0){str+='0';}
	}

	std::string res = "";
	std::string tmp = "";
	char c;
	for(unsigned int i = 0;i<str.length();i++)
	{
		for(int j = 0;j<4;j++)
			tmp += str[i+j];

		c = Parser::BitStringToHex(tmp);

		res += c;

		tmp = "";

		i+=3;
	}

	return res;
}

int Bitstrings::GetHammingDistance(std::string a, std::string b)
{
	int res = 0;

	if(a.length() != b.length()) //Dont allow different lengths
		return -1;

	for(unsigned int i = 0;i<a.length();i++)
	{
		if(a[i]!=b[i])
			res++;
	}

	return res;
}

void StringOperations::StringToCharPointer(std::string from,char* to)
{
	strcpy(to,from.c_str());
}

std::string StringOperations::GetSubString(std::string str,unsigned short start, unsigned short end)
{
	std::string subString = "";

	for(int i = start;i<=end;i++)
	{
		subString += str[i];
	}

	return subString;
}

void StringOperations::Splice32BitValue(unsigned int value, std::string* to)
{
	std::string str = Bitstrings::GetBitString(value);

	for(int i = 0;i<4;i++)
	{
		to[i] = GetSubString(str,i*8,i*8+7);
	}
}

std::string StringOperations::FillWithZerosLeft(std::string str, unsigned int length)
{
	std::string tmp = str;
	while(tmp.length()-1 < length)
	{
		tmp = '0' + tmp;
	}

	return tmp;
}
std::string StringOperations::FillWithZerosRight(std::string str, unsigned int length)
{
	std::string tmp = str;
	while(tmp.length()-1 < length)
	{
		tmp = tmp + '0';
	}

	return tmp;
}

void Other::Divide48Bits(Byte b[48],Byte b2[8])
{
	for(int i = 0;i<8;i++)
	{
		b2[i] = b[i*6+0]|b[i*6+1]<<1|b[i*6+2]<<2|b[i*6+3]<<3|b[i*6+4]<<4|b[i*6+5]<<5;
	}
}

UnsignedValues::Doubleword Other::CombineTo32Bits(Byte b[8])
{
	UnsignedValues::Doubleword dw = 0;
	for(int i = 0;i<8;i++)
	{
		dw |= b[i]<<(i*4);
	}

	return dw;
}

void Other::CopyArray(Byte a[],Byte b[],int length)
{
	for(int i = 0;i<length;i++)
	{
		a[i] = b[i];
	}
}

void Other::Flip4Bits(Byte b1[64])
{
	Byte temp[64]; for(int i = 0;i<64;i++){temp[i]=b1[i];}
	for(int i = 0;i<64;i++)
	{
		b1[i] = temp[i+3];
		b1[i+1] = temp[i+2];
		b1[i+2] = temp[i+1];
		b1[i+3] = temp[i];
		i+=3;
	}
}

//Shifts the content of the given array n position to the left (cyclic)
void Other::CyclicLeftShift(Byte b[],int n,int length)
{
	Byte tmp[length] = {0};
	for(int i = 0;i<length;i++){tmp[i]=b[i];}
	for(int i = 0;i<length;i++)
	{
		b[i] = tmp[(i+n)%length];
	}

}
//Shifts the content of the given array n position to the right (cyclic)
void Other::CyclicRightShift(Byte b[],int n,int length)
{
	Byte tmp[length];
	for(int i = 0;i<length;i++){tmp[i]=b[i];}

	for(int i = 0;i<length;i++)
	{
		b[(i+n)%length] = tmp[i];
	}
}

std::string Other::ConvertCharToHex(unsigned char c)
{
	std::string tmp = "";

	for(int i = 0;i<8;i++)
	{
		tmp += (c%2+'0');
		c/=2;
	}
	std::string tmp1 = "", tmp2 = "";

	for(int i = 0;i<8;i++)
	{
		if(i<4)
			tmp1 += tmp[i];
		else if(i>=4)
			tmp2 += tmp[i];
	}

	tmp1 = StringOperations::ReverseString(tmp1);
	tmp2 = StringOperations::ReverseString(tmp2);


	std::string res;
	res+=Parser::BitStringToHex(tmp2);
	res+=Parser::BitStringToHex(tmp1);

	return res;
}

std::string Other::ConvertStringToHex(std::string str, int acralf)
{
	std::string tmp = "";
	unsigned char c;
	for(unsigned int i = 0;i<str.length();i++)
	{
		c = (unsigned char)str[i];	//Strings normally consist of signed chars
		tmp += Other::ConvertCharToHex(c);
	}

	if(acralf)
		tmp += "0D0A";	//Add carriage return and line feed

	return tmp;
}

std::string Other::ConvertHexStringToCharacterString(std::string str)
{
	std::string res = "";
	unsigned char c;
	for(unsigned int i = 0;i<str.length();i++) //Foreach hexvalue
	{
		//std::cout <<(((int)Parser::HexToBin(((unsigned char)str[i]))<<4)|((int)Parser::HexToBin(((unsigned char)str[i+1]))))<< std::endl;
		//Create a byte out of two hex values (nibbles)
		c = (((int)Parser::HexToBin(((unsigned char)str[i]))<<4)|((int)Parser::HexToBin(((unsigned char)str[i+1]))));

		res += c;

		i++;
	}

	return res;
}

int Other::CompareStrings(std::string a, std::string b)
{
	for(unsigned int i = 0;i<a.length();i++)
	{
		if(a[i]!=b[i])
			return 0;
	}
	return 1;
}
