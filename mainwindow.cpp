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

    setWindowTitle("Main Window");
    setWindowState(Qt::WindowMaximized);

    m_menu[FILE]    = menuBar() -> addMenu("File");
    m_menu[EDIT]    = menuBar() -> addMenu("Edit");
    m_menu[WINDOW]  = menuBar() -> addMenu("Window");
    m_menu[HELP]    = menuBar() -> addMenu("Help");

    QAction *quit = new QAction("&Quit");
    quit -> setShortcut(tr("Ctrl+Q"));

    QAction *back = new QAction("&Log out");
    back -> setShortcut(tr("Ctrl+B"));

    QAction *createDB = new QAction("Create DB");
    createDB -> setShortcut(tr("Ctrl+N"));

    QAction *connectDB = new QAction("Connect to DB");
    connectDB -> setShortcut(tr("Ctrl+L"));

    full_screen = new QAction("Fullscreen mode");
    full_screen -> setShortcut(tr("Ctrl+Shift+F11"));
    full_screen -> setCheckable(true);

    QAction* about = new QAction("About");
    about -> setShortcut(tr("F4"));

    m_menu[FILE]    -> addAction(createDB);
    m_menu[FILE]    -> addAction(connectDB);
    m_menu[FILE]    -> addAction(back);
    m_menu[FILE]    -> addAction(quit);
    m_menu[WINDOW]  -> addAction(full_screen);
    m_menu[HELP]    -> addAction(about);

    leftMenu = new QMdiArea(this);

    m_types = new TypeWindow(leftMenu);
    subWindowTypes = leftMenu -> addSubWindow(m_types);
    subWindowTypes -> setWindowFlags(Qt::Window | Qt::WindowTitleHint | Qt::CustomizeWindowHint | Qt::FramelessWindowHint);
    subWindowTypes -> move(0, 0);
    subWindowTypes -> setFixedSize(260, QGuiApplication::screens().at(0)->geometry().height() - this->height() + 10);
    subWindowTypes -> hide();

    m_actions = new ActionWindow(leftMenu);
    subWindowActions = leftMenu -> addSubWindow(m_actions);
    subWindowActions -> setWindowFlags(Qt::Window | Qt::WindowTitleHint | Qt::CustomizeWindowHint | Qt::FramelessWindowHint);
    subWindowActions -> move(260, 0);
    subWindowActions -> setFixedSize(260, QGuiApplication::screens().at(0)->geometry().height() - this->height() + 10);
    subWindowActions -> hide();

    m_table = new QTableWidget;

    subWindowTable = leftMenu -> addSubWindow(m_table);
    subWindowTable -> setWindowFlags(Qt::Window | Qt::WindowTitleHint | Qt::CustomizeWindowHint | Qt::FramelessWindowHint);
    subWindowTable -> move(520, 0);
    subWindowTable -> setFixedSize(QGuiApplication::screens().at(0)->geometry().width() - 520, QGuiApplication::screens().at(0)->geometry().height() - this->height() + 10);
    subWindowTable -> hide();

    setCentralWidget(leftMenu);

    connect(m_types, SIGNAL(menu_actions_call()), this, SLOT(slot_menu_actions_call()));
    connect(createDB, SIGNAL(triggered()), this, SLOT(createDB()));
    connect(connectDB, SIGNAL(triggered()), this, SLOT(connectDB()));
    connect(back, SIGNAL(triggered()), this, SLOT(exit()));
    connect(quit, SIGNAL(triggered()), qApp, SLOT(quit()));
    connect(full_screen, SIGNAL(triggered()), this, SLOT(slotFullScreen()));
}

MainWindow::~MainWindow() {}

void MainWindow::slotFullScreen() {
    if (full_screen -> isChecked()) {
        QMainWindow::showFullScreen();
    } else {
        setWindowState(Qt::WindowMaximized);
    }
}

void MainWindow::display() {
    m_auth.show();
}

void MainWindow::registerWindowShow() {
    m_regi.clearLines();
    m_regi.setError("Registration");
    m_auth.hide();
    m_regi.show();
}

void MainWindow::Authorization() {
    if (!m_auth.isEmptyLine()) {
        SendToServer(SendingCodes::AUTHENTIFICATION, m_auth.getLogin() + " " + m_auth.getPassword());
    } else {
        m_auth.setError("Fill all fields");
    }
}

void MainWindow::Registration() {
    if (m_regi.checkPass()) {
        SendToServer(SendingCodes::REGISTRATION, m_regi.getLogin() + " " + m_regi.getPassword());
    } else {
        m_regi.setError("Confirm incorrect");
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
    m_dbWindow.setError("Database settings");
    m_dbWindow.changeOkName("Create");
    m_dbWindow.clearLines();
    m_dbWindow.show();
}

void MainWindow::connectDB()
{
    m_dbWindow.setError("Database settings");
    m_dbWindow.changeOkName("Connect");
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
    SendToServer(SendingCodes::GET_RECORDS, m_types -> getType() + " " + m_dbWindow.getName());
}

void MainWindow::getColsTable(QString type)
{
    char* Tech[] = {"ID", "Type", "Date", "Time", "Day", "Serial Number", " Manufacturer", "Release Date", "Model", "Vendor", "Countrymaker", "Price"};

    if (type == "Computer")
    {
        m_table -> setColumnCount(18);
    }
    if (type == "MobilePhone")
    {
        m_table -> setColumnCount(19);
    }
    if (type == "TV")
    {
        m_table -> setColumnCount(18);
    }
    if (type == "Toaster")
    {
        m_table -> setColumnCount(16);
    }
    if (type == "CoffeeMaker")
    {
        m_table -> setColumnCount(15);
    }
    if (type == "ElectricKettle")
    {
        m_table -> setColumnCount(15);
    }
    if (type == "Fridge")
    {
        m_table -> setColumnCount(16);
    }
    if (type == "Conditioner")
    {
        m_table -> setColumnCount(16);
    }
    if (type == "Microwave")
    {
        m_table -> setColumnCount(17);
    }

    for (int i = 0; i < 12; i++)
    {
        m_table -> setHorizontalHeaderItem(i, new QTableWidgetItem(Tech[i]));
    }
}

void MainWindow::handleResult(uint32_t compRecordsCount, qint16 result, QString string)
{

    switch(result)
    {
        case SendingCodes::FAIL_AUTHENTIFICATION:
        {
            m_auth.setError("Wrong Login or Password");
            break;
        }
        case SendingCodes::FAIL_REGISTRATION:
        {
            m_regi.setError("Username already in use");
            break;
        }
        case SendingCodes::SUCCESS_REGISTRATION:
        {
            m_auth.setError("Authentification");
            m_auth.clearLines();
            QMessageBox::information(nullptr, "Information", "Registration Complete", QMessageBox::Ok);
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
            m_dbWindow.setError("This basename is already exist");
            break;
        }
        case SendingCodes::DATABASE_CONNECTION_FAIL:
        {
            m_dbWindow.setError("Wrong name or password");
            break;
        }
        case SendingCodes::DATABASE_CREATION_SUCCESS:
        {
            m_dbWindow.clearLines();
            m_dbWindow.hide();
            QMessageBox::information(nullptr, "Information", "Database created", QMessageBox::Ok);
            break;
        }
        case SendingCodes::DATABASE_CONNECTION_SUCCESS:
        {
            m_dbWindow.clearLines();
            m_dbWindow.hide();
            subWindowTypes -> show();
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
                m_table -> setItem(compRecordsCount, count++, new QTableWidgetItem(field.c_str()));
            }

            if (m_types -> getType() == "Computer")
            {
                for (int i = 0; i < 6; i++)
                {
                    str = getWord(str, field);
                    m_table -> setItem(compRecordsCount, count++, new QTableWidgetItem(field.c_str()));
                }
            }
            if (m_types -> getType() == "MobilePhone")
            {
                for (int i = 0; i < 7; i++)
                {
                    str = getWord(str, field);
                    m_table -> setItem(compRecordsCount, count++, new QTableWidgetItem(field.c_str()));
                }
            }
            if (m_types -> getType() == "TV")
            {
                for (int i = 0; i < 6; i++)
                {
                    str = getWord(str, field);
                    m_table -> setItem(compRecordsCount, count++, new QTableWidgetItem(field.c_str()));
                }
            }
            if (m_types -> getType() == "Toaster")
            {
                for (int i = 0; i < 4; i++)
                {
                    str = getWord(str, field);
                    m_table -> setItem(compRecordsCount, count++, new QTableWidgetItem(field.c_str()));
                }
            }
            if (m_types -> getType() == "CoffeeMaker")
            {
                for (int i = 0; i < 3; i++)
                {
                    str = getWord(str, field);
                    m_table -> setItem(compRecordsCount, count++, new QTableWidgetItem(field.c_str()));
                }
            }
            if (m_types -> getType() == "ElectricKettle")
            {
                for (int i = 0; i < 3; i++)
                {
                    str = getWord(str, field);
                    m_table -> setItem(compRecordsCount, count++, new QTableWidgetItem(field.c_str()));
                }
            }
            if (m_types -> getType() == "Fridge")
            {
                for (int i = 0; i < 4; i++)
                {
                    str = getWord(str, field);
                    m_table -> setItem(compRecordsCount, count++, new QTableWidgetItem(field.c_str()));
                }
            }
            if (m_types -> getType() == "Conditioner")
            {
                for (int i = 0; i < 4; i++)
                {
                    str = getWord(str, field);
                    m_table -> setItem(compRecordsCount, count++, new QTableWidgetItem(field.c_str()));
                }
            }
            if (m_types -> getType() == "Microwave")
            {
                for (int i = 0; i < 5; i++)
                {
                    str = getWord(str, field);
                    m_table -> setItem(compRecordsCount, count++, new QTableWidgetItem(field.c_str()));
                }
            }
            break;
        }
        default: qFatal("Error. Server send wrong data.");
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
