#include <iostream>
#include <vector>
#include <iomanip>
#include "strumok/strumok.h"

void testKeyStream() {
    std::vector<uint64_t> key = {
        0x0000000000000000, 0x0000000000000000,
        0x0000000000000000, 0x8000000000000000
    };

    std::vector<uint64_t> iv = {
        0x0000000000000000, 0x0000000000000000,
        0x0000000000000000, 0x0000000000000000
    };

    std::vector<uint64_t> expected = {
        0xe442d15345dc66ca, 0xf47d700ecc66408a,
        0xb4cb284b5477e641, 0xa2afc9092e4124b0,
        0x728e5fa26b11a7d9, 0xe6a7b9288c68f972,
        0x70eb3606de8ba44c, 0xaced7956bd3e3de7
    };

    Strumok cipher(key, iv, StrumokMode::Strumok256);

    std::cout << "Test Key Stream\n";
    bool ok = true;
    for (size_t i = 0; i < expected.size(); ++i) {
        uint64_t actual = cipher.next();
        std::cout << "Z" << i << ": " << std::hex << std::setw(16) << std::setfill('0') << actual;

        if (actual != expected[i]) {
            std::cout << "  \033[1;31m[wrong, expected: " << expected[i] << "]\033[0m";
            ok = false;
        }
        else {
            std::cout << "  \033[1;32m[ok]\033[0m";
        }
        std::cout << "\n";
    }

    std::cout << std::dec;
    if (ok)
        std::cout << "\n\033[1;32m[+] Test passed.\033[0m\n\n";
    else
        std::cout << "\n\033[1;31m[-] Test failed.\033[0m\n\n";
}

int main() {
    testKeyStream();
    return 0;
}
