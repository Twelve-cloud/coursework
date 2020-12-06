#ifndef INCRYPTION_H
#define INCRYPTION_H

#include <QString>
#include <cctype>
#include <cstdint>

using std::uint32_t;

class VigenereCipher //шифровка виженером для чисел и английских букв
{
public:
    VigenereCipher();
    void incrypt(const std::string& keyword, std::string& message);
    void decrypt(const std::string& keyword, std::string& message);
    char findLetter(uint32_t j, char sym);

private:
    enum Letter
    {
        ZERO, ONE, TWO, THREE,
        FOUR, FIVE, SIX, SEVEN,
        EIGHT, NINE, A, B, C, D,
        E, F, G, H, I, J, K, L,
        M, N, O, P, Q, R, S, T,
        U, V, W, X, Y, Z, MAX_SYM
    };
    uint32_t matrix[MAX_SYM][MAX_SYM];
};

#endif // INCRYPTION_H
