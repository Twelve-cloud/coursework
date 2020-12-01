#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <string>

class Account
{
public:
    Account() {}
    Account(const std::string& login, const std::string& password): m_login(login), m_password(password) {}
    /* 2 конструктора: 1-ый с по умолчанию, 2-ой с двумя параметрами */

    std::string getLogin   () const { return m_login; }
    std::string getPassword() const { return m_password; }
    /* 2 геттера */

    void setLogin   (const std::string& login) { m_login = login; }
    void setPassword(const std::string& password) {m_password = password; }
    /* 2 сеттера */

    friend std::ostream& operator<<(std::ostream& out, const Account& object);
    friend std::istream& operator>>(std::istream& in, Account& object);
    /* 2 дружественные функции для перегрузки оператора вставки и оператора извлечения */

private:
    std::string m_login;
    std::string m_password;
};

#endif // ACCOUNT_H
