#include "server.h"
#include "constants.h"
#include <windows.h>
#include <cstring>
#include <iostream>

MyServer::MyServer(const QString& hostname, uint32_t port, QWidget* parent): QWidget(parent), m_nextBlockSize(0)
{
    m_tcpServer = new QTcpServer(this);

    if (!m_tcpServer -> listen(QHostAddress(hostname), port))
    {
        m_tcpServer -> close();
        qFatal("Server not listen");
    }
    else
    {
        std::cout << "Server listen" << std::endl << std::endl;
        system("pause");
    }

    connect(m_tcpServer, SIGNAL(newConnection()), this, SLOT(slotNewConnection()));
}

void MyServer::slotNewConnection()
{
    QTcpSocket* clientSocket = m_tcpServer -> nextPendingConnection();
    uint32_t sd = clientSocket -> socketDescriptor();
    SClients[sd] = clientSocket; std::cout << "\a";

    connect(SClients[sd], SIGNAL(disconnected()), clientSocket, SLOT(deleteLater()));
    connect(SClients[sd], SIGNAL(readyRead()), this, SLOT(slotReadClient()));
}

void MyServer::slotReadClient()  // данные могут идти частями, в этот слот будет заходить всегда при приходе любого количества байт, будь то все данные, либо их часть
{
    QTcpSocket* clientSocket = (QTcpSocket*)sender();
    uint32_t sd = clientSocket -> socketDescriptor();

    QDataStream in(clientSocket);
    qint16 choice;
    QString login, password;

    while (true)
    {

        if (!m_nextBlockSize) // по умолчанию 0
        {

            if (clientSocket -> bytesAvailable() < static_cast<qint64>(sizeof(quint16))) // если пришедших байт, меньше чем размер, то значит данные идут частями и часть еще не дошла
            {
                break;
            }
            in >> m_nextBlockSize; // это выполняется, если пришедшие байты равны переменной размера, и следовательно данные пришли полностю и можно считать размер следующего блока
        }

        if (clientSocket -> bytesAvailable() < m_nextBlockSize) // если данные пришли не полностью выходим
        {
            break;
        }
        in >> choice >> login >> password; // считываем данные
        m_nextBlockSize = 0;
    }

    m_msg.decrypt("LOGIN", login);
    m_msg.decrypt("PASSWORD", password);

    switch(choice)
    {
        case SendingCodes::AUTHENTIFICATION:
        {
            if(DATABASE -> findObject(login.toStdString(), password.toStdString()))
            {
                sendToClient(clientSocket, SendingCodes::SUCCESS_AUTHENTIFICATION);
            }
            else
            {
                sendToClient(clientSocket, SendingCodes::FAIL_AUTHENTIFICATION);
            }
            break;
        }
        case SendingCodes::REGISTRATION:
        {
            if (!DATABASE -> findObject(login.toStdString()))
            {
                sendToClient(clientSocket, SendingCodes::SUCCESS_REGISTRATION);
                DATABASE -> push_back(new Account(login.toStdString(), password.toStdString()));
                DATABASE -> rewrite();
            }
            else
            {
                sendToClient(clientSocket, SendingCodes::FAIL_REGISTRATION);
            }
            break;
        }
        case SendingCodes::CREATE_DB:
        {
            if (!TECH_BASE -> findDbName(login.toStdString()))
            {
                sendToClient(clientSocket, SendingCodes::DATABASE_CREATION_SUCCESS);
                TECH_BASE -> addDb(new TechBase(login.toStdString(), password.toStdString()));
                TECH_BASE -> rewriteDB();
            }
            else
            {
                sendToClient(clientSocket, SendingCodes::DATABASE_CREATION_FAIL);
            }
            break;
        }
        case SendingCodes::CONNECT_DB:
        {
            if(TECH_BASE -> findDbName(login.toStdString(), password.toStdString()))
            {
                sendToClient(clientSocket, SendingCodes::DATABASE_CONNECTION_SUCCESS);
            }
            else
            {
                sendToClient(clientSocket, SendingCodes::DATABASE_CONNECTION_FAIL);
            }
            break;
        }
        default: qFatal("Error. Client send wrong data.");
    }

    SClients.remove(sd);
}

void MyServer::sendToClient(QTcpSocket* pSocket, const qint16 res)
{
    QByteArray  arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);

    out << quint16(0) << res;
    out.device() -> seek(0);
    out << quint16(arrBlock.size() - sizeof(quint16));

    pSocket -> write(arrBlock);
}
