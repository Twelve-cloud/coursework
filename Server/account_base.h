#ifndef ACCOUNT_BASE_H
#define ACCOUNT_BASE_H

#include "account.h"
#include <string>
#include <vector>
#include <fstream>

class AccountBase : public std::vector<Account*> // наследуем от вектора для удобного хранения данных + возможности дополнить базу необходимыми методами
{
public:
    AccountBase(const std::string& filename) : m_filename(filename) {}
    ~AccountBase() {}
    /* конструктор с параметром {filename} для задания имени файлу хранения данных */

    void readFile();
    void rewrite();
    void setFilename(const std::string& filename) { m_filename = filename; }
    std::string getFilename() const { return m_filename; }
    /* Функции для работы непосредственно с файлом: чтение из файла, перезапись файла, получение имени файла и задание имени файла */

    void addObject(Account* object);
    void remObject(const std::string& login);
    void changeObject(const std::string& login);
    void filltration(const std::string& data);
    void sort();
    bool findObject(const std::string& login, std::uint32_t& index); // вспомогательная функция для remObject и changeObject
    bool findObject(const std::string& login);
    bool findObject(const std::string& login, const std::string& password);
    /* функции работы с вектором: добавление записи, удаление записи, редактирование записи, фильтрация данных, сортировка */

    friend std::ostream& operator<<(std::ostream& out, const AccountBase& object); // дружественная функция для моментального вывода всей БД

private:
    std::fstream m_fstream;
    std::string m_filename;
};

#endif // ACCOUNT_BASE_H
