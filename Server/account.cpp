#include "account.h"
#include <iostream>
#include <sstream>
#include "streamtable.h"

// Реализация Account ниже

std::ostream& operator<<(std::ostream& out, const Account& object) {
    std::cout << object.m_login << std::endl;
    std::cout << object.m_password << std::endl;

    return out;
}
std::istream& operator>>(std::istream& in, Account& object) {
    std::cout << "Enter login: ";    std::cin >> object.m_login;
    std::cout << "Enter passwotd: "; std::cin >> object.m_password;

    return in;
}

// Реализация AccountBase ниже

AccountBase::AccountBase() {
    pbeg = pend = nullptr;
    m_length = 0;
}

AccountBase::~AccountBase() {
    while (pbeg != nullptr) {
        Node *tmp = pbeg;
        pbeg = pbeg -> next;
        delete tmp;
    }
}

void AccountBase::add(Account* object) {
    Node *new_account = new Node(object);
    if (pbeg == 0) {
        pbeg = pend = new_account;
    } else {
        new_account -> prev = pend;
        pend -> next = new_account;
        pend = new_account;
    }
    ++m_length;
}

bool AccountBase::find(std::string login) {
    Node *target = pbeg;
    while (target != nullptr) {
        if (target -> m_object -> getLogin() == login) {
            return true;
        }
       target = target -> next;
    }

    return false;
}

bool AccountBase::findAccount(std::string login, std::string password) {
    Node *target = pbeg;
    while (target != nullptr) {
        if (target -> m_object -> getLogin() == login &&
            target -> m_object -> getPassword() == password) {
            return true;
        }
       target = target -> next;
    }

    return false;
}

AccountBase::Node* AccountBase::find(std::string login, int) { // для поиска ячейки в удалении
    Node *target = pbeg;
    while (target != nullptr && target -> m_object -> getLogin() != login) {
       target = target -> next;
    }

    return target;
}

AccountBase::Node* AccountBase::find(uint32_t index) { // для поиска ячейки в удалении
    uint32_t counter = 0;
    Node *target = pbeg;
    while (target != nullptr && index != counter) {
       target = target -> next;
       counter++;
    }

    return target;
}


bool AccountBase::remove(std::string login) {
    if (Node *target = find(login, 0)) {

        if (target == pbeg) {
            pbeg = pbeg -> next;

            if (pbeg) {
                pbeg -> prev = nullptr;
            }

        } else if (target == pend) {
            pend = pend -> prev;
            pend -> next = nullptr;
        } else {
            target -> next -> prev = target -> prev;
            target -> prev -> next = target -> next;
        }
        --m_length;
        return true;
    }

    return false;
}

std::ostream& operator<<(std::ostream& out, const AccountBase::Node* object)
{

    std::cout << object -> m_object -> getLogin() << object -> m_object -> getPassword();

    return out;
}

std::ostream& operator<<(std::ostream& out, const AccountBase& object)
{
    StreamTable st(std::cout);
    st.AddCol(30);
    st.AddCol(30);


    st.Clear();
    st.SetCols(2, 30);

//    st.SetVisible(1, false);

    st.MakeBorderExt(true);
    st.SetDelimRow(true, '-');//st.SetDelimRow(false);//без символов-разделителей строк
    st.SetDelimCol(true, '|');//st.SetDelimCol(false);//без символов-разделителей строк
    st << "Login" << "Password";

    AccountBase::Node* tmp = object.pbeg;
    while (tmp != nullptr)
    {
        st << tmp -> m_object -> getLogin() << tmp -> m_object -> getPassword();
        tmp = tmp -> next;
    }

    return out;
}
