#ifndef DBNAMES_BASE_H
#define DBNAMES_BASE_H

#include "tech_base.h"

class DbNamesDatabase : public std::vector<TechBase*> // наследуем от вектора для удобного хранения данных + возможности дополнить базу необходимыми методами
{
public:
    DbNamesDatabase(const std::string& filename) : m_filename(filename) {}
    ~DbNamesDatabase() {}
    /* конструктор с параметром {filename} для задания имени файлу хранения данных */

    void readFile();
    void rewriteDB();
    void setFilename(const std::string& filename) { m_filename = filename; }
    std::string getFilename() const { return m_filename; }
    /* Функции для работы непосредственно с файлом: чтение из файла, перезапись файла, получение имени файла и задание имени файла */

    void addDb(TechBase* object);
    void remDb(const std::string& filename);
    void changeDbName(const std::string& filename);
    void sort();
    bool findDbName(const std::string& filename, uint32_t& index);
    bool findDbName(const std::string& filename, const std::string& password);
    bool findDbName(const std::string& filename);
    /* функции работы с вектором: добавление записи, удаление записи, редактирование записи, фильтрация данных, сортировка */

    friend std::ostream& operator<<(std::ostream& out, const DbNamesDatabase& object); // дружественная функция для моментального вывода всей БД

private:
    std::fstream m_fstream;
    std::string m_filename;
};

#endif // DBNAMES_BASE_H
