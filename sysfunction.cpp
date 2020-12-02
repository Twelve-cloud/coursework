#include <string>
#include <cstdarg>
#include <cstring>

std::string getWord(std::string str, std::string& field)
{

    int n = str.find(' ');
    field = str.substr(0, str.find(' '));
    str.erase(0, n + 1);

    return str;
}
