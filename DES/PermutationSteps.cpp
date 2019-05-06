#include "PermutationSteps.h"

void Perm::InitialPermutation(Byte qw[64], Byte tmp[2][32])
	{
		//Byte b[64];
		Byte qw2[64] = {0};
		//qw contains 64 bits
		Byte leftWord[32],rightWord[32];

		Perm::Permutate(qw2,qw,Permutations::IP,64);

		for(int i = 0;i<64;i++)
		{
			if(i<32)
			{
				leftWord[i] = qw2[i];
			}
			else if(i>=32)
			{
				rightWord[i-32] = qw2[i];
			}
		}

		for(int i = 0;i<32;i++){
			tmp[0][i] = leftWord[i];
			tmp[1][i] = rightWord[i];
		}
	}

void Perm::Permutate(Byte* a,Byte* b,Byte* per,int l)
{
	for(int i = 0;i<l;i++)
	{
		a[i] = b[per[i]-1];
	}
}
