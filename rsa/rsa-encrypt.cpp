//
// Created by Florian on 26.04.19.
//
#include <botan/pk_keys.h>
#include <botan/pubkey.h>
#include <botan/x509_key.h>
#include <iostream>
#include <fstream>

using namespace Botan;

#define FILE_SIZE_LIMIT 50000000 // more than 500MB are not directly loaded into memory

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
        std::cerr << "Usage: rsa-encrypt <public-key-file> <file-to-encrypt>" << std::endl;
        exit(-1);
    }
    // load the key
    Public_Key *pKey = X509::load_key(argv[1]);
    if (!pKey) {
        std::cerr << "Unable to open '" << argv[1] << "' for public key. Check your filesystem." << std::endl;
        exit(-1);
    } else {
        std::cout << "Successfully read a " << pKey->algo_name() << " key from '" <<
        argv[1] << "' with " << pKey->key_length() << " Bits" << std::endl;
    }
    // check if the encryption file can be opened
    std::ifstream encFile(argv[2], std::ios::in | std::ios::binary);
    if (!encFile.is_open()) {
        std::cerr << "Unable to open '" << argv[2] << "'. Check your filesystem." << std::endl;
        exit(-1);
    }
    uint64_t fileSize = get_file_size(encFile);
    std::cout << "Successfully opened '" << argv[2] << "' with a size of " << fileSize << " Bytes" << std::endl;
    
    // create the crypt object
    PK_Encryptor_EME cryptObj(*pKey, system_rng(), "EME1(SHA-256)");
    if (fileSize > cryptObj.maximum_input_size()) {
        std::cerr << pKey->algo_name() << " supports at most " << cryptObj.maximum_input_size() << " Bytes for encryption." << std::endl;
        exit(-1);
    }
    
    if (fileSize > FILE_SIZE_LIMIT) {
        std::cerr << "Encrypting files are only supported for files with a max. size of " << FILE_SIZE_LIMIT << " Bytes" << std::endl;
        exit(-1);
    }
    // allocate buffer
    uint8_t* buffer = (uint8_t*) malloc(fileSize * sizeof(uint8_t));
    if (!buffer) {
        std::cerr << "Not enough memory to allocate " << fileSize << " Bytes" << std::endl;
        exit(-1);
    }
    // read the content of the file
    encFile.read((char*) buffer, fileSize);
    encFile.close();

    // encrypt the file
    const std::vector<uint8_t> &cipher = cryptObj.encrypt(buffer, fileSize, system_rng());

    // free the memory from the buffer again
    free(buffer);

    // save the encrypted file
    std::string outFileName = std::string(argv[2]) + ".enc";
    std::ofstream cipherFile(outFileName, std::ios::out | std::ios::trunc);
    if (!cipherFile.is_open()) {
        std::cerr << "Unable to open '" << outFileName << "'. Check your filesystem." << std::endl;
        exit(-1);
    }
    cipherFile.write((const char*) cipher.data(), cipher.size());
    std::cout << "Successfully wrote " << cipher.size() << " Bytes to '" << argv[2] << "'" << std::endl;
    cipherFile.close();
}