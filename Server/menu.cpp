#include "streamtable.h"
#include "constants.h"
#include <cstdint>
#include <vector>
#include <QString>
#include <windows.h>

extern StreamTable mt;
#define Rus(str) QString::fromUtf8(str).toLocal8Bit().data()

void getServerMenu(uint32_t count)
{
    system("cls");
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    mt.firstCell(true);

    std::vector<std::string> items = {Rus("Управление аккаунтами"), Rus("Управление базами данных"), Rus("Выключить сервер")};

    for (uint32_t i = 0; i < items.size(); i++)
    {
        if (i == count - 1)
        {
            SetConsoleTextAttribute(hConsole, (WORD)((Color::LIGHT_BLUE << Color::RED) | Color::WHITE));
            mt << items[i];
            SetConsoleTextAttribute(hConsole, (WORD)((Color::BLACK << Color::RED) | Color::LIGHT_BLUE));
        }
        else
        {
            mt << items[i];
        }
    }
}

void getAccountMenu(uint32_t count)
{
    system("cls");
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    mt.firstCell(true);

    std::vector<std::string> items = {Rus("Добавить аккаунт"), Rus("Удалить аккаунт"), Rus("Изменить данные аккаунта"),
                                      Rus("Вывод всех аккаунтов"), Rus("Найти аккаунт"), Rus("Сортировка"), Rus("Открыть файл"), Rus("Назад") };

    for (uint32_t i = 0; i < items.size(); i++)
    {
        if (i == count - 1)
        {
            SetConsoleTextAttribute(hConsole, (WORD)((Color::LIGHT_BLUE << Color::RED) | Color::WHITE));
            mt << items[i];
           SetConsoleTextAttribute(hConsole, (WORD)((Color::BLACK << Color::RED) | Color::LIGHT_BLUE));
        }
        else
        {
            mt << items[i];
        }
    }
}

void getTechBaseMenu(uint32_t count)
{
    system("cls");
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    mt.firstCell(true);

    std::vector<std::string> items = {Rus("Создать базу данных"), Rus("Подключиться к базе данных"), Rus("Удалить базу данных"),
                                      Rus("Изменить базу данных"), Rus("Вывод всех БД"), Rus("Сортировка по имени"), Rus("Назад")};

    for (uint32_t i = 0; i < items.size(); i++)
    {
        if (i == count - 1)
        {
            SetConsoleTextAttribute(hConsole, (WORD)((Color::LIGHT_BLUE << Color::RED) | Color::WHITE));
            mt << items[i];
            SetConsoleTextAttribute(hConsole, (WORD)((Color::BLACK << Color::RED) | Color::LIGHT_BLUE));
        }
        else
        {
            mt << items[i];
        }
    }
}

void getTechMenu(uint32_t count)
{
    system("cls");
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    mt.firstCell(true);

    std::vector<std::string> items = {Rus("Добавить запись"), Rus("Удалить запись"), Rus("Изменить запись"), Rus("Вывод всех записей"),
                                      Rus( "Найти запись"), Rus("Сортировка"), Rus("Назад")};

    for (uint32_t i = 0; i < items.size(); i++)
    {
        if (i == count - 1)
        {
            SetConsoleTextAttribute(hConsole, (WORD)((Color::LIGHT_BLUE << Color::RED) | Color::WHITE));
            mt << items[i];
            SetConsoleTextAttribute(hConsole, (WORD)((Color::BLACK << Color::RED) | Color::LIGHT_BLUE));
        }
        else
        {
            mt << items[i];
        }
    }
}

void getTypeMenu(uint32_t count)
{
    system("cls");
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    mt.firstCell(true);

    std::vector<std::string> items = {Rus("Компьютер"), Rus("Мобильный телефон"), Rus("Телевизор"), Rus("Тостер"), Rus("Кофемашина"),
                                      Rus("Электрический чайник"), Rus("Холодильник"), Rus("Кондиционер"), Rus("Микроволновка")};

    for (uint32_t i = 0; i < items.size(); i++)
    {
        if (i == count - 1)
        {
            SetConsoleTextAttribute(hConsole, (WORD)((Color::LIGHT_BLUE << Color::RED) | Color::WHITE));
            mt << items[i];
            SetConsoleTextAttribute(hConsole, (WORD)((Color::BLACK << Color::RED) | Color::LIGHT_BLUE));
        }
        else
        {
            mt << items[i];
        }
    }
}
