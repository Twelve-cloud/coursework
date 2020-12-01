#include "regi_window.h"
#include <QKeyEvent>
#include <QHBoxLayout>
#include <QGridLayout>

RegiWindow::RegiWindow(QWidget *parent) : QWidget(parent) {
    QWidget::setFixedSize(QSize(480, 360));
    setWindowTitle("Registration");

    m_generalLbl    = new QLabel("Registration");
    m_generalLbl    -> setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    m_loginLbl      = new QLabel("Login: ");
    m_passwordLbl   = new QLabel("Password: ");
    m_confirmLbl    = new QLabel("Confirm: ");
    m_loginLine     = new QLineEdit(this);
    m_passwordLine  = new QLineEdit(this);
    m_passwordLine  -> setEchoMode(QLineEdit::Password);
    m_confirmLine   = new QLineEdit(this);
    m_confirmLine   -> setEchoMode(QLineEdit::Password);
    m_registerBtn   = new QPushButton("&Register");
    m_registerBtn   -> setShortcut(Qt::Key_Return);
    m_clearBtn      = new QPushButton("&Clear");
    m_backBtn       = new QPushButton("&Back");

    QHBoxLayout* gen_label = new QHBoxLayout;
    gen_label -> addWidget(m_generalLbl,  Qt::Alignment(Qt::AlignHCenter | Qt::AlignVCenter));

    QGridLayout* main_layout = new QGridLayout;
    main_layout -> addItem(gen_label, 0, 1);
    main_layout -> addWidget(m_loginLbl, 1, 0);
    main_layout -> addWidget(m_passwordLbl, 2, 0);
    main_layout -> addWidget(m_confirmLbl, 3, 0);
    main_layout -> addWidget(m_loginLine, 1, 1);
    main_layout -> addWidget(m_passwordLine, 2, 1);
    main_layout -> addWidget(m_confirmLine, 3, 1);
    main_layout -> addWidget(m_registerBtn, 4, 1);
    main_layout -> addWidget(m_clearBtn, 5, 1);
    main_layout -> addWidget(m_backBtn, 6, 1);
    main_layout -> setHorizontalSpacing(2);
    main_layout -> setVerticalSpacing(2);
    main_layout -> setContentsMargins(30, 0, 40, 0);

    connect(m_registerBtn, SIGNAL(clicked()), this, SLOT(LoginLine_edited()));
    connect(m_registerBtn, SIGNAL(clicked()), this, SLOT(PasswordLine_edited()));
    connect(m_registerBtn, SIGNAL(clicked()), this, SLOT(ConfirmLine_edited()));
    connect(m_registerBtn, SIGNAL(clicked()), this, SLOT(Registration2_clicked()));
    connect(m_clearBtn, SIGNAL(clicked()), this, SLOT(Clear_clicked()));
    connect(m_backBtn, SIGNAL(clicked()), this, SLOT(Back_clicked()));
    setLayout(main_layout);
}

void RegiWindow::Registration2_clicked() {
    if (m_userLog.size() < 6 || m_userLog.size() > 15) {
        setError("Login must be between 6 and 30 characters");
    } else if (m_userPass.size() < 6 || m_userPass.size() > 15) {
        setError("Password must be between 6 and 30 characters");
    } else {
        emit registration2_clicked();
    }
}

void RegiWindow::Clear_clicked() {
    clearLines();
}

void RegiWindow::Back_clicked()
{
    emit back_to_auth_clicked();
}

void RegiWindow::LoginLine_edited() {
    m_userLog = m_loginLine -> text();
}

void RegiWindow::PasswordLine_edited() {
    m_userPass = m_passwordLine -> text();
}

void RegiWindow::ConfirmLine_edited() {
    m_confirmation = m_confirmLine -> text();
}

bool RegiWindow::checkPass() {
    return (m_userPass == m_confirmation);
}

void RegiWindow::clearLines() {
    setError("Registration");
    m_loginLine -> clear();
    m_passwordLine -> clear();
    m_confirmLine -> clear();
}

void RegiWindow::setError(const QString& error_str) {
    m_generalLbl -> setText(error_str);
    if (error_str != "Registration") {
        m_generalLbl -> setStyleSheet("color: red");
    }

    if (error_str == "Registration") {
        m_generalLbl -> setStyleSheet("color: rgb(200, 200, 200)");
    }
}

void RegiWindow::keyPressEvent(QKeyEvent *event)
{
    static uint32_t count = 1;
    if (event->key() == Qt::Key_Down && count < 3)
    {
        count++;
        if (count == 2)
            m_passwordLine -> setFocus();
        else if (count == 3)
            m_confirmLine -> setFocus();
    }
    else if (event->key() == Qt::Key_Up && count > 1)
    {
        count--;
        if (count == 1)
            m_loginLine -> setFocus();
        else if (count == 2)
            m_passwordLine -> setFocus();
    }
}
