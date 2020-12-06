#include "mainwindow.h"
#include "constants.h"
#include <QMenu>
#include <QApplication>
#include <QMenuBar>
#include <QDebug>
#include <QAction>
#include <QMessageBox>
#include <QShortcut>
#include <QMdiArea>
#include <QMdiSubWindow>
#include <QScreen>
#include <QTableWidget>
#include <QHeaderView>

std::string getWord(std::string str, std::string& field);

MainWindow::MainWindow(const QString& hostname, int port, QWidget *parent) : QMainWindow(parent), m_nextBlockSize(0)
{
    m_socket = new QTcpSocket(this);
    m_socket -> connectToHost(hostname, port);
    connect(m_socket, SIGNAL(readyRead()), SLOT(slotReadyRead()));

    connect(&m_auth, SIGNAL(registration_clicked()), this, SLOT(registerWindowShow()));
    connect(&m_auth, SIGNAL(login_clicked()), this, SLOT(Authorization()));
    connect(&m_regi, SIGNAL(registration2_clicked()), this, SLOT(Registration()));
    connect(&m_regi, SIGNAL(back_to_auth_clicked()), this, SLOT(backToAuth()));
    connect(&m_dbWindow, SIGNAL(send_db_settings_clicked()), this, SLOT(sendDbData()));

    setWindowTitle("Главное окно");
    setWindowState(Qt::WindowMaximized);

    m_menu[FILE]    = menuBar() -> addMenu("Файл");
    m_menu[EDIT]    = menuBar() -> addMenu("Изменить");
    m_menu[WINDOW]  = menuBar() -> addMenu("Окно");
    m_menu[HELP]    = menuBar() -> addMenu("Помощь");

    QAction *quit = new QAction("&Выйти из программы");
    quit -> setShortcut(tr("Ctrl+Q"));

    QAction *back = new QAction("&Выйти из системы");
    back -> setShortcut(tr("Ctrl+B"));

    QAction *createDB = new QAction("Создать БД");
    createDB -> setShortcut(tr("Ctrl+N"));

    QAction *connectDB = new QAction("Подключиться к БД");
    connectDB -> setShortcut(tr("Ctrl+L"));

    full_screen = new QAction("Полноэкранный режим");
    full_screen -> setShortcut(tr("Ctrl+Shift+F11"));
    full_screen -> setCheckable(true);

    QAction* about = new QAction("О разработчике");
    about -> setShortcut(tr("F4"));

    m_menu[FILE]    -> addAction(createDB);
    m_menu[FILE]    -> addAction(connectDB);
    m_menu[FILE]    -> addAction(back);
    m_menu[FILE]    -> addAction(quit);
    m_menu[WINDOW]  -> addAction(full_screen);
    m_menu[HELP]    -> addAction(about);


////////////////Работа с дочерними окнами//////////////////////////////
    leftMenu = new QMdiArea(this);

    m_types = new TypeWindow(leftMenu);
    subWindowTypes = leftMenu -> addSubWindow(m_types);
    subWindowTypes -> setWindowFlags(Qt::Window | Qt::WindowTitleHint | Qt::CustomizeWindowHint | Qt::FramelessWindowHint);
    subWindowTypes -> move(0, 0);
    subWindowTypes -> setFixedSize(260, QGuiApplication::screens().at(0)->geometry().height() - menuBar()->geometry().height() + 4);
    subWindowTypes -> hide();

    m_actions = new ActionWindow(leftMenu);
    subWindowActions = leftMenu -> addSubWindow(m_actions);
    subWindowActions -> setWindowFlags(Qt::Window | Qt::WindowTitleHint | Qt::CustomizeWindowHint | Qt::FramelessWindowHint);
    subWindowActions -> move(260, 0);
    subWindowActions -> setFixedSize(260, QGuiApplication::screens().at(0)->geometry().height() - menuBar()->geometry().height() + 4);
    subWindowActions -> hide();

    m_table = new QTableWidget;

    subWindowTable = leftMenu -> addSubWindow(m_table);
    subWindowTable -> setWindowFlags(Qt::Window | Qt::WindowTitleHint | Qt::CustomizeWindowHint | Qt::FramelessWindowHint);
    subWindowTable -> move(520, 0);
    subWindowTable -> setFixedSize(QGuiApplication::screens().at(0)->geometry().width() - 520, QGuiApplication::screens().at(0)->geometry().height()- menuBar() ->geometry().height() + 4);
    subWindowTable -> hide();

    m_deleteWidget = new DeleteWidget;

    subWindowDelete = leftMenu -> addSubWindow(m_deleteWidget);
    subWindowDelete -> setWindowFlags(Qt::Window | Qt::WindowTitleHint | Qt::CustomizeWindowHint | Qt::FramelessWindowHint);
    subWindowDelete -> move(QGuiApplication::screens().at(0)->geometry().width() / 2 - m_deleteWidget -> size().width() / 2,
                            QGuiApplication::screens().at(0)->geometry().height() / 2 - m_deleteWidget -> size().height() / 2);
    subWindowDelete -> hide();

    m_findWidget = new FindWidget;

    subWindowFind = leftMenu -> addSubWindow(m_findWidget);
    subWindowFind -> setWindowFlags(Qt::Window | Qt::WindowTitleHint | Qt::CustomizeWindowHint | Qt::FramelessWindowHint);
    subWindowFind -> move(QGuiApplication::screens().at(0)->geometry().width() / 2 - m_findWidget -> size().width() / 2,
                            QGuiApplication::screens().at(0)->geometry().height() / 2 - m_findWidget -> size().height() / 2);
    subWindowFind -> hide();

    m_sortWidget = new SortWidget;

    subWindowSort = leftMenu -> addSubWindow(m_sortWidget);
    subWindowSort -> setWindowFlags(Qt::Window | Qt::WindowTitleHint | Qt::CustomizeWindowHint | Qt::FramelessWindowHint);
    subWindowSort -> move(QGuiApplication::screens().at(0)->geometry().width() / 2 - m_sortWidget -> size().width() / 2,
                            QGuiApplication::screens().at(0)->geometry().height() / 2 - m_sortWidget -> size().height() / 2);
    subWindowSort -> hide();

    setCentralWidget(leftMenu);

////////////////!Работа с дочерними окнами//////////////////////////////

    connect(m_table, SIGNAL(cellChanged(int, int)), this, SLOT(slot_cellChanged(int, int)));
    connect(m_sortWidget, SIGNAL(sort_type_clicked()), this, SLOT(slot_sortTypeClicked()));
    connect(m_sortWidget, SIGNAL(sort_cancel_clicked()), this, SLOT(slot_sortCancelClicked()));
    connect(m_findWidget, SIGNAL(find_ok_clicked()), this, SLOT(slot_findOkClicked()));
    connect(m_findWidget, SIGNAL(find_cancel_clicked()), this, SLOT(slot_findCancelClicked()));
    connect(m_deleteWidget, SIGNAL(delete_ok_clicked()), this, SLOT(slot_deleteOkClicked()));
    connect(m_deleteWidget, SIGNAL(delete_cancel_clicked()), this, SLOT(slot_deleteCancelClicked()));
    connect(m_actions, SIGNAL(add_record_clicked()), this, SLOT(slot_addRecordClicked()));
    connect(m_actions, SIGNAL(delete_record_clicked()), this, SLOT(slot_deleteRecordClicked()));
    connect(m_actions, SIGNAL(change_record_clicked()), this, SLOT(slot_changeRecordClicked()));
    connect(m_actions, SIGNAL(find_record_clicked()), this, SLOT(slot_findRecordClicked()));
    connect(m_actions, SIGNAL(sort_records_clicked()), this, SLOT(slot_sortRecordsClicked()));
    connect(m_types, SIGNAL(menu_actions_call()), this, SLOT(slot_menu_actions_call()));
    connect(createDB, SIGNAL(triggered()), this, SLOT(createDB()));
    connect(connectDB, SIGNAL(triggered()), this, SLOT(connectDB()));
    connect(back, SIGNAL(triggered()), this, SLOT(exit()));
    connect(quit, SIGNAL(triggered()), qApp, SLOT(quit()));
    connect(full_screen, SIGNAL(triggered()), this, SLOT(slotFullScreen()));
}

void MainWindow::slotFullScreen()
{
    if (full_screen -> isChecked())
    {
        QMainWindow::showFullScreen();
    }
    else
    {
        setWindowState(Qt::WindowMaximized);
    }
}

void MainWindow::display()
{
    m_auth.show();
}

void MainWindow::registerWindowShow()
{
    m_regi.clearLines();
    m_regi.setError("Регистрация");
    m_auth.hide();
    m_regi.show();
}

void MainWindow::Authorization()
{
    if (!m_auth.isEmptyLine())
    {
        SendToServer(SendingCodes::AUTHENTIFICATION, m_auth.getLogin() + " " + m_auth.getPassword());
    }
    else
    {
        m_auth.setError("Заполните все поля");
    }
}

void MainWindow::Registration()
{
    if (m_regi.checkPass())
    {
        SendToServer(SendingCodes::REGISTRATION, m_regi.getLogin() + " " + m_regi.getPassword());
    }
    else
    {
        m_regi.setError("Неверный пароль");
    }
}

void MainWindow::backToAuth()
{
    m_regi.hide();
    m_auth.clearLines();
    m_auth.show();
}

void MainWindow::exit()
{
    this -> hide();
    m_auth.show();
    m_auth.clearLines();
}

void MainWindow::sendDbData()
{
    if (m_dbWindow.checkAction())
    {
        SendToServer(SendingCodes::CREATE_DB, m_dbWindow.getName() + " " + m_dbWindow.getPass());
    }
    else
    {
        SendToServer(SendingCodes::CONNECT_DB, m_dbWindow.getName() + " " + m_dbWindow.getPass());
    }
}

void MainWindow::createDB()
{
    m_dbWindow.setError("Настройки базы данных");
    m_dbWindow.changeOkName("Создать");
    m_dbWindow.clearLines();
    m_dbWindow.show();
}

void MainWindow::connectDB()
{
    m_dbWindow.setError("Настройки базы данных");
    m_dbWindow.changeOkName("Подключиться");
    m_dbWindow.clearLines();
    m_dbWindow.show();
}

void MainWindow::slot_menu_actions_call()
{
    m_table ->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    subWindowTable -> show();
    subWindowActions -> show();
    m_table -> setRowCount(0);
    getColsTable(m_types -> getType());
    getHeaders(m_types -> getType());
    rowChanged.clear();
    SendToServer(SendingCodes::GET_RECORDS, m_types -> getType() + " " + m_dbWindow.getName());
}

void MainWindow::slot_sortRecordsClicked()
{
    subWindowSort -> hide();
    subWindowSort -> show();
}

void MainWindow::slot_sortTypeClicked()
{
    SendToServer(SendingCodes::SORT_RECORDS, m_sortWidget -> getType() + " " + m_dbWindow.getName());
}

void MainWindow::slot_sortCancelClicked()
{
    subWindowSort -> hide();
}

void MainWindow::slot_deleteRecordClicked()
{
    subWindowDelete -> hide();
    subWindowDelete -> show();
}

void MainWindow::slot_deleteOkClicked()
{
    if (m_deleteWidget -> isEmptyLine())
    {
        m_deleteWidget -> setError("Заполните все поля");
    }
    else
    {
        m_deleteWidget -> clearLines();
        subWindowDelete -> hide();
        SendToServer(SendingCodes::DELETE_RECORD, m_dbWindow.getName() + " " + m_types -> getType() + " " + m_deleteWidget -> getID());
    }
}

void MainWindow::slot_deleteCancelClicked()
{
    m_deleteWidget -> clearLines();
    subWindowDelete -> hide();
}


void MainWindow::slot_findOkClicked()
{
    if (m_findWidget -> isEmptyLine())
    {
        m_findWidget -> setError("Заполните все поля");
    }
    else
    {
        m_findWidget -> clearLines();
        subWindowFind -> hide();
        m_table -> setRowCount(0);
        SendToServer(SendingCodes::FIND_RECORD, m_dbWindow.getName() + " " + m_types -> getType() + " " + m_findWidget -> getID());
    }
}

void MainWindow::slot_findCancelClicked()
{
    m_findWidget -> clearLines();
    subWindowFind -> hide();
}

void MainWindow::slot_findRecordClicked()
{
    subWindowFind -> hide();
    subWindowFind -> show();
}

void MainWindow::slot_addRecordClicked()
{
    SendToServer(SendingCodes::ADD_RECORD, m_dbWindow.getName() + " " + m_types -> getType());
    m_table -> setRowCount(0);
    SendToServer(SendingCodes::GET_RECORDS, m_types -> getType() + " " + m_dbWindow.getName());
}

void MainWindow::slot_changeRecordClicked()
{
    QString replace_spaces;
    for (uint32_t i = 0; i < m_table -> rowCount(); i++)
    {
        QString stringToSend = m_dbWindow.getName() + " " + m_types -> getType() + " ";
        if (rowChanged[i] == true)
        {
            stringToSend += m_table -> item(i, 0) -> text() + " ";
            for (int j = 5; j < m_table -> columnCount(); j++)
            {
                replace_spaces = m_table -> item(i,j) -> text();
                replace_spaces.replace(" ", "|");
                stringToSend += replace_spaces + " ";
            }
            SendToServer(SendingCodes::CHANGE_RECORD, stringToSend);
        }
        stringToSend.clear();
    }
    rowChanged.clear();
    m_table -> setRowCount(0);
    SendToServer(SendingCodes::GET_RECORDS, m_types -> getType() + " " + m_dbWindow.getName());
}

void MainWindow::slot_cellChanged(int row, int col)
{
    if (col > 4)
        rowChanged[row] = true;
}

void MainWindow::getHeaders(QString type)
{
    char* Tech[] = {"ИД", "Тип", "Дата", "Время", "День", "Сир. номер", "Производитель", "Дата поступл.", "Модель", "Артикул", "Страна изг.", "Цена"};
    char* Computer[] = {"Процессор", "Ядер", "Тип ОП", "Размер ОП", "Разр. экрана", "Диагон. экрана"};
    char* Mobile[] = {"ОС", "Разр. экрана", "Диагон. экрана", "Процессор", "Ядер", "Размер ОП", "Сим. карт"};
    char* TV[] = {"Тип экрана", "Разр. экрана", "Диагон. экрана", "Процессор", "Ядер", "3Д режим"};
    char* Toaster[] = {"Кол. тостов", "Мощность", "Разморозка", "Нагрев"};
    char* CoffeeMaker[] = {"Мощность", "Давление", "Каппучино"};
    char* ElKettle[] = {"Мощность", "Объем", "Таймер"};
    char* Fridge[] = {"Объем", "Кол. полок", "Ур. шума", "Мультизона"};
    char* Conditioner[] = {"Режим работы", "Охл. мощность", "Нагрев. мощность", "Уд. управление"};
    char* Microwave[] = {"Мощность", "Ур. мощности", "Объем", "Быстр. старт", "Таймер"};
    int count = 0, childCount = 0;
    for (; count < 12; count++)
    {
        m_table -> setHorizontalHeaderItem(count, new QTableWidgetItem(Tech[count]));
    }

    if (type == "Компьютер")
    {
        for (; count < 18; count++)
        {
            m_table -> setHorizontalHeaderItem(count, new QTableWidgetItem(Computer[childCount++]));
        }
    }
    if (type == "Моб.Телефон")
    {
        for (; count < 19; count++)
        {
            m_table -> setHorizontalHeaderItem(count, new QTableWidgetItem(Mobile[childCount++]));
        }
    }
    if (type == "Телевизор")
    {
        for (; count < 18; count++)
        {
            m_table -> setHorizontalHeaderItem(count, new QTableWidgetItem(TV[childCount++]));
        }
    }
    if (type == "Тостер")
    {
        for (; count < 16; count++)
        {
            m_table -> setHorizontalHeaderItem(count, new QTableWidgetItem(Toaster[childCount++]));
        }
    }
    if (type == "Кофемашина")
    {
        for (; count < 15; count++)
        {
            m_table -> setHorizontalHeaderItem(count, new QTableWidgetItem(CoffeeMaker[childCount++]));
        }
    }
    if (type == "Эл.Чайник")
    {
        for (; count < 15; count++)
        {
            m_table -> setHorizontalHeaderItem(count, new QTableWidgetItem(ElKettle[childCount++]));
        }
    }
    if (type == "Холодильник")
    {
        for (; count < 16; count++)
        {
            m_table -> setHorizontalHeaderItem(count, new QTableWidgetItem(Fridge[childCount++]));
        }
    }
    if (type == "Кондиционер")
    {
        for (; count < 16; count++)
        {
            m_table -> setHorizontalHeaderItem(count, new QTableWidgetItem(Conditioner[childCount++]));
        }
    }
    if (type == "Микроволновка")
    {
        for (; count < 17; count++)
        {
            m_table -> setHorizontalHeaderItem(count, new QTableWidgetItem(Microwave[childCount++]));
        }
    }
}

void MainWindow::getColsTable(QString type)
{

    if (type == "Компьютер")
    {
        m_table -> setColumnCount(18);
    }
    if (type == "Моб.Телефон")
    {
        m_table -> setColumnCount(19);
    }
    if (type == "Телевизор")
    {
        m_table -> setColumnCount(18);
    }
    if (type == "Тостер")
    {
        m_table -> setColumnCount(16);
    }
    if (type == "Кофемашина")
    {
        m_table -> setColumnCount(15);
    }
    if (type == "Эл.Чайник")
    {
        m_table -> setColumnCount(15);
    }
    if (type == "Холодильник")
    {
        m_table -> setColumnCount(16);
    }
    if (type == "Кондиционер")
    {
        m_table -> setColumnCount(16);
    }
    if (type == "Микроволновка")
    {
        m_table -> setColumnCount(17);
    }
}

void MainWindow::handleResult(uint32_t compRecordsCount, qint16 result, QString string)
{
    rowChanged.push_back(true);
    switch(result)
    {
        case SendingCodes::FAIL_AUTHENTIFICATION:
        {
            m_auth.setError("Неверный логин или пароль");
            break;
        }
        case SendingCodes::FAIL_REGISTRATION:
        {
            m_regi.setError("Данный логин уже существует");
            break;
        }
        case SendingCodes::SUCCESS_REGISTRATION:
        {
            m_auth.setError("Аутентификация");
            m_auth.clearLines();
            QMessageBox::information(nullptr, "Информация", "Регистрация успешно завершена", QMessageBox::Ok);
            m_regi.hide();
            m_auth.show();
            break;
        }
        case SendingCodes::SUCCESS_AUTHENTIFICATION:
        {
            m_auth.hide();
            this -> show();
            break;
        }
        case SendingCodes::DATABASE_CREATION_FAIL:
        {
            m_dbWindow.setError("База данных с таким имене уже существует");
            break;
        }
        case SendingCodes::DATABASE_CONNECTION_FAIL:
        {
            m_dbWindow.setError("Неверный логин или пароль");
            break;
        }
        case SendingCodes::DATABASE_CREATION_SUCCESS:
        {
            m_dbWindow.clearLines();
            m_dbWindow.hide();
            QMessageBox::information(nullptr, "Информация", "База данных создана", QMessageBox::Ok);
            break;
        }
        case SendingCodes::DATABASE_CONNECTION_SUCCESS:
        {
            m_dbWindow.clearLines();
            m_dbWindow.hide();
            subWindowTypes -> show();
            break;
        }
        case SendingCodes::ADD_RECORD_SUCCESS:
        {
            m_table -> setRowCount(0);
            SendToServer(SendingCodes::GET_RECORDS, m_types -> getType() + " " + m_dbWindow.getName());
            break;
        }
        case SendingCodes::ADD_RECORD_FAIL:
        {
            QMessageBox::information(nullptr, "Ошибка", "Добавить запись не удалось", QMessageBox::Ok);
            break;
        }
        case SendingCodes::CHANGE_RECORD_FAIL:
        {
            QMessageBox::information(nullptr, "Ошибка", "Изменить запись не удалось", QMessageBox::Ok);
            SendToServer(SendingCodes::GET_RECORDS, m_types -> getType() + " " + m_dbWindow.getName());
            break;
        }
        case SendingCodes::DELETE_RECORD_SUCCESS:
        {
            QMessageBox::information(nullptr, "Информация", "Запись удалена", QMessageBox::Ok);
            m_table -> setRowCount(0);
            SendToServer(SendingCodes::GET_RECORDS, m_types -> getType() + " " + m_dbWindow.getName());
            break;
        }
        case SendingCodes::DELETE_RECORD_FAIL:
        {
            m_deleteWidget -> setError("Запись не найдена");
            subWindowDelete -> show();
            break;
        }
        case SendingCodes::FIND_RECORD_FAIL:
        {
            SendToServer(SendingCodes::GET_RECORDS, m_types -> getType() + " " + m_dbWindow.getName());
            m_findWidget -> setError("Запись не найдена");
            subWindowFind -> show();
            break;
        }
        case SendingCodes::GET_RECORDS_SUCCESS:
        {
            uint32_t count = 0;
            std::string str = string.toStdString(), field;
            m_table -> insertRow(compRecordsCount);

            for (int i = 0; i < 12; i++)
            {
                str = getWord(str, field);
                std::replace(field.begin(), field.end(), '|', ' ');
                m_table -> setItem(compRecordsCount, count++, new QTableWidgetItem(field.c_str()));
            }

            if (m_types -> getType() == "Компьютер")
            {
                for (int i = 0; i < 6; i++)
                {
                    str = getWord(str, field);
                    std::replace(field.begin(), field.end(), '|', ' ');
                    m_table -> setItem(compRecordsCount, count++, new QTableWidgetItem(field.c_str()));
                }
            }
            if (m_types -> getType() == "Моб.Телефон")
            {
                for (int i = 0; i < 7; i++)
                {
                    str = getWord(str, field);
                    std::replace(field.begin(), field.end(), '|', ' ');
                    m_table -> setItem(compRecordsCount, count++, new QTableWidgetItem(field.c_str()));
                }
            }
            if (m_types -> getType() == "Телевизор")
            {
                for (int i = 0; i < 6; i++)
                {
                    str = getWord(str, field);
                    std::replace(field.begin(), field.end(), '|', ' ');
                    m_table -> setItem(compRecordsCount, count++, new QTableWidgetItem(field.c_str()));
                }
            }
            if (m_types -> getType() == "Тостер")
            {
                for (int i = 0; i < 4; i++)
                {
                    str = getWord(str, field);
                    std::replace(field.begin(), field.end(), '|', ' ');
                    m_table -> setItem(compRecordsCount, count++, new QTableWidgetItem(field.c_str()));
                }
            }
            if (m_types -> getType() == "Кофемашина")
            {
                for (int i = 0; i < 3; i++)
                {
                    str = getWord(str, field);
                    std::replace(field.begin(), field.end(), '|', ' ');
                    m_table -> setItem(compRecordsCount, count++, new QTableWidgetItem(field.c_str()));
                }
            }
            if (m_types -> getType() == "Эл.Чайник")
            {
                for (int i = 0; i < 3; i++)
                {
                    str = getWord(str, field);
                    std::replace(field.begin(), field.end(), '|', ' ');
                    m_table -> setItem(compRecordsCount, count++, new QTableWidgetItem(field.c_str()));
                }
            }
            if (m_types -> getType() == "Холодильник")
            {
                for (int i = 0; i < 4; i++)
                {
                    str = getWord(str, field);
                    std::replace(field.begin(), field.end(), '|', ' ');
                    m_table -> setItem(compRecordsCount, count++, new QTableWidgetItem(field.c_str()));
                }
            }
            if (m_types -> getType() == "Кондиционер")
            {
                for (int i = 0; i < 4; i++)
                {
                    str = getWord(str, field);
                    std::replace(field.begin(), field.end(), '|', ' ');
                    m_table -> setItem(compRecordsCount, count++, new QTableWidgetItem(field.c_str()));
                }
            }
            if (m_types -> getType() == "Микроволновка")
            {
                for (int i = 0; i < 5; i++)
                {
                    str = getWord(str, field);
                    std::replace(field.begin(), field.end(), '|', ' ');
                    m_table -> setItem(compRecordsCount, count++, new QTableWidgetItem(field.c_str()));
                }
            }
            rowChanged[compRecordsCount] = false;
            break;
        }
        case SendingCodes::SORT_RECORDS_SUCCESS:
        {
            m_table -> setRowCount(0);
            SendToServer(SendingCodes::GET_RECORDS, m_types -> getType() + " " + m_dbWindow.getName());
            break;
        }
        default: qFatal("Ошибка, сервер отправил ошибочные данные");
    }
}

void MainWindow::slotReadyRead()
{
    QDataStream in(m_socket);
    qint16 result;
    QString string;
    uint32_t compRecordsCount = 0;

    while (true)
    {

        if (!m_nextBlockSize)
        {
            if (m_socket -> bytesAvailable() < sizeof(quint16))
            {
                break;
            }
            in >> m_nextBlockSize;
        }

        if (m_socket -> bytesAvailable() < m_nextBlockSize)
        {
            break;
        }
        in >> result >> string;
        handleResult(compRecordsCount++, result, string);
        m_nextBlockSize = 0;
    }
}

void MainWindow::SendToServer(qint16 choice, QString string)
{
    QByteArray  arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);

    out << quint16(0) << choice << string;
    out.device() -> seek(0);
    out << quint16(arrBlock.size() - sizeof(quint16));

    m_socket -> write(arrBlock);
}
