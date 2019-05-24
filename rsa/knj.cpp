//
// Created by raven on 24.05.19.
//

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
#include <gmp.h>

using namespace Botan;


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

    char buffer[100];
    mpz_t n;
    mpz_init (n);
    mpz_set_str(n, key.get_n().to_dec_string().c_str(), 10);
    mpz_t root;
    mpz_init (root);
    mpz_sqrt (root, n);
    mpz_get_str(buffer, 10, root);
    //std::cout << buffer << std::endl;
    mpz_t mod;
    mpz_init (mod);
    mpz_t two ;
    mpz_init (two);
    mpz_set_si(two, 2);
    mpz_mod(mod, root, two);
    if (mpz_sgn(mod) == 0) {
        mpz_set_si(two, 1);
        mpz_add(root, root, two);
    }
    mpz_t y;
    mpz_init (y);
    do {
        if (mpz_probab_prime_p(root, 15) == 2) {
            if (mpz_divisible_p(n, root)) {
                mpz_div(y, n, root);
                mpz_get_str(buffer, 10, y);
                std::cout << "y: " << buffer;
                mpz_get_str(buffer, 10, root);
                std::cout << " x: " << buffer << std::endl;
            }
        }
        mpz_set_si(two, 2);
        mpz_sub(root, root, two);
    } while (!mpz_divisible_p(n, root));
}