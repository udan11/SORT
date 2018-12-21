#ifndef __XORSHIFT_H__
#define __XORSHIFT_H__

#include <stdint.h>

/**
 * State of the XorShift128 PRNG.
 */
typedef uint32_t xorshift128_t[4];

/**
 * Generates 32 random bits and advances generator's state.
 *
 * @param state State of the XorShift128 generator.
 *
 * @return Random bits.
 */
inline uint32_t xorshift128(xorshift128_t state)
{
    uint32_t s, t = state[3];
    t ^= t << 11;
    t ^= t >> 8;
    state[3] = state[2];
    state[2] = state[1];
    state[1] = s = state[0];
    t ^= s;
    t ^= s >> 19;
    state[0] = t;
    return t;
}

#endif /* __XORSHIFT_H__*/
