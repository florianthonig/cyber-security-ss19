//
// Created by Florian on 26.04.19.
//

#include "rsa-impl-replacement.h"
#include <botan/rsa.h>
#include <botan/system_rng.h>
#include <botan/bigint.h>
#include <botan/numthry.h>
#include <botan/pkcs8.h>
#include <botan/x509_key.h>
#include <iostream>
#include <fstream>

using namespace Botan;

int main(int argc, char** argv) {
    // need at least one argument with the keysize
    if (argc < 2) {
        std::cerr << "Usage: rsa-keygen <keysize in bits>" << std::endl;
        exit(-1);
    }
    uint64_t bitsize = strtoul(argv[1], nullptr, 10);
    // check for error or default bit size
    if (bitsize == -1UL || bitsize == 0) {
        std::cerr << "Something went wrong:\nThe key size is set to default 512 bits" << std::endl;
        bitsize = 512;
    }
    // create the random numbers
    uint64_t bits_p = (bitsize + 1) /2;
    uint64_t bits_q = bitsize - bits_p;
    BigInt m_p;
    BigInt m_q;
    BigInt m_n;
    BigInt exp = 65537;
    do {
        m_p = generate_rsa_prime_allow_small_keys(system_rng(), system_rng(), bits_p, exp);
        m_q = generate_rsa_prime_allow_small_keys(system_rng(), system_rng(), bits_q, exp);
        m_n = m_p * m_q;
    } while(m_n.bits() != bitsize);
    RSA_PrivateKey privateKey (m_p, m_q, exp);

    std::ofstream pkFile("private.pem", std::ios::out | std::ios::trunc);
    if (!pkFile.is_open()) {
        std::cerr << "Unable to open 'private.pem'. Check your filesystem." << std::endl;
        exit(-1);
    }
    pkFile << PKCS8::PEM_encode(privateKey);
    pkFile.close();

    std::ofstream pukFile("public.pem", std::ios::out | std::ios::trunc);
    if (!pukFile.is_open()) {
        std::cerr << "Unable to open 'public.pem'. Check your filesystem." << std::endl;
        exit(-1);
    }
    pukFile << X509::PEM_encode(privateKey);
    pukFile.close();

}