#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "auth_window.h"
#include "regi_window.h"
#include "createdb_window.h"
#include "type_window.h"
#include "action_window.h"
#include "delete_widget.h"
#include "find_widget.h"
#include "sort_widget.h"
#include "greetingwindow.h"
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
    void getHeaders(QString type);
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
    void slot_sortRecordsClicked();
    void slot_sortTypeClicked();
    void slot_sortCancelClicked();
    void slot_deleteOkClicked();
    void slot_deleteCancelClicked();
    void slot_deleteRecordClicked();
    void slot_findOkClicked();
    void slot_findCancelClicked();
    void slot_findRecordClicked();
    void slot_addRecordClicked();
    void slot_changeRecordClicked();
    void slot_cellChanged(int row, int col);

private:
    enum MAX_MENUS
    {
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
    QMdiSubWindow* subWindowDelete;
    QMdiSubWindow* subWindowFind;
    QMdiSubWindow* subWindowSort;
    QMdiSubWindow* subWindowGreeting;

    DeleteWidget* m_deleteWidget;
    FindWidget* m_findWidget;
    SortWidget* m_sortWidget;
    GreetingWindow* m_greetingWidget;

    QMenu* m_menu[MAX_MENUS];
    std::vector<bool> rowChanged;
    QAction* full_screen;
    AuthWindow m_auth;
    RegiWindow m_regi;
    CreateDB_Window m_dbWindow;
    VigenereCipher m_msg;
    QTcpSocket* m_socket;
    quint16 m_nextBlockSize;
};

#endif // MAINWINDOW_H
