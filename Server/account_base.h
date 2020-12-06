#ifndef ACCOUNT_BASE_H
#define ACCOUNT_BASE_H

#include "account.h"
#include "incryption.h"
#include <string>
#include <vector>
#include <fstream>

class AccountBase : public std::vector<Account*>
{
public:
    AccountBase(const std::string& filename) : m_filename(filename) {}
    ~AccountBase() {}
    /* конструктор с параметром {filename} для задания имени файлу хранения данных */

    void readFile();
    void rewrite();
    void setFilename(const std::string& filename) { m_filename = filename; }
    std::string getFilename() const { return m_filename; }

    void addObject(Account* object);
    void remObject(const std::string& login);
    void changeObject(const std::string& login);
    void filltration(const std::string& data);
    void sort();
    bool findObject(const std::string& login, std::uint32_t& index); // вспомогательная функция для remObject и changeObject
    bool findObject(const std::string& login);
    bool findObject(const std::string& login, const std::string& password);

    friend std::ostream& operator<<(std::ostream& out, const AccountBase& object);

private:
    std::fstream m_fstream;
    std::string m_filename;
    VigenereCipher msg;
};

#endif // ACCOUNT_BASE_H
