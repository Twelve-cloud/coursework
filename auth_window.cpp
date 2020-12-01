#include "auth_window.h"
#include <QKeyEvent>
#include <QHBoxLayout>
#include <QGridLayout>

AuthWindow::AuthWindow(QWidget *parent) : QWidget(parent) {
    QWidget::setFixedSize(QSize(480, 360));
    setWindowTitle("Authentification");

    m_generalLbl    = new QLabel("Authentification");
    m_generalLbl -> setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    m_loginLbl      = new QLabel("Login: ");
    m_passwordLbl   = new QLabel("Password: ");
    m_loginLine     = new QLineEdit(this);
    m_passwordLine  = new QLineEdit(this);
    m_passwordLine -> setEchoMode(QLineEdit::Password);
    m_loginBtn      = new QPushButton("&Log In");
    m_loginBtn -> setShortcut(Qt::Key_Return);
    m_registerBtn   = new QPushButton("&Register");

    QHBoxLayout *gen_label = new QHBoxLayout;
    gen_label -> addWidget(m_generalLbl,  Qt::Alignment(Qt::AlignHCenter | Qt::AlignVCenter));

    QGridLayout* main_layout = new QGridLayout;
    main_layout -> addItem(gen_label, 0, 1);
    main_layout -> addWidget(m_loginLbl, 1, 0, Qt::AlignRight);
    main_layout -> addWidget(m_passwordLbl, 2, 0, Qt::AlignRight);
    main_layout -> addWidget(m_loginLine, 1, 1);
    main_layout -> addWidget(m_passwordLine, 2, 1);
    main_layout -> addWidget(m_loginBtn, 3, 1);
    main_layout -> addWidget(m_registerBtn, 4, 1);
    main_layout -> setHorizontalSpacing(2);
    main_layout -> setVerticalSpacing(2);
    main_layout -> setContentsMargins(30, 0, 40, 0);

    connect(m_loginBtn, SIGNAL(clicked()), this, SLOT(LoginLine_edited()));
    connect(m_loginBtn, SIGNAL(clicked()), this, SLOT(PasswordLine_edited()));
    connect(m_loginBtn, SIGNAL(clicked()), this, SLOT(Login_clicked()));
    connect(m_registerBtn, SIGNAL(clicked()), this, SLOT(Registration_clicked()));
    setLayout(main_layout);
}

void AuthWindow::Registration_clicked() {
    emit registration_clicked();
}

void AuthWindow::Login_clicked() {
    emit login_clicked();
}

void AuthWindow::LoginLine_edited() {
    m_userLog = m_loginLine -> text();
}

void AuthWindow::PasswordLine_edited() {
    m_userPass = m_passwordLine -> text();
}

void AuthWindow::clearLines() {
    m_loginLine -> clear();
    m_passwordLine -> clear();
}

bool AuthWindow::isEmptyLine()
{
    if (m_userLog.isEmpty() || m_userPass.isEmpty())
        return true;
    return false;
}

void AuthWindow::setError(const QString& error_str)
{
    m_generalLbl -> setText(error_str);
    if (error_str != "Authentification") {
        m_generalLbl -> setStyleSheet("color: red");
    }

    if (error_str == "Authentification") {
        m_generalLbl -> setStyleSheet("color: rgb(200, 200, 200)");
    }
}

void AuthWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Down)
    {
        m_passwordLine -> setFocus();
    }
    else if (event->key() == Qt::Key_Up)
    {
        m_loginLine -> setFocus();
    }
}
