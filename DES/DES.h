/*************************************************************************************************************
 *******************This code was made to test the data encryption standard about security.*******************
 *******************It is intended for use at the university of Rostock but may also be used******************
 *******************freely by anyone else for any (legal) purpose. To use this implementation*****************
 *******************simply import the header "DES.h" and use the methods CipherText and***********************
 *******************DecipherText. The data can be an arbitrary big string. The key must be********************
 *******************in hex format e.g. "0E329232EA6D0D73" and have 16 characters******************************
 *******************(16 symbols à 4 bits sums up to 64 bits keylength of which 56 are used).******************
 *******************To perform 3DES simply Cipher 3 times with 3 different keys.******************************
 *******************You are free to use and distribute this software as you wish as long as*******************
 *******************the authorship stays by me.***************************************************************
 *************************************************************************************************************
 *******************Author: Christian Koch********************************************************************
 *******************Email: ckoch533@gmail.com*****************************************************************
 *************************************************************************************************************/

//#define DEBUG		//Uncomment for debugging
#ifdef DEBUG
#include <iostream>
#endif

#include "BitByteMath.h"
#include "Feistel.h"

namespace crypto
{
	//This algorithm is used for both en- and decryption
	void DESAlgorithm(Byte text[64], Byte key[64], Feistel::Mode m);

	//Encrypts 64 bits of data with a key of 64 bits
	void Cipher(Byte text[64],Byte key[64]);

	//Encrypts a string of hex values with a key of 64 bits
	std::string CipherHexText(std::string, std::string);

	//Encrypts an arbitrary long string of characters with a key of 64 bits
	std::string CipherText(std::string, std::string);

	//Decrypts 64 bits of data with a key of 64 bits
	void Decipher(Byte text[64],Byte key[64]);

	//Decrypts a string of hex values with a key of 64 bits
	std::string DecipherHexText(std::string, std::string);

	//Decrypts an arbitrary long string of characters with a key of 64 bits
	std::string DecipherText(std::string, std::string);
}

namespace Keychecker
{
	//Describes the safety level of a key
	//NonHex: The given key contains non hex values (letters or values that are not from [0-F])
	//InvalidLength: The given key is not conform to the standard length (too long or short)
	//Normal: This key can be used normally
	//Semiweak: This key is semi weak and should not be used (safety is roughly halfed!)
	//Weak: This key should not be used
	enum Safety{NonHex = -2, InvalidLength = -1, Normal = 0, SemiWeak = 1, Weak = 2};

	//Checks if the given key is weak
	Keychecker::Safety KeyIsWeak(std::string key);

	//Note: This implementation uses 64 bit keys with 8 parity bits
	//If it did not do this there would be 4 additional weak keys:
	//0000000000000000,FFFFFFFFFFFFFFFF,E1E1E1E1F0F0F0F0,1E1E1E1E0F0F0F0F
}
