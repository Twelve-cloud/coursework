#ifndef TECH_BASE_H
#define TECH_BASE_H

#include "tech.h"
#include <string>
#include <vector>
#include <fstream>

class TechBase : public std::vector<Tech*> // наследуем от вектора для удобного хранения данных + возможности дополнить базу необходимыми методами
{
public:
    TechBase(const std::string& filename, const std::string& password) : m_filename(filename), m_password(password) {}
    ~TechBase() {}
    /* конструктор с параметром {filename} для задания имени файлу хранения данных */

    void readFile();
    void rewriteDB();
    void setFilename(const std::string& filename) { m_filename = filename; }
    void setPassword(const std::string& password) { m_password = password; }
    std::string getFilename() const { return m_filename; }
    std::string getPassword() const { return m_password; }
    /* Функции для работы непосредственно с файлом: чтение из файла, перезапись файла, получение имени файла и задание имени файла */

    void addObject(Tech* object);
    void addObject(Tech* object, int);
    void remObject(const std::uint32_t& ID);
    void changeObject(const std::uint32_t& ID);
    void filltration(const std::string& data);
    void sort();
    bool findObject(const std::uint32_t& ID, std::uint32_t& index); // вспомогательная функция для remObject и changeObject
    bool findObject(const std::uint32_t& ID);
    /* функции работы с вектором: добавление записи, удаление записи, редактирование записи, фильтрация данных, сортировка */

    friend std::ostream& operator<<(std::ostream& out, const TechBase& object); // дружественная функция для моментального вывода всей БД

private:
    std::fstream m_fstream;
    std::string m_filename;
    std::string m_password;
};

#endif // TECH_BASE_H
