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
    Strumok(std::vector<uint64_t> key, std::vector<uint64_t> iv, StrumokMode mode = StrumokMode::Strumok256);
    uint64_t next();

private:
    void init();

    void nextState();

    void initRound();

    void shift();

    uint64_t FSM(uint64_t s, uint64_t r1, uint64_t r2);

    uint64_t T(uint64_t input);

    uint64_t mulAlpha(uint64_t w);

    uint64_t mulAlphaInv(uint64_t w);

private:
    std::array<uint64_t, 16> Register;

    uint64_t r1;
    uint64_t r2;

    uint64_t s;

    std::array<uint64_t, 4> K;
    std::array<uint64_t, 4> IV;
};

#endif
