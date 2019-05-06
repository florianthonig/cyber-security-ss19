#include "Expansion.h"

void Expand::ExpandTo48Bit(Byte b[32], Byte b2[48])
{
	for(int i = 0;i<48;i++)
	{
		b2[i] = b[Permutations::ExpandBitPermutation[i]-1];
	}
}
