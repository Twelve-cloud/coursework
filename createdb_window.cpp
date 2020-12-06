#include "createdb_window.h"
#include <QWidget>
#include <QPushButton>
#include <QKeyEvent>
#include <QLineEdit>
#include <QLabel>
#include <QString>
#include <QHBoxLayout>
#include <QGridLayout>

CreateDB_Window::CreateDB_Window(QWidget *parent) : QWidget(parent)
{
    QWidget::setFixedSize(QSize(480, 360));
    setWindowTitle("Настройки базы данных");

    m_dbSettingsLbl    = new QLabel("Настройки базы данных");
    m_dbSettingsLbl    -> setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);

    m_dbNameLbl        = new QLabel("Имя базы данных: ");
    m_dbNameLine       = new QLineEdit(this);

    m_dbPasswordLbl    = new QLabel("Пароль: ");
    m_dbPassLine       = new QLineEdit(this);
    m_dbPassLine       -> setEchoMode(QLineEdit::Password);

    m_okBtn            = new QPushButton("&Создать | Подключиться");

    QHBoxLayout *gen_label = new QHBoxLayout;
    gen_label -> addWidget(m_dbSettingsLbl,  Qt::Alignment(Qt::AlignHCenter | Qt::AlignVCenter));

    QGridLayout* main_layout = new QGridLayout;
    main_layout -> addItem(gen_label, 0, 1);
    main_layout -> addWidget(m_dbNameLbl, 1, 0, Qt::AlignRight);
    main_layout -> addWidget(m_dbPasswordLbl, 2, 0, Qt::AlignRight);
    main_layout -> addWidget(m_dbNameLine, 1, 1);
    main_layout -> addWidget(m_dbPassLine, 2, 1);
    main_layout -> addWidget(m_okBtn, 3, 1);
    main_layout -> setHorizontalSpacing(2);
    main_layout -> setVerticalSpacing(2);
    main_layout -> setContentsMargins(30, 0, 40, 0);

    connect(m_okBtn, SIGNAL(clicked()), this, SLOT(dbNameLine_edited()));
    connect(m_okBtn, SIGNAL(clicked()), this, SLOT(dbPassLine_edited()));
    connect(m_okBtn, SIGNAL(clicked()), this, SLOT(Ok_clicked()));
    setLayout(main_layout);
}

void CreateDB_Window::Ok_clicked()
{
    bool isWrong = false;

    if (m_dbName.isEmpty() || m_dbPass.isEmpty())
    {
        setError("Заполните все поля");
        return;
    }

    if(m_okBtn ->text() == "Создать")
    {
        for (const auto& ch : m_dbName)
        {
            if (!ch.isDigit() && !ch.isLetter())
            {
                setError("Имя базы данных должен содержать только бувы или цифры");
                isWrong = true;
            }
        }

        for (const auto& ch : m_dbPass)
        {
            if (!ch.isDigit() && !ch.isLetter())
            {
                setError("Пароль должен содержать только буквы или цифры");
                isWrong = true;
            }
        }

        if (m_dbName.size() < 6 || m_dbName.size() > 20)
        {
            setError("Имя базы данных должно быть от 6 до 20 символов");
        }
        else if (m_dbPass.size() < 6 || m_dbPass.size() > 20)
        {
            setError("Пароль должен быть от 6 до 20 символов");
        }
        else if (!isWrong)
        {
            emit send_db_settings_clicked();
        }
    }
    else
    {
        emit send_db_settings_clicked();
    }
}

void CreateDB_Window::dbNameLine_edited()
{
    m_dbName = m_dbNameLine -> text();
}

void CreateDB_Window::dbPassLine_edited()
{
    m_dbPass = m_dbPassLine -> text();
}

void CreateDB_Window::clearLines()
{
    m_dbNameLine -> clear();
    m_dbPassLine -> clear();
}

void CreateDB_Window::changeOkName(QString btnName)
{
    m_okBtn -> setText(btnName);
    m_okBtn -> setShortcut(Qt::Key_Return);
}

bool CreateDB_Window::checkAction()
{
    if(m_okBtn -> text() == "Создать")
        return true;
    return false;
}

void CreateDB_Window::setError(const QString& error_str)
{
    m_dbSettingsLbl -> setText(error_str);
    if (error_str != "Настройки базы данных") {
        m_dbSettingsLbl -> setStyleSheet("color: red");
    }

    if (error_str == "Настройки базы данных") {
        m_dbSettingsLbl -> setStyleSheet("color: rgb(200, 200, 200)");
    }
}

void CreateDB_Window::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Down)
    {
        m_dbPassLine -> setFocus();
    }
    else if (event->key() == Qt::Key_Up)
    {
        m_dbNameLine -> setFocus();
    }
}
