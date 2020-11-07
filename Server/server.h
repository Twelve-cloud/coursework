#ifndef SERVER_H
#define SERVER_H

#include "account.h"
#include "incryption.h"
#include <QTcpServer>
#include <QTcpSocket>
#include <QWidget>
#include <QMap>
#include <QString>
#include <fstream>
#include <cstdint>

class MyServer: public QWidget {
    Q_OBJECT
private:
    void sendToClient(QTcpSocket* socket, qint16 choice, const QString& login, const QString& password);
    void sendToClient(QTcpSocket* socket, qint16 res);
    void writeFile(const std::string& login, const std::string& password);
    void readFile();

public:
    MyServer(const std::string& filename, const QString& hostname, std::uint32_t port, QWidget* parent = nullptr);
    AccountBase* getDatabase() { return &DATABASE; }
    void rewriteBD();

public slots:
    void slotNewConnection();
    void slotReadClient   ();

private:
    QTcpServer* m_tcpServer;
    QMap<int, QTcpSocket*> SClients;
    quint16     m_nextBlockSize;
    AccountBase DATABASE;
    VigenereCipher m_msg;
    std::string m_filename;
    std::fstream file;
};

#endif // SERVER_H
