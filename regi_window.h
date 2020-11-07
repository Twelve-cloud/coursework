#ifndef USER_REG_H
#define USER_REG_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QString>
#include <QLineEdit>

class RegiWindow : public QWidget {
    Q_OBJECT
public:
    explicit RegiWindow(QWidget *parent = nullptr);
    QString& getLogin() { return m_userLog; };
    QString& getPassword() { return m_userPass; };
    bool isCorrectLogin(const QString& login);
    bool isCorrectPassword(const QString& login);
    bool checkPass();
    void setError(const QString& error_str);
    void clearLines();

signals:
    void registration2_clicked();

private slots:
    void Registration2_clicked();
    void Clear_clicked();
    void LoginLine_edited();
    void PasswordLine_edited();
    void ConfirmLine_edited();

private:
    QPushButton *m_registerBtn, *m_clearBtn;
    QLabel *m_generalLbl, *m_loginLbl, *m_passwordLbl, *m_confirmLbl;
    QLineEdit *m_loginLine, *m_passwordLine, *m_confirmLine;
    QString m_userLog, m_userPass, m_confirmation;
};

#endif // USER_REG_H
