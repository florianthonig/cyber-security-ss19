#include "Feistel.h"

void Feistel::FeistelFunction(Byte leftBlock[32], Byte rightBlock[32], Byte CKey[28], Byte DKey[28], Mode m)
{
	Byte expandStorage[48] = {0};	//A 48 bit value (saved as bytes, because there is no smaller datatype)
	Byte key[48] = {0};				//The used 48-Bit key

	Byte rb[32] = {0};	//This will contain a copy of the rightblock to store it in the leftblock

	//Needed in 3
	Byte storage = 0;	//Store the S-Box value in this variable
	Byte b[6] = {0};			//Store 1 6-Bit value in this variable
	Byte tmp32[32] = {0};	//Store the 8 4-Bit values in this variable
	Byte tmp48[8][6] = {0}; //Store the 8 6-Bit values in this variable

	for(int n = 1;n<=16;n++)	//Do 16 rounds
	{
		//0.1 Create a copy of the untouched rightBlock
		Other::CopyArray(rb,rightBlock,32);

		//0.2 Get Key
		Feistel::GetNthKey(CKey,DKey,n,key,m); //Store the nth key in the variable key

		//1. Expansion with the given expansion method
		Expand::ExpandTo48Bit(rightBlock,expandStorage);

		//2. Result gets XOR-ed every round with a part of the 48-bit sub-key (generated from the main key)
		for(int i = 0;i<48;i++)
		{
			expandStorage[i] = XOR(expandStorage[i],key[i]);
		}

		//3. Create 8 6-Bit parts from the 48-Bits and shorten them into 4 bits by using the S-Boxes
		for(int i = 0;i<8;i++) //Store the 48 bits in tmp2
		{
			for(int j = 0;j<6;j++){
				tmp48[i][j] = expandStorage[i*6+j];
			}
		}

		for(int i = 0;i<8;i++)
		{
			//1. Get a 6-Bit value
			for(int j = 0;j<6;j++){b[j] = tmp48[i][j];} //this works correctly

			//2. Get the corresponding 4-Bit value from the S-Box
			storage = SBoxMethods::GetSBoxArrayEntry(b,i+1); //this works correctly

			//3. Store the 4-Bit value in variable
			for(int k=0;k<4;k++)
			{
				tmp32[i*4+k] = BitOperations::GetBit(storage,3-k); //Store bits in reverse order
			}

		}	//Shorten all 8 6-Bit values to 4-Bit values

		//4. Permutate with permutation p
		//Store the permutation in the right block
		Perm::Permutate(rightBlock,tmp32,Permutations::P,32);

		//5. XOR left and right side and store it in the right side
		for(int i = 0;i<32;i++) //Works correctly
		{
			rightBlock[i] = XOR(rightBlock[i],leftBlock[i]);
		}

		//5.1 Copy rightblock into leftblock
		Other::CopyArray(leftBlock,rb,32);

		//------Until here everything should be fine!------

		//Note: Result of first iteration is correct, end result is false
		//Error mus be in feistel function: R16 and L16 are both false
		//R16 ist korrekt
	}
}

void Feistel::GetNthKey(Byte CKey[28], Byte DKey[28],int nr,Byte key[48], Mode m)
{
	Byte temp[28] = {0},temp2[28] = {0},unshorted[56] = {0};	//temp and temp2 are copies of the key blocks
																//unshorted is combines CKey and DKey
	Byte tCKey[28] = {0};
	Byte tDKey[28] {0};

	for(int i = 0;i<28;i++){tCKey[i] = CKey[i]; tDKey[i] = DKey[i];}

	int n=0;

	do
	{
		//Get Key
		for(int i = 0;i<28;i++){temp[i] = tCKey[i];temp2[i] = tDKey[i];}	//Copy the key blocks
		int shifts = 0; //The ammount of shifts for the c or d key

		if(m == encrypt){
			shifts = Key::ShiftValue[n];
			for(int i = 0;i<28;i++)	//cyclic leftshifting the keys
			{
				tCKey[i] = temp[(i+shifts)%28];
				tDKey[i] = temp2[(i+shifts)%28];
			}
		}
		else if(m == decrypt)
		{
			shifts = Key::DecryptShiftValue[n];
			for(int i = 0;i<28;i++)	//Cyclic rightshifting the keys
			{
				tCKey[(i+shifts)%28] = temp[i];
				tDKey[(i+shifts)%28] = temp2[i];
			}
		}


		for(int i = 0;i<56;i++)	//Create a combined key (a CD key)
		{
			if(i<28)
				unshorted[i] = tCKey[i];
			else if(i>=28)
			{
				unshorted[i] = tDKey[i-28];
			}
		}

		//Last step: shorten to 48 bits
		Perm::Permutate(key,unshorted,Key::SelectBitOfKey,48);

		n++;
	}while(n<nr);
}
