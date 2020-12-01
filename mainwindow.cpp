#include "mainwindow.h"
#include "constants.inc"
#include <QMenu>
#include <QApplication>
#include <QMenuBar>
#include <QDebug>
#include <QAction>
#include <QMessageBox>
#include <QShortcut>

MainWindow::MainWindow(const QString& hostname, int port, QWidget *parent)
    : QMainWindow(parent), m_nextBlockSize(0) {
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
        SendToServer(SendingCodes::AUTHENTIFICATION, m_auth.getLogin(), m_auth.getPassword());
    } else {
        m_auth.setError("Fill all fields");
    }
}

void MainWindow::Registration() {
    if (m_regi.checkPass()) {
        SendToServer(SendingCodes::REGISTRATION, m_regi.getLogin(), m_regi.getPassword());
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
        SendToServer(SendingCodes::CREATE_DB, m_dbWindow.getName(), m_dbWindow.getPass());
    }
    else
    {
        SendToServer(SendingCodes::CONNECT_DB, m_dbWindow.getName(), m_dbWindow.getPass());
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

void MainWindow::slotReadyRead()
{
    QDataStream in(m_socket);
    qint16 result;

    while (true) {

        if (!m_nextBlockSize) {
            if (m_socket -> bytesAvailable() < sizeof(quint16)) {
                break;
            }
            in >> m_nextBlockSize;
        }

        if (m_socket -> bytesAvailable() < m_nextBlockSize) {
            break;
        }

        in >> result;

        m_nextBlockSize = 0;
    }


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
            break;
        }
        default: qFatal("Error. Server send wrong data.");
    }
}

void MainWindow::SendToServer(qint16 choice, QString login, QString password) {
    QByteArray  arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);

    m_msg.incrypt("LOGIN", login);
    m_msg.incrypt("PASSWORD", password);

    out << quint16(0) << choice << login << password;
    out.device() -> seek(0);
    out << quint16(arrBlock.size() - sizeof(quint16));

    m_socket -> write(arrBlock);
}
