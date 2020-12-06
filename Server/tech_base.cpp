#include "streamtable.h"
#include "tech_base.h"
#include "constants.h"
#include "tech.h"
#include <iostream>
#include <map>
#include <algorithm>

extern StreamTable kt;

bool comparePrice(Tech* obj1, Tech* obj2)
{
    return obj1 -> getPrice() < obj2 -> getPrice();
}

bool compareID(Tech* obj1, Tech* obj2)
{
    return obj1 -> getID() < obj2 -> getID();
}

bool compareDate(Tech* obj1, Tech* obj2)
{
    return obj1 -> getRecordTime() < obj2 -> getRecordTime();
}

bool compareSerial(Tech* obj1, Tech* obj2)
{
    return obj1 -> getSerialNumber() < obj2 -> getSerialNumber();
}

bool compareManufacturer(Tech* obj1, Tech* obj2)
{
    return obj1 -> getManufacturer() < obj2 -> getManufacturer();
}

void TechBase::addObject(Tech* object) // добавление, которое использует админ
{
    std::cin >> *object;
    push_back(object);
    std::cout << Rus("Запись успешно добавлена.") << std::endl;
}

void TechBase::addObject(Tech* object, int) // добавление с клиента
{
    object -> getRecordTime() = object -> setCurrentDate();
    object -> getRecordTime().erase(object -> getRecordTime().find(","), 1);
    push_back(object);
}

void TechBase::remObject(const uint32_t& ID)
{
    uint32_t index = -1;
    if (findObject(ID, index))
    {
        erase(begin() + index);
        std::cout << Rus("Запись успешно удалена.") << std::endl;
    }
    else
    {
        std::cout << Rus("Запись с таким ИД не найдена.") << std::endl;
    }
}

void TechBase::changeObject(const uint32_t& ID)
{
    uint32_t index = -1;
    if (findObject(ID, index))
    {
        std::cin >> *(*this)[index];
        std::cout << Rus("Запись успешно изменена.") << std::endl;
    }
    else
    {
        std::cout << Rus("Запись с таким ИД не найдена.") << std::endl;
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
    std::cout << Rus("Введите данные: ");
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

void TechBase::sort(std::string type)
{
    bool (*compare) (Tech*, Tech*);

    if (type == Rus("ИД"))
        compare = compareID;
    if (type == Rus("Дата"))
        compare = compareDate;
    if (type == Rus("Серия"))
        compare = compareSerial;
    if (type == Rus("Производитель"))
        compare = compareManufacturer;
    if (type == Rus("Цена"))
        compare = comparePrice;

    if (type == Rus("ИД") || Rus("Дата") || Rus("Серия") || Rus("Производитель") || Rus("Цена"))
        std::sort(begin(), end(), compare);
}

void TechBase::readFile()
{
    std::uint32_t ID;
    std::string type;
    std::map<std::string, Tech*> create_type = {{"Компьютер", new Computer},{"Моб.Телефон", new MobilePhone}, {"Телевизор", new TV},
                                                {"Тостер", new Toaster}, {"Кофемашина", new CoffeMaker}, {"Эл.Чайник", new ElKettle},
                                                {"Холодильник", new Fridge}, {"Кондиционер", new Conditioner}, {"Микроволновка", new Microwawe}};

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
