//
// Created by raven on 19.05.19.
//

#ifndef RSA_RSA_IMPL_REPLACEMENT_H
#define RSA_RSA_IMPL_REPLACEMENT_H

#include <botan/bigint.h>
#include <botan/numthry.h>
#include <botan/rng.h>
#include <algorithm>

using namespace Botan;

class Prime_Sieve final
{
public:
    Prime_Sieve(const BigInt& init_value, size_t sieve_size) :
            m_sieve(std::min(sieve_size, PRIME_TABLE_SIZE))
    {
        for(size_t i = 0; i != m_sieve.size(); ++i)
            m_sieve[i] = static_cast<uint16_t>(init_value % PRIMES[i]);
    }

    void step(word increment)
    {
        for(size_t i = 0; i != m_sieve.size(); ++i)
        {
            m_sieve[i] = (m_sieve[i] + increment) % PRIMES[i];
        }
    }

    bool passes(bool check_2p1 = false) const
    {
        for(size_t i = 0; i != m_sieve.size(); ++i)
        {
            /*
            In this case, p is a multiple of PRIMES[i]
            */
            if(m_sieve[i] == 0)
                return false;

            if(check_2p1)
            {
                /*
                In this case, 2*p+1 will be a multiple of PRIMES[i]
                So if potentially generating a safe prime, we want to
                avoid this value because 2*p+1 will certainly not be prime.
                See "Safe Prime Generation with a Combined Sieve" M. Wiener
                https://eprint.iacr.org/2003/186.pdf
                */
                if(m_sieve[i] == (PRIMES[i] - 1) / 2)
                    return false;
            }
        }

        return true;
    }

private:
    std::vector<uint16_t> m_sieve;
};

BigInt generate_rsa_prime_allow_small_keys(RandomNumberGenerator& keygen_rng,
                          RandomNumberGenerator& prime_test_rng,
                          size_t bits,
                          const BigInt& coprime,
                          size_t prob = 128)
{
    /*
    * The restriction on coprime <= 64 bits is arbitrary but generally speaking
    * very large RSA public exponents are a bad idea both for performance and due
    * to attacks on small d.
    */
    if(coprime <= 1 || coprime.is_even())
        throw Invalid_Argument("generate_rsa_prime_allow_small_keys coprime must be an odd positive integer");

    const size_t MAX_ATTEMPTS = 32*1024;

    while(true)
    {
        BigInt p(keygen_rng, bits);

        // Force lowest and two top bits on
        p.set_bit(bits - 1);
        p.set_bit(bits - 2);
        p.set_bit(0);

        Prime_Sieve sieve(p, bits);

        const word step = 2;

        size_t counter = 0;
        while(true)
        {
            ++counter;

            if(counter > MAX_ATTEMPTS)
            {
                break; // don't try forever, choose a new starting point
            }

            p += step;

            sieve.step(step);

            if(sieve.passes() == false)
                continue;

            /*
            * Check if p - 1 and coprime are relatively prime by computing the inverse.
            *
            * We avoid gcd here because that algorithm is not constant time.
            * Modular inverse is (for odd modulus anyway).
            *
            * We earlier verified that coprime argument is odd. Thus the factors of 2
            * in (p - 1) cannot possibly be factors in coprime, so remove them from p - 1.
            * Using an odd modulus allows the const time algorithm to be used.
            *
            * This assumes coprime < p - 1 which is always true for RSA.
            */
            BigInt p1 = p - 1;
            p1 >>= low_zero_bits(p1);
            if(inverse_mod(coprime, p1).is_zero())
            {
                BOTAN_DEBUG_ASSERT(gcd(p1, coprime) > 1);
                continue;
            }

            BOTAN_DEBUG_ASSERT(gcd(p1, coprime) == 1);

            if(p.bits() > bits)
                break;

            if(is_prime(p, prime_test_rng, prob, true))
                return p;
        }
    }
}

#endif //RSA_RSA_IMPL_REPLACEMENT_H
