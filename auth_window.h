#ifndef USER_AUTH_H
#define USER_AUTH_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QString>
#include <QLineEdit>

class AuthWindow : public QWidget // окно аутентификации
{
    Q_OBJECT
public:
    explicit AuthWindow(QWidget *parent = nullptr);

    QString& getLogin() { return m_userLog; };
    QString& getPassword() { return m_userPass; }
    void setError(const QString& error_str);
    bool isEmptyLine();
    void clearLines();

signals:
    void registration_clicked();
    void login_clicked();

private slots:
    void Login_clicked();
    void Registration_clicked();
    void LoginLine_edited();
    void PasswordLine_edited();

protected:
   virtual void keyPressEvent(QKeyEvent *event);

private:
    QPushButton *m_registerBtn, *m_loginBtn;
    QLabel *m_generalLbl, *m_loginLbl, *m_passwordLbl;
    QLineEdit *m_loginLine, *m_passwordLine;
    QString m_userLog, m_userPass;
};

#endif // USER_AUTH_H
