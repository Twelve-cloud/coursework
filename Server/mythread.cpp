#include "mythread.h"
#include "sysfunction.h"
#include "constants.h"
#include "streamtable.h"
#include <iostream>
#include <cstdint>
#include <windows.h>
#include <conio.h>

void getServerMenu(std::uint32_t count);
void getAccountMenu(std::uint32_t count);
void getTechBaseMenu(uint32_t count);
void getTechMenu(uint32_t count);
void getTypeMenu(uint32_t count);

void MyThread::run()
{
    std::uint32_t count = Output::RANGE_UP, engine;
    bool isEnd;

    do
    {
        getServerMenu(count);
        isEnd = false;
        do
        {
            engine = getch();
            if (engine == Output::PAGE_UP && count > Output::RANGE_UP)
            {
                count--;
            }
            if (engine == Output::PAGE_DOWN && count < Output::SERVER_RANGE_DOWN)
            {
                count++;
            }
            getServerMenu(count);
        } while (engine != Output::ENTER);

        switch (count)
        {
            case 1:
                account_management_menu(); break;
            case 2:
                techbase_management_menu(); break;
            case 3:
                std::exit(Error::FAIL_EXEC);
            default:
                std::cout << Rus("Некорректное число. Попробуйте еще раз.") << std::endl; system("pause");
        }
    } while(!isEnd);
}

void MyThread::account_management_menu()
{
    int count = Output::RANGE_UP, engine;
    bool isBack;

    do
    {
        getAccountMenu(count);
        isBack = false;
        do
        {
            engine = getch();
            if (engine == Output::PAGE_UP && count > Output::RANGE_UP)
            {
                count--;
            }
            if (engine == Output::PAGE_DOWN && count < Output::ACCOUNT_RANGE_DOWN)
            {
                count++;
            }
            getAccountMenu(count);
        } while (engine != Output::ENTER);

        switch(count)
        {
            case 1:
            {
                m_tcpServer -> getDatabase() -> addObject(new Account);
                m_tcpServer -> getDatabase() -> rewrite();
                system("pause"); break;
            }
            case 2:
            {
                std::string login;
                std::cout << Rus("Введите логин: "); std::getline(std::cin, login);
                m_tcpServer -> getDatabase() -> remObject(login);
                m_tcpServer -> getDatabase() -> rewrite();
                system("pause"); break;
            }
            case 3:
            {
                std::string login;
                std::cout << Rus("Введите логин: "); std::getline(std::cin, login);
                m_tcpServer -> getDatabase() -> changeObject(login);
                m_tcpServer -> getDatabase() -> rewrite();
                system("pause"); break;
            }
            case 4:
            {
                system("cls");
                std::cout << *m_tcpServer -> getDatabase() << std::endl;
                system("pause"); break;
            }
            case 5:
            {
                system("cls");
                std::string target;
                std::cout << Rus("Введите данные: ");
                char c;
                do
                {
                    c = getch();
                    if (c == Output::BACKSPACE)
                    {
                        putchar(' '); putchar(c);
                        if (target.size() > 0)
                        {
                            target.erase(target.size() - 1);
                        }
                    }
                    else
                    {
                        target += c;
                    }
                    m_tcpServer -> getDatabase() -> filltration(target);
                } while (c != Output::ENTER);
                break;
            }
            case 6:
            {
                m_tcpServer -> getDatabase() -> sort();
                uint32_t c;
                do
                {
                    system("cls");
                    std::cout << Rus("Перезаписать файл? Y/N");
                    c = getch();
                } while (c != 'Y' && c != 'N');
                if (c == 'Y')
                {
                    m_tcpServer -> getDatabase() -> rewrite();
                }
                break;
            }
            case 7:
                system(m_tcpServer -> getDatabase() -> getFilename().c_str()); break;
            case 8:
                isBack = true; break;
            default:
                std::cout << Rus("Некорректное число. Попробуйте еще раз.") << std::endl; system("pause");
        }
    } while (!isBack);
}

void MyThread::techbase_management_menu()
{
    int count = Output::RANGE_UP, engine;
    bool isBack;

    do
    {
        getTechBaseMenu(count);
        isBack = false;
        do
        {
            engine = getch();
            if (engine == Output::PAGE_UP && count > Output::RANGE_UP)
            {
                count--;
            }
            if (engine == Output::PAGE_DOWN && count < Output::TECH_BASE_RANGE_DOWN)
            {
                count++;
            }
            getTechBaseMenu(count);
        } while (engine != Output::ENTER);

        switch(count)
        {
            case 1:
            {
                std::string dbname, dbpass;
                std::cout << Rus("Введите имя БД: ");     std::cin >> std::setw(15) >> dbname;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << Rus("Введите пароль: ");     std::cin >> std::setw(15) >> dbpass;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                m_tcpServer -> getTechBase() -> push_back(new TechBase(dbname, dbpass));
                m_tcpServer -> getTechBase() -> rewriteDB();
                std::ofstream createFile(dbname + ".txt"); createFile.close();
                std::cout << Rus("База данных успешно создана.") << std::endl;
                system("pause"); break;
            }
            case 2:
            {
                bool isFound = false;
                std::string dbname, dbpass;
                std::cout << Rus("Введите имя БД: ");  std::getline(std::cin, dbname);
                std::cout << Rus("Введите пароль: ");  std::getline(std::cin, dbpass);
                for (const auto& i : *(m_tcpServer -> getTechBase()))
                {
                    if (i -> getFilename() == dbname && i -> getPassword() == dbpass)
                    {
                        tech_management_menu(i);
                        isFound = true;
                        break;
                    }
                }
                if (!isFound)
                {
                    std::cout << Rus("Неверное имя или пароль") << std::endl;
                    system("pause");
                }
                break;
            }
            case 3:
            {
                std::string dbname;
                std::cout << Rus("Введите имя: "); std::getline(std::cin, dbname);
                m_tcpServer -> getTechBase() -> remDb(dbname);
                m_tcpServer -> getTechBase() -> rewriteDB();
                system("pause"); break;
            }
            case 4:
            {
                std::string dbname;
                std::cout << Rus("Введите имя: "); std::getline(std::cin, dbname);
                m_tcpServer -> getTechBase() -> changeDbName(dbname);
                m_tcpServer -> getTechBase() -> rewriteDB();
                system("pause"); break;
            }
            case 5:
            {
                system("cls");
                std::cout << *m_tcpServer -> getTechBase() << std::endl;
                system("pause"); break;
            }
            case 6:
            {
                m_tcpServer -> getTechBase() -> sort();
                uint32_t c;
                do
                {
                    system("cls");
                    std::cout << Rus("Перезаписать файл? Y/N");
                    c = getch();
                } while (c != 'Y' && c != 'N');
                if (c == 'Y')
                {
                    m_tcpServer -> getTechBase() -> rewriteDB();
                }
                break;
            }
            case 7:
                isBack = true; break;
            default:
                std::cout << Rus("Некорректное число. Попробуйте еще раз.") << std::endl; system("pause");
        }
    } while (!isBack);
}

void menu(void (*ptrMenu)(uint32_t), uint32_t RangeDown, int& count)
{
    int engine;
    ptrMenu(count);
    do
    {
        engine = getch();
        if (engine == Output::PAGE_UP && count > Output::RANGE_UP)
        {
           count--;
        }
        if (engine == Output::PAGE_DOWN && count < RangeDown)
        {
            count++;
        }
        ptrMenu(count);
    } while (engine != Output::ENTER);
}

void MyThread::tech_management_menu(TechBase* DATABASE)
{
    int count = Output::RANGE_UP, engine;
    bool isBack;
    std::vector<Tech*> create_type = {new Computer, new MobilePhone, new TV, new Toaster, new CoffeMaker, new ElKettle, new Fridge, new Conditioner, new Microwawe};

    do
    {
        getTechMenu(count);
        isBack = false;
        do
        {
            engine = getch();
            if (engine == Output::PAGE_UP && count > Output::RANGE_UP)
            {
                count--;
            }
            if (engine == Output::PAGE_DOWN && count < Output::TECH_RANGE_DOWN)
            {
                count++;
            }
            getTechMenu(count);
        } while (engine != Output::ENTER);

        switch(count)
        {
            case 1:
            {
                int countTypes = Output::RANGE_UP;
                menu(getTypeMenu, Types::TYPE_RANGE_DOWN, countTypes);
                DATABASE -> addObject(create_type[countTypes - 1] -> getTypeClass());
                DATABASE -> rewriteDB();
                system("pause"); break;
            }
            case 2:
            {
                uint32_t ID; getnumber(ID, Rus("ИД: "));
                DATABASE -> remObject(ID);
                DATABASE -> rewriteDB();
                system("pause"); break;
            }
            case 3:
            {
                uint32_t ID; getnumber(ID, Rus("ИД: "));
                DATABASE -> changeObject(ID);
                DATABASE -> rewriteDB();
                system("pause"); break;
            }
            case 4:
            {
                system("cls");
                std::cout << *DATABASE << std::endl;
                system("pause"); break;
            }
            case 5:
            {
                system("cls");
                std::string target;
                std::cout << Rus("Введите данные: ");
                char c;
                do
                {
                    c = getch();
                    if (c == Output::BACKSPACE)
                    {
                        putchar(' '); putchar(c);
                        if (target.size() > 0)
                        {
                            target.erase(target.size() - 1);
                        }
                    }
                    else
                    {
                        target += c;
                    }
                    DATABASE -> filltration(target);
                } while (c != Output::ENTER);
                break;
            }
            case 6:
            {
                DATABASE -> sort("ИД");
                uint32_t c;
                do
                {
                    system("cls");
                    std::cout << Rus("Перезаписать файл? Y/N");
                    c = getch();
                } while (c != 'Y' && c != 'N');
                if (c == 'Y')
                {
                    DATABASE -> rewriteDB();
                }
                break;
            }
            case 7:
                isBack = true; break;
            default:
                std::cout << Rus("Некорректное число. Попробуйте еще раз.") << std::endl; system("pause");
        }
    } while (!isBack);
}
