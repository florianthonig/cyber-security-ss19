#include "DES.h"

void crypto::DESAlgorithm(Byte text[64], Byte key[64], Feistel::Mode m)
{
	//1. Initial permutation
		Byte blocks[2][32] = {0};
		Perm::InitialPermutation(text, blocks);

		Byte leftBlock[32] = {0};
		Byte rightBlock[32] = {0};

		Other::CopyArray(leftBlock,blocks[0],32);
		Other::CopyArray(rightBlock,blocks[1],32);

		//2. Key management
		Byte tempKey[56] = {0};
		Perm::Permutate(tempKey,key,Key::CDBlockValues,56);

		Byte CKey[28]={0},DKey[28]={0};
		for(int i = 0;i<28;i++)	//Permutate the key and store it in a C and a D block
		{
			CKey[i] = tempKey[i];
			DKey[i] = tempKey[i+28];
		}

		//3. Feistel function
		Feistel::FeistelFunction(leftBlock,rightBlock,CKey,DKey,m);

		//Combine the two halfs to a 64 bit value
		Byte arr[64];	//This will contain all 64 bits of the calculated value
		for(int i = 0;i<64;i++)
		{
			if(i<32)
				arr[i] = rightBlock[i];//arr[i] = leftBlock[i];
			else if(i>=32)
				arr[i] = leftBlock[i-32];//arr[i] = rightBlock[i-32];
		}

		//4. InverseInitialPermutation (Final permutation)
		Perm::Permutate(text,arr,Permutations::InverseInitialPermutation,64);
}

void crypto::Cipher(Byte text[64],Byte key[64])
{
	crypto::DESAlgorithm(text,key,Feistel::encrypt);
}

void crypto::Decipher(Byte text[64],Byte key[64])
{
	crypto::DESAlgorithm(text,key,Feistel::decrypt);
}

std::string crypto::CipherHexText(std::string text, std::string keyValue)
{
	//Make the length of the text a multiple of 16
	if(text.length()%16 != 0)
	{
		while(text.length()%16 != 0){text+='0';}
	}

	std::string tmp = "";
	std::string res = "";
	//There is just one key
	Byte key[64] = {0};
	Parser::ParseSymbolsToBits(keyValue,key);
	Byte temp[text.length()/16][64] = {0};

	//Cipher the data (store the result in temp)
	int n = 0;
	for(unsigned int i = 0;i<text.length();i++){

		std::string data = "";
		for(int k = 0;k<16;k++){data += text[i+k];}
		Byte qw[64] = {0};
		Parser::ParseSymbolsToBits(data,qw);

		crypto::Cipher(qw,key);

		for(int a = 0;a<64;a++)
			temp[n][a] = qw[a];
		i+=15;
		n++;
	}

	//Convert the bits to a string
	res = "";
	for(unsigned int i = 0;i<text.length()/16;i++)
	{
		tmp = "";

		for(int j = 0;j<16;j++){
			for(int k=0;k<4;k++)
			{
				tmp += temp[i][j*4+k]+'0';
			}

			res += Parser::BitStringToHex(tmp);	//Store the result
			tmp = "";
		}
	}

	return res;
}

std::string crypto::CipherText(std::string text, std::string keyValue)
{
	std::string res = "";
	res = crypto::CipherHexText(Other::ConvertStringToHex(text,0),keyValue);

	return Other::ConvertHexStringToCharacterString(res);
}

std::string crypto::DecipherHexText(std::string text, std::string keyValue)
{
	//Make the length of the text a multiple of 16
	if(text.length()%16 != 0)
	{
		while(text.length()%16 != 0){text+='0';}
	}

	std::string tmp = "";
	std::string res = "";
	//There is just one key
	Byte key[64] = {0};
	Parser::ParseSymbolsToBits(keyValue,key);
	Byte temp[text.length()/16][64] = {0};

	//Cipher the data (store the result in temp)
	int n = 0;
	for(unsigned int i = 0;i<text.length();i++){

		std::string data = "";
		for(int k = 0;k<16;k++){data += (unsigned char)text[i+k];}
		Byte qw[64] = {0};
		Parser::ParseSymbolsToBits(data,qw);

		crypto::Decipher(qw,key);

		for(int a = 0;a<64;a++)
			temp[n][a] = qw[a];
		i+=15;
		n++;
	}

	//Convert the bits to a string
	res = "";
	for(unsigned int i = 0;i<text.length()/16;i++)
	{
		tmp = "";

		for(int j = 0;j<16;j++){
			for(int k=0;k<4;k++)
			{
				tmp += temp[i][j*4+k]+'0';
			}

			res += Parser::BitStringToHex(tmp);	//Store the result
			tmp = "";
		}
	}

	return res;
}

std::string crypto::DecipherText(std::string text, std::string key)
{
	std::string res = "";
	res = crypto::DecipherHexText(Other::ConvertStringToHex(text,0),key);

	return Other::ConvertHexStringToCharacterString(res);
}

Keychecker::Safety Keychecker::KeyIsWeak(std::string key)
{
	if(key.length() != 16)	//Check if keylength is valid
		return Keychecker::InvalidLength;

	std::string tmp; //Make a copy of the key
	for(int i = 0;i<16;i++){tmp += key[i];}

	for(int i = 0;i<16;i++)	//Make all letters uppercase
	{
		switch(tmp[i])
		{
			case 'a': tmp[i] = 'A';break;
			case 'b': tmp[i] = 'B';break;
			case 'c': tmp[i] = 'C';break;
			case 'd': tmp[i] = 'D';break;
			case 'e': tmp[i] = 'E';break;
			case 'f': tmp[i] = 'F';break;
		}
	}

	unsigned char allowedSymbols[] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
	for(int i = 0;i<16;i++)
	{
		int p = 0;
		for(int j = 0;j<16;j++)
		{
			if(tmp[i]==allowedSymbols[j])
				p|=1;
		}

		if(p != 1)
			return Keychecker::NonHex;
	}

	for(int i = 0;i<4;i++)	//Check for weak keys
	{
		if(tmp==Key::WeakKeys[i])
			return Keychecker::Weak;
	}

	for(int i = 0;i<12;i++)	//Check for semi-weak keys
	{
		if(tmp==Key::SemiWeakKeys[i])
			return Keychecker::SemiWeak;
	}

	return Keychecker::Normal;
}
