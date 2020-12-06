#include "sysfunction.h"
#include "account.h"
#include "tech_base.h"
#include "tech.h"
#include <string>
#include <cstdarg>
#include <algorithm>
#include <cstring>

std::string getWords(int count, std::string str, ...)
{
    va_list list;

    va_start(list, str);

    for (int arg = 0; arg < count - 1; arg++)
    {
        int n = str.find(' ');
        if (n != -1)
            std::replace(str.begin(), str.begin() + n - 1, '|', ' ');
        std::strcpy(va_arg(list, char*), str.substr(0, n).c_str());
        str.erase(0, n + 1);
    }

    va_end(list);

    return str;
}

uint32_t checkNegative(const char* str)
{
    int32_t posivite = atoi(str);
    if (posivite < 0)
        return 0;

    return posivite;
}
