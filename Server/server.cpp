 #include "server.h"
#include "constants.h"
#include <QString>
#include <windows.h>
#include <cstring>
#include <iostream>

void getWords(int count, std::string str, ...);

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
    QString string;

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
        in >> choice >> string; // считываем данные
        m_nextBlockSize = 0;
    }

    switch(choice)
    {
        case SendingCodes::AUTHENTIFICATION:
        {
            char login[64], password[64];
            getWords(3, string.toStdString(), login, password);
            if(DATABASE -> findObject(login, password))
            {
                sendToClient(clientSocket, SendingCodes::SUCCESS_AUTHENTIFICATION, "");
            }
            else
            {
                sendToClient(clientSocket, SendingCodes::FAIL_AUTHENTIFICATION, "");
            }
            break;
        }
        case SendingCodes::REGISTRATION:
        {
            char login[64], password[64];
            getWords(3, string.toStdString(), login, password);
            if (!DATABASE -> findObject(login))
            {
                sendToClient(clientSocket, SendingCodes::SUCCESS_REGISTRATION, "");
                DATABASE -> push_back(new Account(login, password));
                DATABASE -> rewrite();
            }
            else
            {
                sendToClient(clientSocket, SendingCodes::FAIL_REGISTRATION, "");
            }
            break;
        }
        case SendingCodes::CREATE_DB:
        {
            char dbname[64], dbpass[64];
            getWords(3, string.toStdString(), dbname, dbpass);
            if (!TECH_BASE -> findDbName(dbname))
            {
                sendToClient(clientSocket, SendingCodes::DATABASE_CREATION_SUCCESS, "");
                TECH_BASE -> addDb(new TechBase(dbname, dbpass));
                TECH_BASE -> rewriteDB();
            }
            else
            {
                sendToClient(clientSocket, SendingCodes::DATABASE_CREATION_FAIL, "");
            }
            break;
        }
        case SendingCodes::CONNECT_DB:
        {
            char dbname[64], dbpass[64];
            getWords(3, string.toStdString(), dbname, dbpass);
            if(TECH_BASE -> findDbName(dbname, dbpass))
            {
                sendToClient(clientSocket, SendingCodes::DATABASE_CONNECTION_SUCCESS, "");
            }
            else
            {
                sendToClient(clientSocket, SendingCodes::DATABASE_CONNECTION_FAIL, "");
            }
            break;
        }
        case SendingCodes::GET_RECORDS:
        {
            char type[64], dbname[64];
            uint32_t index = -1;
            std::string stringToSend;
            getWords(3, string.toStdString(), type, dbname);
            if (TECH_BASE -> findDbName(dbname, index))
            {
                for (Tech* i : *TECH_BASE -> getDB(index))
                {
                    if (i -> getType() == type)
                    {
                        i -> getStringToSend(stringToSend);
                        sendToClient(clientSocket, SendingCodes::GET_RECORDS_SUCCESS, QString().fromStdString(stringToSend));
                        stringToSend.clear();
                    }
                }
            }
            break;
        }
        default: qFatal("Error. Client send wrong data.");
    }

    SClients.remove(sd);
}

void MyServer::sendToClient(QTcpSocket* pSocket, const qint16 res, QString string)
{
    QByteArray  arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);

    out << quint16(0) << res << string;
    out.device() -> seek(0);
    out << quint16(arrBlock.size() - sizeof(quint16));

    pSocket -> write(arrBlock);
}
