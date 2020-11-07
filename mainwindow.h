#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "auth_window.h"
#include "regi_window.h"
#include "incryption.h"
#include "constants.inc"
#include <QTcpSocket>
#include <QMainWindow>
#include <QMenu>

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(const QString& hostname, int port, QWidget *parent = nullptr);
    ~MainWindow();
    void SendToServer(qint16 choice, QString login, QString password);
    void display();

private slots:
    void registerWindowShow();
    void Authorization();
    void Registration();
    void slotFullScreen();
    void slotReadyRead();

private:
    enum MAX_MENUS {
        FILE,
        EDIT,
        WINDOW,
        HELP,
        MAX_MENUS
    };

private:
    QMenu* m_menu[MAX_MENUS];
    QAction* full_screen;
    AuthWindow m_auth;
    RegiWindow m_regi;
    VigenereCipher m_msg;
    QTcpSocket* m_socket;
    quint16 m_nextBlockSize;
};

#endif // MAINWINDOW_H
