#include "BitByteMath.h"
#include "DefinitionTables.h"

namespace Perm
{
	//Splits a 64-Bit value into two 32-Bit values and permutates them
	void InitialPermutation(Byte qw[64], Byte tmp[2][32]);

	//Permutates a with the values of b at a position determined by a permutation table per
	void Permutate(Byte* a,Byte* b,Byte* per,int l);
}
