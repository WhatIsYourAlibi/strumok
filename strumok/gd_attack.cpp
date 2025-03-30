#include <fstream>
#include <iostream>
#include <unordered_map>
#include <vector>
#include "strumok/strumok.h"
#include "strumok/strumok_tables/strumok_tables.h"
#define u64t uint64_t

std::unordered_map<uint64_t, uint64_t> invT;

std::vector<uint64_t> key = {
    0x0000000000000000, 0x0000000000000000,
    0x0000000000000000, 0x8000000000000000
};

std::vector<uint64_t> iv = {
    0x0000000000000001, 0x0000000000000002,
    0x0000000000000003, 0x0000000000000004
};

Strumok cipher(key, iv);

void RestoreState(std::vector<u64t>& S, std::vector<u64t>& R, const std::vector<u64t>& z) {
    // First step
    R[8] = Strumok::T(S[19] + R[6]);
    R[7] = Strumok::T(S[18] + R[5]);
    S[6] = Strumok::MulAlphaInv(S[22] ^ S[19] ^ Strumok::MulAlphaInv(S[17]));
    // Second step
    R[9] = Strumok::T(S[20] + R[7]);
    S[7] = z[7] ^ Strumok::FSM(S[22], R[6] + S[19], R[7]);
    S[21] = (z[6] ^ S[6] ^ R[6]) - (R[5] + S[18]);
    // Third step
    R[10] = Strumok::T(S[21] + R[8]);
    R[11] = Strumok::T(S[22] + R[9]);
    S[23] = S[20] ^ Strumok::MulAlphaInv(S[18]) ^ Strumok::MulAlpha(S[7]);
    S[16] = Strumok::MulAlpha(S[21] ^ S[18] ^ Strumok::MulAlpha(S[5]));
    S[15] = Strumok::MulAlpha(S[20] ^ S[17] ^ Strumok::MulAlpha(S[4]));
    // Fourth step
    R[12] = Strumok::T(S[23] + R[10]);
    S[8] = z[8] ^ Strumok::FSM(S[23], R[7] + S[20], R[8]);
    // Fifth step
    S[24] = S[21] ^ Strumok::MulAlphaInv(S[19]) ^ Strumok::MulAlpha(S[8]);
    // Sixth step
    S[9] = z[9] ^ Strumok::FSM(S[24], R[8] + S[21], R[9]);
    // Seventh step
    S[25] = S[22] ^ Strumok::MulAlphaInv(S[20]) ^ Strumok::MulAlpha(S[9]);
    // Eighth step
    S[10] = z[10] ^ Strumok::FSM(S[25], R[9] + S[22], R[10]);
    // Ninth step
    S[26] = S[23] ^ Strumok::MulAlphaInv(S[21]) ^ Strumok::MulAlpha(S[10]);

    // Additional steps, not from the generated graph
    S[11] = z[11] ^ Strumok::FSM(S[26], R[10] + S[23], R[11]);
    S[27] = S[24] ^ Strumok::MulAlphaInv(S[22]) ^ Strumok::MulAlpha(S[11]);

    S[12] = z[12] ^ Strumok::FSM(S[27], R[11] + S[24], R[12]);
    S[28] = S[25] ^ Strumok::MulAlphaInv(S[23]) ^ Strumok::MulAlpha(S[12]);

    R[13] = Strumok::T(S[24] + R[11]);
    S[13] = z[13] ^ Strumok::FSM(S[28], R[12] + S[25], R[13]);
    S[29] = S[26] ^ Strumok::MulAlphaInv(S[24]) ^ Strumok::MulAlpha(S[13]);

    R[14] = Strumok::T(S[25] + R[12]);
    S[14] = z[14] ^ Strumok::FSM(S[29], R[13] + S[26], R[14]);
    S[30] = S[27] ^ Strumok::MulAlphaInv(S[25]) ^ Strumok::MulAlpha(S[14]);
}

int main() {
    const std::vector<u64t> Z = {
        0xfe44a2508b5a2acd, 0xaf355b4ed21d2742, 0xdcd7fdd6a57a9e71, 0x5d267bd2739fb5eb, 0xb22eee96b2832072,
        0xc7de6a4cdaa9a847, 0x72d5da93812680f2, 0x4a0acb7e93da2ce0, 0xa8d2064f319678c4, 0x433f87b11a94c349,
        0xf143c3411fbab1ec, 0xa2b17537cfb0984d, 0x5b00072536bcebb3, 0x57a59ee3e14ac5b7, 0xda7448a47e4574d6
    }; // Known values of gamma from 0 to 14

    std::vector<u64t> S(31, 0);
    std::vector<u64t> R(15, 0);

    // Our guessed values for given basis
    S[22] = 0x874f28ca919b4b89;
    S[20] = 0x0f0b7d3e96245639;
    S[19] = 0x80952427ada3a3c0;
    S[18] = 0x2361a5a4def28a8d;
    S[17] = 0xcfdbe9be1a6a21cf;
    R[6] = 0x0b43dada7eb4e162;
    R[5] = 0xe80fd8403b4fc8e5;

    RestoreState(S, R, Z);

    std::cout << "S vector state:\n";
    for (short i = 6; i < S.size(); ++i) {
        std::cout << std::dec << "S[" << i << "] = " << std::hex << S[i] << "\n";
    }
    std::cout << std::endl << "R vector state:\n";
    std::cout << std::dec << "R2[" << 5 << "] = " << std::hex << R[5] << "\n";
    for (short i = 6; i < R.size(); ++i) {
        std::cout << std::dec << "R1[" << i << "] = " << std::hex << R[i - 1] + S[i + 12] << "\n";
        std::cout << std::dec << "R2[" << i << "] = " << std::hex << R[i] << "\n";
    }
    std::cout << std::endl;

    for (int i = 0; i < 15; ++i) {
        cipher.Strm();
    }
    cipher.ShowState();

    return 0;
}
