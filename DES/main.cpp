/**************************************************************************************************
 *********************This is a simple example on how to use the DES algorithm.********************
 *********************This can be expanded by including an algorithm to read***********************
 *********************from a file and write to a file to more easily read and**********************
 *********************write encrypted data.********************************************************
 **************************************************************************************************
 **************************************************************************************************/

//includes
#include <iostream> //Used to display the data
#include "DES.h" //Used for encryption
#include "Filereader.h" //Used to read and write data to/from files

//Macros
#define PRINT(x) (std::cout << x << std::endl) //Displays a given object in one line
#define SPACE() (PRINT(""))	//Prints an empty line
#define INPUT "Input.txt"	//The inputfile
#define OUTPUT "Output.txt" //The outputfile
#define ENCRYPTIONSTEP "Encrypted.txt"	//The encryption step (the input after encryption)

#define USEHEXVALUES	//Uncomment to use hexvalues instead of 8 characters
#ifndef USEHEXVALUES
#define KEY "abcdefgh" //Has to have a length of 8
#else
#define KEY "1234000000001234" //Has to have a length of 16
#endif

int main()
{
	PRINT("--------------------------------------------------------");
	//Setting up storage to display single steps
	std::string t = "";		//Used to store the encrypted data
	std::string t2 = ""; 	//Used to store the decrypted data

	//Setup the key and the data
	std::string vKey =
#ifndef USEHEXVALUES
			Other::ConvertStringToHex(KEY,0);
#else
			KEY;
#endif

	//std::string vKey = "0E329232EA6D0D73";	//The used key
	std::string vdata = read::GetContent(INPUT);//"Everything is a lie! This program is not real!";	//The used data

	//Make sure to check your key
	//There are about 16 weak keys
	switch(Keychecker::KeyIsWeak(vKey)) //Returns 0 if the key is valid
	{
		case Keychecker::NonHex:PRINT("Your key contains non hex values!"); return 1;
		case Keychecker::InvalidLength: PRINT("Your key has the wrong length!"); return 1;
		case Keychecker::Normal: break;
		case Keychecker::SemiWeak: PRINT("Your key is semi weak! This decreases the security by a lot\n"
				   << "For more information take a look at weak keys for DES"); return 1;
		case Keychecker::Weak: PRINT("Your key is weak! Do not use this key!\n"
				   << "For more information take a look at weak keys for DES"); return 1;
		default: PRINT("Your key is not safe or valid!"); return 1;
	}

	PRINT("Used Key:          " << vKey << std::endl); //Display the key
	PRINT("Input encryption:  " << vdata); //Display the unencrypted data
	SPACE();

	//Encrypt
	t=crypto::CipherText(vdata,vKey);	//Encrypt the given data

	PRINT("Output Encryption: " << t); //Display the encrypted data
	SPACE();

	write::OverwriteFile(ENCRYPTIONSTEP,t);	//Create a file which contains the encrypted data

	//Decrypt
	t2 = crypto::DecipherText(t,vKey);	//Decrypt the given data

	PRINT("Output Decryption: " << t2); //Display the decrypted data
	SPACE();

	write::OverwriteFile(OUTPUT,t2);	//Create a file which contains the encrypted data

	PRINT("--------------------------------------------------------");

	return 0;
}
