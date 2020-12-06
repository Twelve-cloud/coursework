#ifndef DBNAMES_BASE_H
#define DBNAMES_BASE_H

#include "tech_base.h"

class DbNamesDatabase : public std::vector<TechBase*>
{
public:
    DbNamesDatabase(const std::string& filename) : m_filename(filename) {}
    ~DbNamesDatabase() {}
    /* конструктор с параметром {filename} для задания имени файлу хранения данных */

    void readFile();
    void rewriteDB();
    void setFilename(const std::string& filename) { m_filename = filename; }
    std::string getFilename() const { return m_filename; }
    TechBase* getDB(uint32_t index) { return (*this)[index]; }


    void addDb(TechBase* object);
    void remDb(const std::string& filename);
    void changeDbName(const std::string& filename);
    void sort();
    bool findDbName(const std::string& filename, uint32_t& index); // вспомогательная
    bool findDbName(const std::string& filename, const std::string& password);
    bool findDbName(const std::string& filename);

    friend std::ostream& operator<<(std::ostream& out, const DbNamesDatabase& object);

private:
    std::fstream m_fstream;
    std::string m_filename;
};

#endif // DBNAMES_BASE_H
