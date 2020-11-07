#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <string>
#include <cstdint>
#include <fstream>
#include <cassert>

class Account {
public:
    Account() {}
    Account(std::string login, std::string password)
        : m_login(login), m_password(password) {}

    std::string getLogin   () { return m_login; }
    std::string getPassword() { return m_password; }
    friend std::ostream& operator<<(std::ostream& out, const Account& object);
    friend std::istream& operator>>(std::istream& in, Account& object);

private:
    std::string m_login;
    std::string m_password;
};

class AccountBase {
public:
    class Node {
    public:
        Account *m_object;
        Node *next, *prev;
        Node(Account* object) : m_object(object) {
            next = prev = nullptr;
        }
        friend std::ostream& operator<<(std::ostream& out, const Node* object);
    };

    AccountBase();
    ~AccountBase();

    void add(Account* object);
    Node* find(std::string login, int);
    Node* find(uint32_t index);
    bool find(std::string login);
    bool findAccount(std::string login, std::string password);
    bool remove(std::string login);
    std::uint32_t length() { return m_length; }
    friend std::ostream& operator<<(std::ostream& out, const AccountBase& object);
    Node* operator[](uint32_t index) { assert(index < m_length && "Invalid index"); return find(index);}

private:
    Node *pbeg, *pend;
    std::uint32_t m_length;
};

#endif // ACCOUNT_H
