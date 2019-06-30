//
// Created by florian on 24.05.19.
//

#include <botan/pk_keys.h>
#include <botan/pubkey.h>
#include <botan/x509_key.h>
#include <botan/rsa.h>
#include <botan/numthry.h>
#include <iostream>
#include <fstream>
#include <chrono>
#include "rsa-impl-replacement.h"

using namespace Botan;

BigInt find_x(const BigInt& n) {
    BigInt x(system_rng(), n.bits());
    while (gcd(x, n) != 1) {
        x = BigInt(system_rng(), n.bits());
    }
    return x;
}

BigInt find_r(const BigInt &x, const BigInt &n) {
    BigInt r = 2;

    while (power_mod(x, r, n) != 1 && r < n) {
         r+=2;
    }
    return r;
}

int main(int argc, char** argv) {
    // need at least one argument with the keysize
    if (argc < 2) {
        std::cerr << "Usage: rsa-encrypt <public-key-file>" << std::endl;
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
    RSA_PublicKey key(pKey->algorithm_identifier(), pKey->public_key_bits());
    std::cout << "N: " << key.get_n() << std::endl;

    using clock = std::chrono::system_clock;
    using sec = std::chrono::duration<double>;

    const auto before = clock::now();
    BigInt x;
    BigInt r;
    BigInt a;
    do {
        x = find_x(key.get_n());
        r = find_r(x, key.get_n());
    } while ((power_mod(x, r/2, key.get_n()) == key.get_n() - 1));
    const sec duration = clock::now() - before;

    std::cout << "p: " << gcd(power(x, r/2) - 1, key.get_n()) << std::endl;
    std::cout << "Algorithm took " << duration.count() << " seconds" << std::endl;
}