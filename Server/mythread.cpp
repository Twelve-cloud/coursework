#include "mythread.h"
#include <iostream>
#include <cstdint>

void MyThread::run()
{
    bool isEnd;
    std::uint32_t choice;

    do {
        isEnd = false;
        std::cout << "1 - Account management"  << std::endl;
        std::cout << "2 - Turn off the server" << std::endl;
        std::cout << "Your choice: ";
        std::cin >> choice;

        switch(choice)
        {
            case 1:
                acc_menu(); break;
            case 2:
                std::exit(-1);
            default:
                std::cout << "Incorrect number. Try again." << std::endl;
        }

    } while(!isEnd);
}

void MyThread::acc_menu() {
    bool isBack;
    std::uint32_t choice;

    do {
        isBack = false;
        std::cout << "1 - Add account" << std::endl;
        std::cout << "2 - Delete account" << std::endl;
        std::cout << "3 - Print all accounts" << std::endl;
        std::cout << "4 - Find account" << std::endl;
        std::cout << "5 - Open file" << std::endl;
        std::cout << "6 - Back" << std::endl;
        std::cout << "Your choice: ";
        std::cin >> choice;

        switch(choice) {
            case 1:
            {
                Account* tmp = new Account; std::cin >> *tmp;
                m_tcpServer -> getDatabase() -> add(tmp);
                break;
            }
            case 2:
            {
                std::string login;
                std::cout << "Enter login: ";
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::getline(std::cin, login, '\n');
                if (m_tcpServer -> getDatabase() -> remove(login)) {
                    std::cout << "Account was deleted." << std::endl;
                    m_tcpServer -> rewriteBD();
                } else {
                    std::cout << "There is no account with this login." << std::endl;
                }
                break;
            }
            case 3:
            {
                std::cout << *m_tcpServer -> getDatabase() << std::endl;
                break;
            }
            case 4:
            {
                std::string login;
                std::cout << "Enter login: ";
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::getline(std::cin, login, '\n');
                AccountBase::Node* tmp = m_tcpServer -> getDatabase() -> find(login, 0);
                if (tmp != nullptr) {
                    std::cout << tmp;
                } else {
                    std::cout << "There is no account with this login." << std::endl;
                }
                break;
            }
            case 5:
                system(m_filename.c_str()); break;
            case 6:
                isBack = true; break;
            default:
                std::cout << "Incorrect number. Try again." << std::endl;

        }
    } while (!isBack);
}

