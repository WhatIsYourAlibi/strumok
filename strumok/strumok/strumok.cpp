#include "strumok.h"
#include "strumok_tables/strumok_tables.h"
#include <stdexcept>
#include <vector>
#include <cstdint>
#include <array>
#include <iostream>

Strumok::Strumok(const std::vector<uint64_t>& key, const std::vector<uint64_t>& iv, const StrumokMode mode)
    : Register{}, r1(0), r2(0), s(0) {
    if (mode == StrumokMode::Strumok256) {
        if (key.size() != 4 || iv.size() != 4)
            throw std::invalid_argument("Strumok-256 requires 256-bit key and IV");
        Register[15] = ~key[0];
        Register[14] = key[1];
        Register[13] = ~key[2];
        Register[12] = key[3];
        Register[11] = key[0];
        Register[10] = ~key[1];
        Register[9] = key[2];
        Register[8] = key[3];

        Register[7] = ~key[0];
        Register[6] = ~key[1];
        Register[5] = key[2];
        Register[4] = key[3];
        Register[3] = key[0];
        Register[2] = key[1];
        Register[1] = key[2];
        Register[0] = key[3];

        Register[5] ^= iv[3];
        Register[3] ^= iv[2];
        Register[2] ^= iv[1];
        Register[0] ^= iv[0];
    }
    else if (mode == StrumokMode::Strumok512) {
        if (key.size() != 8 || iv.size() != 4)
            throw std::invalid_argument("Strumok-512 requires 512-bit key and IV");
        Register[15] = key[0];
        Register[14] = ~key[1];
        Register[13] = key[2];
        Register[12] = key[3];
        Register[11] = ~key[7];
        Register[10] = key[5];
        Register[9] = ~key[6];
        Register[8] = key[4];

        Register[7] = ~key[0];
        Register[6] = key[1];
        Register[5] = key[2];
        Register[4] = key[3];
        Register[3] = key[4];
        Register[2] = key[5];
        Register[1] = key[6];
        Register[0] = key[7];

        Register[8] ^= iv[3];
        Register[5] ^= iv[2];
        Register[3] ^= iv[1];
        Register[0] ^= iv[0];
    }

    for (int i = 0; i < 32; i++) InitRound();
    t = -1;
}

uint64_t Strumok::Strm() {
    ++t;
    NextState();
    return (Register[0] ^ FSM(Register[15], r1, r2));
}

void Strumok::InitRound() {
    // Previous state
    const uint64_t x = MulAlpha(Register[0]) ^ Register[13] ^ MulAlphaInv(Register[11]) ^ FSM(Register[15], r1, r2);
    const uint64_t r2_temp = r2;
    r2 = T(r1);
    r1 = (r2_temp + Register[13]);

    // Current state
    Shift();
    Register[15] = x;
}

void Strumok::NextState() {
    // Previous state
    s = MulAlpha(Register[0]) ^ Register[13] ^ MulAlphaInv(Register[11]);
    const uint64_t r2_temp = r2;
    r2 = T(r1);
    r1 = (r2_temp + Register[13]);

    // Current state
    Shift();
    Register[15] = s;
}

uint64_t Strumok::FSM(const uint64_t s, const uint64_t r1, const uint64_t r2) {
    return ((s + r1) ^ r2);
}

void Strumok::Shift() {
    for (int i = 0; i < 15; i++) Register[i] = Register[i + 1];
}

uint64_t Strumok::T(const uint64_t x) {
    return ((strumok::T0[x & 0xff]) ^
        (strumok::T1[(x >> 8) & 0xff]) ^
        (strumok::T2[(x >> 16) & 0xff]) ^
        (strumok::T3[(x >> 24) & 0xff]) ^
        (strumok::T4[(x >> 32) & 0xff]) ^
        (strumok::T5[(x >> 40) & 0xff]) ^
        (strumok::T6[(x >> 48) & 0xff]) ^
        (strumok::T7[(x >> 56) & 0xff]));
}

uint64_t Strumok::MulAlpha(const uint64_t w) {
    return (((w) << 8) ^ (strumok::alpha_mul[w >> 56]));
}

uint64_t Strumok::MulAlphaInv(const uint64_t w) {
    return (((w) >> 8) ^ (strumok::alpha_inv_mul[w & 0xff]));
}

void Strumok::ShowState() const {
    for (int i = 0; i < 16; i++) {
        std::cout << "S" << std::dec << t + i << ": " << std::hex << Register[i] << std::endl;
    }
    std::cout << "R1_" << std::dec << t << ": " << std::hex << r1 << std::endl;
    std::cout << "R2_" << std::dec << t << ": " << std::hex << r2 << std::endl;
}
