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

using namespace Botan;

BigInt f(BigInt x, BigInt c) {
    return x * x + c;
}

BigInt pollard(BigInt &x0, BigInt &c, const BigInt &n) {
    BigInt x1 = f(x0, c) % n;
    BigInt x2 = f(x1, c) % n;

    BigInt tmp = gcd(x2-x1, n);
    int32_t i = 0;
    while (tmp == 1) {
        x1 = f(x1, c) % n;
        x2 = f(f(x2, c), c) % n;
        tmp = gcd(x2-x1, n);
        i++;
        if (i % 500 == 0) {
            std::cout << "i: " << i << std::endl;
        }
    }
    return tmp;
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

    BigInt x0(system_rng(), key.get_n().bits());
    while ( x0 > key.get_n()) {
        x0 = BigInt(system_rng(), key.get_n().bits());
    }
    BigInt c(3);
    BigInt res = pollard(x0, c, key.get_n());
    while (res == 0) {
        while ( x0 > key.get_n()) {
            x0 = BigInt(system_rng(), key.get_n().bits());
        }
        res = pollard(x0, c, key.get_n());
    }
    std::cout << res.to_dec_string() << " x0:" << x0 << std::endl;
}