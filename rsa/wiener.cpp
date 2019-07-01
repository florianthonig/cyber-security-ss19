//
// Created by Florian on 01.07.19.
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
#include <iostream>
#include <algorithm>
#include <vector>
#include <set>
#include <queue>
#include <chrono>
#include <math.h>
#include "rsa-math.h"

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
    std::cout << "e: " << key.get_e() << std::endl;

    using clock = std::chrono::system_clock;
    using sec = std::chrono::duration<double>;

    const auto before = clock::now();
    bigint n = bigint(key.get_n().to_dec_string().c_str());
    bigint e = bigint(key.get_e().to_dec_string().c_str());
    // calculate continued fractions
    auto frac_list = euclid_continued_fraction(e, n);
    /*std::cout << "fractions: " << frac_list.size() << std::endl;
    for (const bigint & i : frac_list) {
        std::cout << i << ",";
    }
    std::cout << endl;*/
    auto fracs = calculate_fractions(frac_list);
    /*std::cout << "real fractions: " << fracs.size() << std::endl;
    for (const std::pair<bigint, bigint> &i : fracs) {
        std::cout << i.first << "/" << i.second << ", ";
    }
    std::cout << endl;*/
    // go through all the fractions
    for (const std::pair<bigint, bigint> &i : fracs) {
        if (i.first == bigint(0) || i.second == bigint(0)) {
            continue;
        }
        bigint phi_n = (e * i.second - 1) / i.first;
        bigint p = n - phi_n + 1; // no negative sign here
        bigint under_root = (p*p)/(2*2) - n;
        bigint root = under_root.sqrt();
        bigint p0 = p/2 + root; // here the negative sign is also skipped
        bigint q0 = p/2 - root; // here the negative sign is also skipped
        if (p0 * q0 == n) {
            std::cout << "secret key d: " << i.second << std::endl;
            std::cout << "p: " << p0<< std::endl;
            std::cout << "q: " << q0<< std::endl;
            break;
        }
    }
    const sec duration = clock::now() - before;
    std::cout << "Algorithm took " << duration.count() << " seconds" << std::endl;
}