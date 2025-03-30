#ifndef STRUMOK_HPP
#define STRUMOK_HPP

#include <array>
#include <cstdint>
#include <vector>

enum class StrumokMode {
    Strumok256,
    Strumok512
};


class Strumok {
public:
    Strumok(const std::vector<uint64_t>& key, const std::vector<uint64_t>& iv, StrumokMode mode = StrumokMode::Strumok256);

    uint64_t Strm();

    static uint64_t FSM(uint64_t s, uint64_t r1, uint64_t r2);

    static uint64_t T(uint64_t x);

    static uint64_t MulAlpha(uint64_t w);

    static uint64_t MulAlphaInv(uint64_t w);

    void ShowState() const;

private:
    void NextState();

    void InitRound();

    void Shift();

    std::array<uint64_t, 16> Register;

    int t;

    uint64_t r1;
    uint64_t r2;

    uint64_t s;
};

#endif
