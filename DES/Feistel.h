#include "BitByteMath.h"
#include "Expansion.h"
#include "PermutationSteps.h"
using namespace Macros;

namespace Feistel{
	//Use this enum to define if you either en- or decrypting
	enum Mode{encrypt = 0,decrypt = 1};

	//The feistel function defines a general way of encrypting blocks of data
	void FeistelFunction(Byte leftBlock[32], Byte rightBlock[32], Byte CKey[28], Byte DKey[28], Mode m);

	//This method returns the nth key of the en- or decryption process depending on the Mode m
	void GetNthKey(Byte CKey[28], Byte DKey[28],int n,Byte key[48], Mode m);
}
