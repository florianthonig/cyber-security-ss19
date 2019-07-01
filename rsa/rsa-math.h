//
// Created by Florian on 01.07.19.
//

#ifndef RSA_RSA_MATH_H
#define RSA_RSA_MATH_H

#include <vector>
#include "bigint.h"

// continued fraction of a two integer numbers x/y
std::vector<int> euclid_continued_fraction(int x, int y) {
    std::queue<int> queue;
    // add x and y to the queue
    queue.push(x);
    queue.push(y);

    std::vector<int> a_values;
    std::vector<int> r_values;
    for (;;) {
        int to_divide = queue.front();
        queue.pop();
        int quo = to_divide / y; // integer division
        int r_temp = to_divide - quo; // modulo
        if (!r_temp) { // exit when the division is full
            break;
        }
        a_values.push_back(quo);
        r_values.push_back(r_temp);
        queue.push(r_temp);
    }
    return a_values;
}

// continued fraction of a two integer numbers x/y
std::vector<bigint> euclid_continued_fraction(bigint x, bigint y) {
    std::vector<bigint> a_values;
    std::vector<bigint> r_values;
    a_values.push_back(x / y);
    r_values.push_back(x % y);
    if (r_values[0] > y) {
        return a_values;
    }
    a_values.push_back(y / r_values[0]);
    r_values.push_back(y % r_values[0]);
    if (r_values[1] > r_values[0]) {
        return a_values;
    }
    int i = 1;
    do {
        i++;
        a_values.push_back(r_values[i-2] / r_values[i-1]);
        r_values.push_back(r_values[i-2] % r_values[i-1]);
    } while (r_values.back() != bigint(0));
    return a_values;
}

// of the form k / l with k,l
typedef std::pair<bigint, bigint> fraction_t;

std::vector<fraction_t> calculate_fractions(const std::vector<bigint> &a_values) {
    std::vector<fraction_t> ret;
    if (a_values.size() > 1) {
        ret.emplace_back(a_values[0], 1);
    } else {
        return ret;
    }
    if (a_values.size() > 2) {
        ret.emplace_back(a_values[0] * a_values[1] + 1, a_values[1]);
    } else {
        return ret;
    }
    for (size_t i = 2; i < a_values.size(); i++) {
        ret.emplace_back(a_values[i]*ret[i-1].first + ret[i-2].first, a_values[i]*ret[i-1].second + ret[i-2].second);
    }
    return ret;
}

bigint square_root( bigint N )
{
    bigint n, p, low, high;
    if( bigint(2) > N )
        return( N );
    low  = 0;
    high = N;
    while( high > low + 1 )
    {
        n = (high + low) / 2;
        p = n * n;
        if( N < p )
            high = n;
        else if( N > p )
            low = n;
        else
            break;
    }
    return( N == p ? n : low );
}

#endif //RSA_RSA_MATH_H
