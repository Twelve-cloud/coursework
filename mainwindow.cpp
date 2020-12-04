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
    subWindowFind -> move(QGuiApplication::screens().at(0)->geometry().width() / 2 - m_deleteWidget -> size().width() / 2,
                            QGuiApplication::screens().at(0)->geometry().height() / 2 - m_deleteWidget -> size().height() / 2);
    subWindowFind -> hide();

    setCentralWidget(leftMenu);

    connect(m_table, SIGNAL(cellChanged(int, int)), this, SLOT(slot_cellChanged(int, int)));
    connect(m_findWidget, SIGNAL(find_ok_clicked()), this, SLOT(slot_findOkClicked()));
    connect(m_findWidget, SIGNAL(find_cancel_clicked()), this, SLOT(slot_findCancelClicked()));
    connect(m_deleteWidget, SIGNAL(delete_ok_clicked()), this, SLOT(slot_deleteOkClicked()));
    connect(m_deleteWidget, SIGNAL(delete_cancel_clicked()), this, SLOT(slot_deleteCancelClicked()));
    connect(m_actions, SIGNAL(add_record_clicked()), this, SLOT(slot_addRecordClicked()));
    connect(m_actions, SIGNAL(delete_record_clicked()), this, SLOT(slot_deleteRecordClicked()));
    connect(m_actions, SIGNAL(change_record_clicked()), this, SLOT(slot_changeRecordClicked()));
    connect(m_actions, SIGNAL(find_record_clicked()), this, SLOT(slot_findRecordClicked()));
    connect(m_actions, SIGNAL(sort_records_clicked()), this, SLOT(slot_sort()));
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
    getHeaders(m_types -> getType());
    rowChanged.clear();
    SendToServer(SendingCodes::GET_RECORDS, m_types -> getType() + " " + m_dbWindow.getName());
}

void MainWindow::slot_sort()
{
    SendToServer(SendingCodes::SORT_RECORDS, " " + m_dbWindow.getName());
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
        m_deleteWidget -> setError("Fill all fields");
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
        m_findWidget -> setError("Fill all fields");
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
    char* Tech[] = {"ID", "Type", "Date", "Time", "Day", "Serial Number", " Manufacturer", "Release Date", "Model", "Vendor", "Countrymaker", "Price"};
    char* Computer[] = {"Processor", "Core", "Ram Type", "Ram Size", "Screen res", "Screen diagonal"};
    char* Mobile[] = {"OS", "Screen res", "Screen diagonal", "Processor", "Core", "Ram Size", "Sim count"};
    char* TV[] = {"Type of screen", "Screen res", "Screen diagonal", "Processor", "Core", "3D"};
    char* Toaster[] = {"Toast count", "Power", "Defrosting mode", "Heating mode"};
    char* CoffeeMaker[] = {"Power", "Pressure", "Cap.maker"};
    char* ElKettle[] = {"Power", "Volume", "Timer"};
    char* Fridge[] = {"Volume", "Shelf count", "Noise lvl", "Multizone"};
    char* Conditioner[] = {"Work mode", "Cooling power", "Heathing power", "Remote contol"};
    char* Microwave[] = {"Power", "Lvls of power", "Volume", "Quickstart", "Timer"};
    int count = 0, childCount = 0;
    for (; count < 12; count++)
    {
        m_table -> setHorizontalHeaderItem(count, new QTableWidgetItem(Tech[count]));
    }

    if (type == "Computer")
    {
        for (; count < 18; count++)
        {
            m_table -> setHorizontalHeaderItem(count, new QTableWidgetItem(Computer[childCount++]));
        }
    }
    if (type == "MobilePhone")
    {
        for (; count < 19; count++)
        {
            m_table -> setHorizontalHeaderItem(count, new QTableWidgetItem(Mobile[childCount++]));
        }
    }
    if (type == "TV")
    {
        for (; count < 18; count++)
        {
            m_table -> setHorizontalHeaderItem(count, new QTableWidgetItem(TV[childCount++]));
        }
    }
    if (type == "Toaster")
    {
        for (; count < 16; count++)
        {
            m_table -> setHorizontalHeaderItem(count, new QTableWidgetItem(Toaster[childCount++]));
        }
    }
    if (type == "CoffeeMaker")
    {
        for (; count < 15; count++)
        {
            m_table -> setHorizontalHeaderItem(count, new QTableWidgetItem(CoffeeMaker[childCount++]));
        }
    }
    if (type == "ElectricKettle")
    {
        for (; count < 15; count++)
        {
            m_table -> setHorizontalHeaderItem(count, new QTableWidgetItem(ElKettle[childCount++]));
        }
    }
    if (type == "Fridge")
    {
        for (; count < 16; count++)
        {
            m_table -> setHorizontalHeaderItem(count, new QTableWidgetItem(Fridge[childCount++]));
        }
    }
    if (type == "Conditioner")
    {
        for (; count < 16; count++)
        {
            m_table -> setHorizontalHeaderItem(count, new QTableWidgetItem(Conditioner[childCount++]));
        }
    }
    if (type == "Microwave")
    {
        for (; count < 17; count++)
        {
            m_table -> setHorizontalHeaderItem(count, new QTableWidgetItem(Microwave[childCount++]));
        }
    }
}

void MainWindow::getColsTable(QString type)
{

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
}

void MainWindow::handleResult(uint32_t compRecordsCount, qint16 result, QString string)
{
    rowChanged.push_back(true);
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
        case SendingCodes::ADD_RECORD_SUCCESS:
        {
            m_table -> setRowCount(0);
            SendToServer(SendingCodes::GET_RECORDS, m_types -> getType() + " " + m_dbWindow.getName());
            break;
        }
        case SendingCodes::ADD_RECORD_FAIL:
        {
            QMessageBox::information(nullptr, "Error", "Can't add record.", QMessageBox::Ok);
            break;
        }
        case SendingCodes::CHANGE_RECORD_FAIL:
        {
            QMessageBox::information(nullptr, "Error", "Can't changed records.", QMessageBox::Ok);
            SendToServer(SendingCodes::GET_RECORDS, m_types -> getType() + " " + m_dbWindow.getName());
            break;
        }
        case SendingCodes::DELETE_RECORD_SUCCESS:
        {
            QMessageBox::information(nullptr, "Information", "Record deleted", QMessageBox::Ok);
            m_table -> setRowCount(0);
            SendToServer(SendingCodes::GET_RECORDS, m_types -> getType() + " " + m_dbWindow.getName());
            break;
        }
        case SendingCodes::DELETE_RECORD_FAIL:
        {
            m_deleteWidget -> setError("Record not found");
            subWindowDelete -> show();
            break;
        }
        case SendingCodes::FIND_RECORD_FAIL:
        {
            SendToServer(SendingCodes::GET_RECORDS, m_types -> getType() + " " + m_dbWindow.getName());
            m_findWidget -> setError("Record not found");
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

            if (m_types -> getType() == "Computer")
            {
                for (int i = 0; i < 6; i++)
                {
                    str = getWord(str, field);
                    std::replace(field.begin(), field.end(), '|', ' ');
                    m_table -> setItem(compRecordsCount, count++, new QTableWidgetItem(field.c_str()));
                }
            }
            if (m_types -> getType() == "MobilePhone")
            {
                for (int i = 0; i < 7; i++)
                {
                    str = getWord(str, field);
                    std::replace(field.begin(), field.end(), '|', ' ');
                    m_table -> setItem(compRecordsCount, count++, new QTableWidgetItem(field.c_str()));
                }
            }
            if (m_types -> getType() == "TV")
            {
                for (int i = 0; i < 6; i++)
                {
                    str = getWord(str, field);
                    std::replace(field.begin(), field.end(), '|', ' ');
                    m_table -> setItem(compRecordsCount, count++, new QTableWidgetItem(field.c_str()));
                }
            }
            if (m_types -> getType() == "Toaster")
            {
                for (int i = 0; i < 4; i++)
                {
                    str = getWord(str, field);
                    std::replace(field.begin(), field.end(), '|', ' ');
                    m_table -> setItem(compRecordsCount, count++, new QTableWidgetItem(field.c_str()));
                }
            }
            if (m_types -> getType() == "CoffeeMaker")
            {
                for (int i = 0; i < 3; i++)
                {
                    str = getWord(str, field);
                    std::replace(field.begin(), field.end(), '|', ' ');
                    m_table -> setItem(compRecordsCount, count++, new QTableWidgetItem(field.c_str()));
                }
            }
            if (m_types -> getType() == "ElectricKettle")
            {
                for (int i = 0; i < 3; i++)
                {
                    str = getWord(str, field);
                    std::replace(field.begin(), field.end(), '|', ' ');
                    m_table -> setItem(compRecordsCount, count++, new QTableWidgetItem(field.c_str()));
                }
            }
            if (m_types -> getType() == "Fridge")
            {
                for (int i = 0; i < 4; i++)
                {
                    str = getWord(str, field);
                    std::replace(field.begin(), field.end(), '|', ' ');
                    m_table -> setItem(compRecordsCount, count++, new QTableWidgetItem(field.c_str()));
                }
            }
            if (m_types -> getType() == "Conditioner")
            {
                for (int i = 0; i < 4; i++)
                {
                    str = getWord(str, field);
                    std::replace(field.begin(), field.end(), '|', ' ');
                    m_table -> setItem(compRecordsCount, count++, new QTableWidgetItem(field.c_str()));
                }
            }
            if (m_types -> getType() == "Microwave")
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
