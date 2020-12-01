#include "streamtable.h"
#include "tech_base.h"
#include "constants.h"
#include "tech.h"
#include <iostream>
#include <map>
#include <algorithm>

extern StreamTable kt;

bool compare(const Tech* obj1, const Tech* obj2) // функция для сортировки
{
    return obj1 -> getType() < obj2 -> getType();
}

void TechBase::addObject(Tech* object)
{
    std::cin >> *object;
    push_back(object);
    std::cout << "Record successfully added." << std::endl;
}

void TechBase::remObject(const uint32_t& ID)
{
    uint32_t index = -1;
    if (findObject(ID, index))
    {
        erase(begin() + index);
        std::cout << "Record successfully deleted." << std::endl;
    }
    else
    {
        std::cout << "Record with this ID not found." << std::endl;
    }
}

void TechBase::changeObject(const uint32_t& ID)
{
    uint32_t index = -1;
    if (findObject(ID, index))
    {
        std::cin >> *(*this)[index];
        std::cout << "Data of this record is successuly changed." << std::endl;
    }
    else
    {
        std::cout << "Record with this ID not found." << std::endl;
    }
}

bool TechBase::findObject(const uint32_t& ID, std::uint32_t& index)
{
    for (const auto& obj : *this)
    {
        index++;
        if (obj -> getID() == ID)
            return true;
    }

    return false;
}

bool TechBase::findObject(const uint32_t& ID)
{
    for (const auto& obj : *this)
    {
        if (obj -> getID() == ID)
            return true;
    }

    return false;
}

void TechBase::filltration(const std::string &data)
{
    system("cls");
    kt.firstCell(true);
    std::cout << "Enter data: ";
    for (char ch : data)
    {
        putchar(ch);
    }

    for (const auto& obj : *this)
    {
        if (!obj -> getType().compare(0, data.size(), data))
        {
            std::cout << *obj;
            continue;
        }
        if (!obj -> getReleaseData().compare(0, data.size(), data))
        {
            std::cout << *obj;
            continue;
        }

        if (!obj -> getRecordTime().compare(0, data.size(), data))
        {
            std::cout << *obj;
            continue;
        }

        if (!obj -> getSerialNumber().compare(0, data.size(), data))
        {
            std::cout << *obj;
            continue;
        }

        if (!obj -> getManufacturer().compare(0, data.size(), data))
        {
            std::cout << *obj;
            continue;
        }

        if (!obj -> getModel().compare(0, data.size(), data))
        {
            std::cout << *obj;
            continue;
        }

        if (!obj -> getVendor().compare(0, data.size(), data))
        {
            std::cout << *obj;
            continue;
        }

        if (!obj -> getCountrymaker().compare(0, data.size(), data))
        {
            std::cout << *obj;
            continue;
        }
    }
}

void TechBase::sort()
{
    std::sort(begin(), end(), compare);
}

void TechBase::readFile() // НЕ ДОДЕЛАНО
{
    std::uint32_t ID;
    std::string type;
    std::map<std::string, Tech*> create_type = {{"Computer", new Computer},{"Mobile Phone", new MobilePhone}, {"TV", new TV}, {"Toaster", new Toaster},
                                                {"Coffee Maker", new CoffeMaker}, {"Electric Kottle", new ElKettle}, {"Fridge", new Fridge},
                                                {"Conditioner", new Conditioner}, {"Microwave", new Microwawe}};

    m_fstream.open(m_filename + ".txt", std::ios_base::in);

    while (m_fstream >> ID)
    {
        if (m_fstream.eof()) break;
        std::getline(m_fstream, type);
        std::getline(m_fstream, type);
        Tech* obj = create_type[type][0].getTypeClass();
        obj -> getID() = ID;
        obj -> getType() = type;
        obj -> readFile(m_fstream, obj);
        push_back(obj);
    }

    m_fstream.close();
}

void TechBase::rewriteDB()
{
    m_fstream.open(m_filename + ".txt", std::ios_base::out);
    for (const auto& object : *this)
    {
        object -> writeToFile(m_fstream);
    }
    m_fstream.close();
}

std::ostream& operator<<(std::ostream& out, const TechBase& m_base)
{
    for (const auto& obj : m_base)
    {
        kt.firstCell(true);
        std::cout << *obj;
    }

    return out;
}
