#include "sysfunction.h"
#include <string>
#include <cstdarg>
#include <cstring>

void getWords(int count, std::string str, ...)
{
    va_list list;

    va_start(list, str);

    for (int arg = 0; arg < count - 1; arg++)
    {
        int n = str.find(' ');
        std::strcpy(va_arg(list, char*), str.substr(0, str.find(' ')).c_str());
        str.erase(0, n + 1);
    }

    va_end(list);
}
