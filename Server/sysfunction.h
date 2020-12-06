#ifndef SYSFUNCTION_H
#define SYSFUNCTION_H

#include "account.h"
#include "tech_base.h"
#include "tech.h"
#include <iostream>
#include <iomanip>
#include <QString>

#define Rus(str) QString::fromUtf8(str).toLocal8Bit().data()
#define NRus(str) QString::fromLocal8Bit(str).toUtf8().data()
#define KEY 666

#define ENCRYPTION_CYCLE(str, key) 									\
        for(uint32_t i = 0; i < str.size(); i++)                    \
        str[i] ^= key;                                              \


std::string getWords(int count, std::string str, ...);
uint32_t checkNegative(const char* str);

template <typename T>
void getnumber(T& number, const char* message)
{
    bool isCorrect;

    do
    {
        isCorrect = true;
        std::cout << message;
        std::cin >> std::setw(20) >> number;
        if (std::cin.fail())
        {
            isCorrect = false;
            std::cin.clear();
            std::cin.ignore(32767, '\n');
            std::cout << Rus("Некорректное число. Попробуйте еще раз.") << std::endl;
        }
        else
        {
             std::cin.ignore(32767, '\n');
        }
    } while (!isCorrect);
}

#endif // SYSFUNCTION_H
