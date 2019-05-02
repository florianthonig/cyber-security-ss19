//
// Created by Florian on 26.04.19.
//
#include <botan/pk_keys.h>
#include <botan/pubkey.h>
#include <botan/pkcs8.h>
#include <iostream>
#include <fstream>

using namespace Botan;

#define FILE_SIZE_LIMIT 100000000 // more than 500MB are not directly loaded into memory

uint64_t get_file_size(std::ifstream &file) {
    const std::fpos<mbstate_t> &fpos = file.tellg();
    file.seekg(0, file.end);
    const std::fpos<mbstate_t> &rPos = file.tellg();
    file.seekg(fpos);
    return rPos;
}

int main(int argc, char** argv) {
    // need at least one argument with the keysize
    if (argc < 3) {
        std::cerr << "Usage: rsa-decrypt <private-key-file> <file-to-decrypt>" << std::endl;
        exit(-1);
    }
    // load the key
    Private_Key *pKey = PKCS8::load_key(argv[1], system_rng());
    if (!pKey) {
        std::cerr << "Unable to open '" << argv[1] << "' for private key. Check your filesystem." << std::endl;
        exit(-1);
    } else {
        std::cout << "Successfully read a " << pKey->algo_name() << " key from '" <<
                  argv[1] << "' with " << pKey->key_length() << " Bits" << std::endl;
    }
    // check if the decryption file can be opened
    std::ifstream decFile(argv[2], std::ios::in | std::ios::binary);
    if (!decFile.is_open()) {
        std::cerr << "Unable to open '" << argv[2] << "'. Check your filesystem." << std::endl;
        exit(-1);
    }
    uint64_t fileSize = get_file_size(decFile);
    std::cout << "Successfully opened '" << argv[2] << "' with a size of " << fileSize << " Bytes" << std::endl;

    // create the crypt object
    PK_Decryptor_EME cryptObj(*pKey, system_rng(), "EME1(SHA-256)");
    if (fileSize > FILE_SIZE_LIMIT) {
        std::cerr << "Decrypting files are only supported for files with a max. size of " << FILE_SIZE_LIMIT << " Bytes" << std::endl;
        exit(-1);
    }
    // allocate buffer
    uint8_t* buffer = (uint8_t*) malloc(fileSize * sizeof(uint8_t));
    if (!buffer) {
        std::cerr << "Not enough memory to allocate " << fileSize << " Bytes" << std::endl;
        exit(-1);
    }
    // read the content of the file
    decFile.read((char*) buffer, fileSize);
    decFile.close();

    // decrypt the file
    const secure_vector<uint8_t > &plain = cryptObj.decrypt(buffer, fileSize);

    // free the memory from the buffer again
    free(buffer);

    // save the decrypted file
    std::string outFileName = std::string(argv[2]) + ".dec";
    std::ofstream plainFile(outFileName, std::ios::out | std::ios::trunc);
    if (!plainFile.is_open()) {
        std::cerr << "Unable to open '" << outFileName << "'. Check your filesystem." << std::endl;
        exit(-1);
    }
    plainFile.write((const char*) plain.data(), plain.size());
    std::cout << "Successfully wrote " << plain.size() << " Bytes to '" << argv[2] << "'" << std::endl;
    plainFile.close();
}