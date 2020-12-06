#ifndef TECH_BASE_H
#define TECH_BASE_H

#include "tech.h"
#include <string>
#include <vector>
#include <fstream>

class TechBase : public std::vector<Tech*>
{
public:
    TechBase(const std::string& filename, const std::string& password) : m_filename(filename), m_password(password) {}
    ~TechBase() {}

    void readFile();
    void rewriteDB();
    void setFilename(const std::string& filename) { m_filename = filename; }
    void setPassword(const std::string& password) { m_password = password; }
    std::string getFilename() const { return m_filename; }
    std::string getPassword() const { return m_password; }

    void addObject(Tech* object);
    void addObject(Tech* object, int);
    void remObject(const std::uint32_t& ID);
    void changeObject(const std::uint32_t& ID);
    void filltration(const std::string& data);
    void sort(std::string type);
    bool findObject(const std::uint32_t& ID, std::uint32_t& index); // вспомогательная функция для remObject и changeObject
    bool findObject(const std::uint32_t& ID);

    friend std::ostream& operator<<(std::ostream& out, const TechBase& object);

private:
    std::fstream m_fstream;
    std::string m_filename;
    std::string m_password;
};

#endif // TECH_BASE_H
