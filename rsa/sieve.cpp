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
#include <iostream>
#include <algorithm>
#include <vector>
#include <set>
#include <queue>
#include <chrono>
#include <math.h>
#include "bigint.h"

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

    using clock = std::chrono::system_clock;
    using sec = std::chrono::duration<double>;

    const auto before = clock::now();
    bigint n = bigint(key.get_n().to_dec_string().c_str());
    std::vector<bigint> f = n.factor();
    const sec duration = clock::now() - before;

    for(int i = 0; i < f.size(); i++) {
        std::cout << f[i] << endl;
    }

    std::cout << "Algorithm took " << duration.count() << " seconds" << std::endl;
}