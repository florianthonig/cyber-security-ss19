#include "BitByteMath.h"

#ifndef DEFINITIONTABLES_H

//The following contains all substitution boxes needed for the DES algorithm
namespace SubBoxes{
	extern Byte S1[4][16];

	extern Byte S2[4][16];

	extern Byte S3[4][16];

	extern Byte S4[4][16];

	extern Byte S5[4][16];

	extern Byte S6[4][16];

	extern Byte S7[4][16];

	extern Byte S8[4][16];
}

namespace SBoxMethods
{
	//Returns the entry of a given S-Box
	//b: The byte you want to use (only the first 6 bits are used)
	//You can use a notation like this: Byte b = Parser::ParseToByte("000010");
	//sBox: The S-Box you want to use (this value is between 1 and 8)
	Byte GetSBoxArrayEntry(Byte b, int sBox);
	Byte GetSBoxArrayEntry(Byte b[6], int sBox);

}

//PermutationBox
namespace Permutations{

	//PermutationBox, used for the last permutation of the bits
	extern Byte P[];

	//This table inverses the initial permutation
	extern Byte InverseInitialPermutation[];

	//Initial Permutation table (this table contains the LBlock and RBlock initial permutation tables)
	extern Byte IP[];

	//Used for the initial permutation of the left block
	extern Byte LBlockInitialPermutation[];

	//Used for the initial permutation of the rigth block
	extern Byte RBlockInitialPermutation[];

	//This table can be used to expand a 32 bit value
	//to a 48 bit value
	extern Byte ExpandBitPermutation[];

}

namespace Key
{
	//Used in encryption with a left rotation
	extern Byte ShiftValue[];

	//Used in decryption with a right rotation
	extern Byte DecryptShiftValue[];

	//These values are used to determine which bits of
	//the key used as a final key
	extern Byte CDBlockValues[];

	//These values are used to determine which bits of
	//the key are stored in the CBlock of the used key
	extern Byte CBlockValues[];

	//These values are used to determine which bits of
	//the key are stored in the DBlock of the used key
	extern Byte DBlockValues[];

	//These 48 of 56 values of the key will be used
	extern Byte SelectBitOfKey[];

	//These keys are known to be weak, they shouldnt be used
	//Weak means, that all subkeys created are identical
	extern std::string WeakKeys[];

	//These keys are known to be semi-weak, they shouldnt be used either
	//Semi-weak means, that Ek1(Ek2(M))=M,
	//so one key can be broken with two keys
	extern std::string SemiWeakKeys[];
}

#endif
