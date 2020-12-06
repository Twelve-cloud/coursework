#include "streamtable.h"
#include "dbnames_base.h"
#include "tech.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <QString>

extern StreamTable nt;
#define Rus(str) QString::fromUtf8(str).toLocal8Bit().data()

bool compare(const TechBase* obj1, const TechBase* obj2) // функция для сортировки
{
    return obj1 -> getFilename() < obj2 -> getFilename();
}

void DbNamesDatabase::addDb(TechBase* object)
{
    push_back(object);
    std::cout << Rus("База данных успешно добавлена.") << std::endl;
}

void DbNamesDatabase::remDb(const std::string& filename)
{
    uint32_t index = -1;
    if (findDbName(filename, index))
    {
        erase(begin() + index);
        remove((filename + ".txt").c_str());
        std::cout << Rus("База данных успешно удалена.") << std::endl;
    }
    else
    {
        std::cout << Rus("Базы данных с таким именем не существует.") << std::endl;
    }
}

void DbNamesDatabase::changeDbName(const std::string& filename)
{
    uint32_t index = -1;
    if (findDbName(filename, index))
    {
        std::string dbname, dbpass;
        std::cout << Rus("Введите новое имя: "); std::getline(std::cin, dbname);
        std::cout << Rus("Введите новый пароль: "); std::getline(std::cin, dbpass);
        (*this)[index] -> setFilename(dbname);
        (*this)[index] -> setPassword(dbpass);
        rename((filename + ".txt").c_str(), (dbname + ".txt").c_str());
        std::cout << Rus("Данные БД успешно изменены.") << std::endl;
    }
    else
    {
        std::cout << Rus("База данных с таким именем не существует.") << std::endl;
    }
}

bool DbNamesDatabase::findDbName(const std::string& filename, uint32_t& index)
{
    for (const auto& obj : *this)
    {
        index++;
        if (obj -> getFilename() == filename)
            return true;
    }

    return false;
}

bool DbNamesDatabase::findDbName(const std::string& filename, const std::string& password)
{
    for (const auto& obj : *this)
    {
        if (obj -> getFilename() == filename && obj -> getPassword() == password)
            return true;
    }

    return false;
}

bool DbNamesDatabase::findDbName(const std::string& filename)
{
    for (const auto& obj : *this)
    {
        if (obj -> getFilename() == filename)
            return true;
    }

    return false;
}

void DbNamesDatabase::sort()
{
    std::sort(begin(), end(), compare);
}

void DbNamesDatabase::readFile()
{
    std::string dbname, dbpass;
    uint32_t count = -1;

    m_fstream.open(m_filename, std::ios_base::in);

    while (m_fstream >> dbname >> dbpass)
    {
        count++;
        TechBase* object = new TechBase(dbname, dbpass);
        push_back(object);
        (*this)[count] -> readFile();
        if (m_fstream.eof()) break;
    }

    m_fstream.close();
}

void DbNamesDatabase::rewriteDB()
{
   m_fstream.open(m_filename, std::ios_base::out);

   for (const auto& obj : *this)
   {
       m_fstream << obj -> getFilename() << std::endl;
       m_fstream << obj -> getPassword() << std::endl;
   }
   m_fstream.close();
}

std::ostream& operator<<(std::ostream& out, const DbNamesDatabase& m_base)
{
    nt.firstCell(true);
    for (const auto& obj : m_base)
    {
        nt << obj -> getFilename() << obj -> getPassword();
    }

    return out;
}
