#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "auth_window.h"
#include "regi_window.h"
#include "createdb_window.h"
#include "type_window.h"
#include "action_window.h"
#include "incryption.h"
#include "constants.h"
#include <QTcpSocket>
#include <QMdiArea>
#include <QMainWindow>
#include <QMenu>
#include <QTableWidget>

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(const QString& hostname, int port, QWidget *parent = nullptr);
    ~MainWindow();
    void SendToServer(qint16 choice, QString string);
    void handleResult(uint32_t compRecordsCount, qint16 result, QString string);
    void getColsTable(QString type);
    void display();

private slots:
    void registerWindowShow();
    void Authorization();
    void Registration();
    void slotFullScreen();
    void slotReadyRead();
    void backToAuth();
    void sendDbData();
    void createDB();
    void connectDB();
    void exit();
    void slot_menu_actions_call();

private:
    enum MAX_MENUS {
        FILE,
        EDIT,
        WINDOW,
        HELP,
        MAX_MENUS
    };

private:
    QMdiArea* leftMenu;
    TypeWindow* m_types;
    ActionWindow* m_actions;
    QTableWidget* m_table;

    QMdiSubWindow* subWindowTypes;
    QMdiSubWindow* subWindowActions;
    QMdiSubWindow* subWindowTable;

    QMenu* m_menu[MAX_MENUS];
    QAction* full_screen;
    AuthWindow m_auth;
    RegiWindow m_regi;
    CreateDB_Window m_dbWindow;
    VigenereCipher m_msg;
    QTcpSocket* m_socket;
    quint16 m_nextBlockSize;
};

#endif // MAINWINDOW_H
