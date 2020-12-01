#include "streamtable.h"
#include "constants.h"
#include <cstdint>
#include <vector>
#include <windows.h>

extern StreamTable mt;

void getServerMenu(uint32_t count)
{
    system("cls");
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    mt.firstCell(true);

    std::vector<std::string> items = {"Account management", "Tech Database management", "Turn off the server"};

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

    std::vector<std::string> items = {"Add account", "Delete account", "Change Account Data", "Print all accounts", "Find account", "Sort accounts", "Open file", "Back" };

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

    std::vector<std::string> items = {"Create database", "Connect database", "Delete database", "Change database", "Print all databases", "Sort by name", "Back"};

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

    std::vector<std::string> items = {"Add record", "Delete record", "Change record Data", "Print all records", "Find record", "Sort records", "Open file", "Back"};

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

    std::vector<std::string> items = {"Computer", "Mobile Phone", "TV", "Toaster", "Coffee maker", "Electric kettle", "Fridge", "Conditioner", "Microwave"};

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
