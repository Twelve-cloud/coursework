#include "mythread.h"
#include "streamtable.h"
#include <windows.h>
#include <conio.h>
#include <iostream>
#include <cstdint>

void getinitmenu(int count) {
    system("cls");
    StreamTable st(std::cout);

    st.AddCol(20);
    st.MakeBorderExt(true);
    st.SetDelimRow(true, '-');
    st.SetDelimCol(true, '|');
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    switch(count) {
        case 1: SetConsoleTextAttribute(hConsole, (WORD)((2 << 4) | 15)); st << "Account management"; SetConsoleTextAttribute(hConsole, (WORD)((0 << 4) | 2)); st << "Turn off the server"; break;
        case 2: st << "Account management"; SetConsoleTextAttribute(hConsole, (WORD)((2 << 4) | 15)); st << "Turn off the server"; SetConsoleTextAttribute(hConsole, (WORD)((0 << 4) | 2)); break;
    }
}

void getsecmenu(int count) {
    system("cls");
    StreamTable st(std::cout);

    st.AddCol(20);
    st.MakeBorderExt(true);
    st.SetDelimRow(true, '-');
    st.SetDelimCol(true, '|');
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    switch(count) {
        case 1:
        {
            SetConsoleTextAttribute(hConsole, (WORD)((2 << 4) | 15)); st << "Add account"; SetConsoleTextAttribute(hConsole, (WORD)((0 << 4) | 2));
            st << "Delete account";  st << "Print all accounts"; st << "Find account"; st << "Sort accounts"; st << "Open file"; st << "Back"; break;
        }
        case 2:
        {
            st << "Add account"; SetConsoleTextAttribute(hConsole, (WORD)((2 << 4) | 15)); st << "Delete account"; SetConsoleTextAttribute(hConsole, (WORD)((0 << 4) | 2));
            st << "Print all accounts"; st << "Find account"; st << "Sort accounts"; st << "Open file"; st << "Back"; break;
        }
        case 3:
        {
            st << "Add account"; st << "Delete account"; SetConsoleTextAttribute(hConsole, (WORD)((2 << 4) | 15)); st << "Print all accounts";
            SetConsoleTextAttribute(hConsole, (WORD)((0 << 4) | 2)); st << "Find account"; st << "Sort accounts"; st << "Open file"; st << "Back"; break;
        }
        case 4:
        {
            st << "Add account"; st << "Delete account"; st << "Print all accounts"; SetConsoleTextAttribute(hConsole, (WORD)((2 << 4) | 15));
            st << "Find account"; SetConsoleTextAttribute(hConsole, (WORD)((0 << 4) | 2)); st << "Sort accounts"; st << "Open file"; st << "Back"; break;
        }
        case 5:
        {
            st << "Add account"; st << "Delete account"; st << "Print all accounts"; st << "Find account"; SetConsoleTextAttribute(hConsole, (WORD)((2 << 4) | 15));
            st << "Sort accounts"; SetConsoleTextAttribute(hConsole, (WORD)((0 << 4) | 2)); st << "Open file"; st << "Back"; break;
        }
        case 6:
        {
            st << "Add account"; st << "Delete account"; st << "Print all accounts"; st << "Find account"; st << "Sort accounts";
            SetConsoleTextAttribute(hConsole, (WORD)((2 << 4) | 15)); st << "Open file"; SetConsoleTextAttribute(hConsole, (WORD)((0 << 4) | 2)); st << "Back"; break;
        }
        case 7:
        {
            st << "Add account"; st << "Delete account"; st << "Print all accounts"; st << "Find account"; st << "Sort accounts"; st << "Open file";
            SetConsoleTextAttribute(hConsole, (WORD)((2 << 4) | 15)); st << "Back"; SetConsoleTextAttribute(hConsole, (WORD)((0 << 4) | 2)); break;
        }
    }


}

void MyThread::run()
{
    int count = 1, engine;
    bool isEnd;

    do {
        getinitmenu(count);
        isEnd = false;
        do {
            engine = getch();
            if (engine == 72 && count > 1) {
                count--;
            }
            if (engine == 80 && count < 7) {
                count++;
            }
            getinitmenu(count);
        } while (engine != 13);

        switch (count)
        {
            case 1:
                acc_menu(); break;
            case 2:
                std::exit(-1);
            default:
                std::cout << "Incorrect number. Try again." << std::endl; system("pause");
        }

    } while(!isEnd);
}

void MyThread::acc_menu() {
    int count = 1, engine;
    bool isBack;


    do {
        getsecmenu(count);
        isBack = false;
        do {
            engine = getch();
            if (engine == 72 && count > 1) {
                count--;
            }
            if (engine == 80 && count < 7) {
                count++;
            }
            getsecmenu(count);
        } while (engine != 13);

        switch(count) {
            case 1:
            {
                Account* tmp = new Account; std::cin >> *tmp;
                m_tcpServer -> getDatabase() -> add(tmp);
                m_tcpServer -> rewriteBD();
                std::cout << "SUCCESSFUL." << std::endl;
                system("pause");
                break;
            }
            case 2:
            {
                system("cls");
                std::string login;
                std::cout << "Enter login: ";
                std::getline(std::cin, login, '\n');
                if (m_tcpServer -> getDatabase() -> remove(login)) {
                    std::cout << "Account was deleted." << std::endl;
                    m_tcpServer -> rewriteBD();
                } else {
                    std::cout << "There is no account with this login." << std::endl;
                }
                system("pause");
                break;
            }
            case 3:
            {
                system("cls");
                std::cout << *m_tcpServer -> getDatabase() << std::endl;
                system("pause");
                break;
            }
            case 4:
            {
                system("cls");
                std::string login;
                std::cout << "Enter login: ";
                char c;
                do {
                    c = getche();
                    if (c == 8) {
                        putchar(' ');
                        putchar(c);
                        if (login.size() > 0)
                            login.erase(login.size() - 1);
                    } else {
                        login += c;
                    }
                    m_tcpServer -> getDatabase() -> filltration(login);
                } while (c != 13);
                break;
            }
            case 5:
            {
                //m_tcpServer -> getDatabase() -> sort();
                int c;
                do {
                    std::cout << "Rewrite file? Y/N";
                    c = getch();
                } while (c != 'Y' || c != 'N');
                if (c == 'Y') {
                    m_tcpServer -> rewriteBD();
                }
            }
            case 6:
                system(m_filename.c_str()); break;
            case 7:
                isBack = true; break;
            default:
                std::cout << "Incorrect number. Try again." << std::endl; system("pause");

        }
    } while (!isBack);
}
