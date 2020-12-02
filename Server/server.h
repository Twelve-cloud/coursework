#ifndef SERVER_H
#define SERVER_H

#include "tech_base.h"
#include "dbnames_base.h"
#include "account_base.h"
#include "incryption.h"
#include <QTcpServer>
#include <QTcpSocket>
#include <QWidget>
#include <QMap>
#include <QString>
#include <fstream>
#include <cstdint>
#include <vector>

class MyServer: public QWidget
{
    Q_OBJECT
private:
    void sendToClient(QTcpSocket* socket, qint16 res, QString string);

public:
    MyServer(const QString& hostname, std::uint32_t port, QWidget* parent = nullptr);
    void connectToDatabase(AccountBase& db) { DATABASE = &db; DATABASE -> readFile(); };
    void connectToDbNames(DbNamesDatabase& db) { TECH_BASE = &db; TECH_BASE -> readFile(); }
    AccountBase* getDatabase() const { return DATABASE; }
    DbNamesDatabase* getTechBase() const { return TECH_BASE; }

public slots:
    void slotNewConnection();
    void slotReadClient   ();

private:
    QTcpServer* m_tcpServer;
    QMap<int, QTcpSocket*> SClients;
    quint16     m_nextBlockSize;
    VigenereCipher m_msg;

    DbNamesDatabase* TECH_BASE;
    AccountBase* DATABASE;
};

#endif // SERVER_H
