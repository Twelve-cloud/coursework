#include "server.h"
#include "sysfunction.h"
#include "constants.h"
#include <QString>
#include <windows.h>
#include <cstring>
#include <iostream>


MyServer::MyServer(const QString& hostname, uint32_t port, QWidget* parent): QWidget(parent), m_nextBlockSize(0)
{
    m_tcpServer = new QTcpServer(this);

    if (!m_tcpServer -> listen(QHostAddress(hostname), port))
    {
        m_tcpServer -> close();
        std::cout << Rus("Сервер не находится в режиме прослушивания несущей частоты") << std::endl;
        std::exit(-1);
    }
    else
    {
        std::cout << Rus("Сервер находится в режиме прослушивания несущей частоты") << std::endl << std::endl;
        system("pause");
    }

    connect(m_tcpServer, SIGNAL(newConnection()), this, SLOT(slotNewConnection()));
}

void MyServer::slotNewConnection()
{
    QTcpSocket* clientSocket = m_tcpServer -> nextPendingConnection();
    uint32_t sd = clientSocket -> socketDescriptor();
    SClients[sd] = clientSocket;

    connect(SClients[sd], SIGNAL(disconnected()), clientSocket, SLOT(deleteLater()));
    connect(SClients[sd], SIGNAL(readyRead()), this, SLOT(slotReadClient()));
}

void MyServer::handleRequest(QTcpSocket* clientSocket, qint16 choice, std::string string)
{
    switch(choice)
    {
        case SendingCodes::AUTHENTIFICATION:
        {
            char login[64], password[64];
            getWords(3, string, login, password);
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
            getWords(3, string, login, password);
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
            getWords(3, string, dbname, dbpass);
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
            getWords(3, string, dbname, dbpass);
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
        case SendingCodes::ADD_RECORD:
        {
            std::map<std::string, Tech*> create_type =  {{Rus("Компьютер"), new Computer},{Rus("Моб.Телефон"), new MobilePhone}, {Rus("Телевизор"), new TV},
                                                         {Rus("Тостер"), new Toaster}, {Rus("Кофемашина"), new CoffeMaker}, {Rus("Эл.Чайник"), new ElKettle},
                                                         {Rus("Холодильник"), new Fridge}, {Rus("Кондиционер"), new Conditioner}, {Rus("Микроволновка"), new Microwawe}};
            char dbname[64], type[64];
            uint32_t index = -1;
            getWords(3, string, dbname, type);
            if(TECH_BASE -> findDbName(dbname, index))
            {
                (*TECH_BASE)[index] -> addObject(create_type[type]->getTypeClass(), 0);
                (*TECH_BASE)[index] -> rewriteDB();
            }
            else
            {
                sendToClient(clientSocket, SendingCodes::ADD_RECORD_FAIL, "");
            }
            break;
        }
        case SendingCodes::DELETE_RECORD:
        {
            char dbname[64], type[64], id[64];
            getWords(4, string, dbname, type, id);
            uint32_t ID = atoi(id), index = -1;
            bool isFound = false;
            if(TECH_BASE -> findDbName(dbname, index))
            {
                for (Tech* i : *TECH_BASE -> getDB(index))
                {
                    std::string record_type = Rus(i -> getType().c_str());
                    if (i -> getID() == ID && record_type == type)
                    {
                        (*TECH_BASE)[index] -> remObject(ID);
                        sendToClient(clientSocket, SendingCodes::DELETE_RECORD_SUCCESS, "");
                        isFound = true;
                        break;
                    }
                }
                if (!isFound)
                {
                    sendToClient(clientSocket, SendingCodes::DELETE_RECORD_FAIL, "");
                }
            }
            else
            {
                sendToClient(clientSocket, SendingCodes::DELETE_RECORD_FAIL, "");
            }
            break;
        }
        case SendingCodes::CHANGE_RECORD:
        {
            char dbname[64], type[64], id[64];
            std::string stringToReplace = getWords(4, string, dbname, type, id);
            uint32_t ID = atoi(id), index = -1;
            if(TECH_BASE -> findDbName(dbname, index))
            {
                for (Tech* i : *TECH_BASE -> getDB(index))
                {
                    std::string record_type = Rus(i -> getType().c_str());
                    if (i -> getID() == ID && record_type == type)
                    {
                        i -> replaceObject(stringToReplace); break;
                    }
                }
                TECH_BASE -> getDB(index) -> rewriteDB();
            }
            else
            {
                sendToClient(clientSocket, SendingCodes::CHANGE_RECORD_FAIL, "");
            }
            break;
        }
        case SendingCodes::FIND_RECORD:
        {
            char dbname[64], type[64], id[64];
            std::string stringToSend;
            getWords(4, string, dbname, type, id);
            uint32_t ID = atoi(id), index = -1;
            bool isFound = false;
            if(TECH_BASE -> findDbName(dbname, index))
            {
                for (Tech* i : *TECH_BASE -> getDB(index))
                {
                    std::string record_type = Rus(i -> getType().c_str());
                    if (i -> getID() == ID && record_type == type)
                    {
                        i -> getStringToSend(stringToSend);
                        sendToClient(clientSocket, SendingCodes::GET_RECORDS_SUCCESS, QString().fromStdString(stringToSend));
                        stringToSend.clear();
                        isFound = true;
                        break;
                    }
                }
                if (!isFound)
                {
                    sendToClient(clientSocket, SendingCodes::FIND_RECORD_FAIL, "");
                }
            }
            else
            {
                sendToClient(clientSocket, SendingCodes::FIND_RECORD_FAIL, "");
            }
            break;
        }
        case SendingCodes::GET_RECORDS:
        {
            char type[64], dbname[64];
            uint32_t index = -1;
            std::string stringToSend;
            getWords(3, string, type, dbname);
            if (TECH_BASE -> findDbName(dbname, index))
            {
                for (Tech* i : *TECH_BASE -> getDB(index))
                {
                    std::string record_type = Rus(i -> getType().c_str());
                    if (record_type == type)
                    {
                        i -> getStringToSend(stringToSend);
                        sendToClient(clientSocket, SendingCodes::GET_RECORDS_SUCCESS, QString().fromStdString(stringToSend));
                        stringToSend.clear();
                    }
                }
            }
            break;
        }
        case SendingCodes::SORT_RECORDS:
        {
            char type[64], dbname[64];
            uint32_t index = -1;
            std::string stringToSend;
            getWords(3, string, type, dbname);
            if (TECH_BASE -> findDbName(dbname, index))
            {
                (*TECH_BASE)[index] -> sort(type);
                sendToClient(clientSocket, SendingCodes::SORT_RECORDS_SUCCESS, "");
            }
            break;
        }
        default: std::cout << Rus("Ошибка. Клиент отправить ошибочные данные") << std::endl;
    }
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
        std::string stringToHandle = Rus(string.toStdString().c_str());
        handleRequest(clientSocket, choice, stringToHandle);
        m_nextBlockSize = 0;
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
