#include "server.h"
#include "constants.inc"
#include "streamtable.h"
#include <windows.h>
#include <cstring>
#include <iostream>

MyServer::MyServer(const std::string& filename, const QString& hostname, uint32_t port, QWidget* parent)
    : QWidget(parent), m_nextBlockSize(0), m_filename(filename) {

    readFile();
    m_tcpServer = new QTcpServer(this);
    if (!m_tcpServer -> listen(QHostAddress(hostname), port)) {
        m_tcpServer->close();
        qFatal("Server can't listeting.");
    } else {
        std::cout << "Server listening." << std::endl << std::endl; system("pause");
    }

    connect(m_tcpServer, SIGNAL(newConnection()), this, SLOT(slotNewConnection()));
}

void MyServer::slotNewConnection() {
    QTcpSocket* clientSocket = m_tcpServer -> nextPendingConnection();
    uint32_t idusersoc = clientSocket -> socketDescriptor();
    SClients[idusersoc] = clientSocket;
    std::cout << "\a";
    connect(SClients[idusersoc], SIGNAL(disconnected()), clientSocket, SLOT(deleteLater()));
    connect(SClients[idusersoc], SIGNAL(readyRead()), this, SLOT(slotReadClient()));
}

void MyServer::slotReadClient() { // данные могут идти частями, в этот слот будет заходить всегда при приходе любого количества байт, будь то все данные, либо их часть
    QTcpSocket* clientSocket = (QTcpSocket*)sender();
    uint32_t idusersoc = clientSocket -> socketDescriptor();

    QDataStream in(clientSocket);
    qint16 choice;
    QString login, password;

    while (true) {

        if (!m_nextBlockSize) { // по умолчанию 0

            if (clientSocket -> bytesAvailable() < static_cast<qint64>(sizeof(quint16))) { // если пришедших байт, меньше чем размер, то значит данные идут частями и часть еще не дошла
                break;
            }
            in >> m_nextBlockSize; // это выполняется, если пришедшие байты равны переменной размера, и следовательно данные пришли полностю и можно считать размер следующего блока
        }

        if (clientSocket -> bytesAvailable() < m_nextBlockSize) { // если данные пришли не полностью выходим
            break;
        }
        in >> choice >> login >> password; // считываем данные
        m_nextBlockSize = 0;
    }

    m_msg.decrypt("LOGIN", login);
    m_msg.decrypt("PASSWORD", password);

    switch(choice) {
        case SendingCodes::AUTHENTIFICATION:
        {
            if(DATABASE.findAccount(login.toStdString(), password.toStdString())) {
                sendToClient(clientSocket, SendingCodes::SUCCESS_AUTHENTIFICATION);
            } else {
                sendToClient(clientSocket, SendingCodes::FAIL_AUTHENTIFICATION);
            }
            break;
        }
        case SendingCodes::REGISTRATION:
        {
            if (!DATABASE.find(login.toStdString())) {
                sendToClient(clientSocket, SendingCodes::SUCCESS_REGISTRATION);
                DATABASE.add(new Account(login.toStdString(), password.toStdString()));
                writeFile(login.toStdString(), password.toStdString());
                std::cout << "a";
            } else {
                sendToClient(clientSocket, SendingCodes::FAIL_REGISTRATION);
            }
            break;
        }
        default: qFatal("Error. Client send wrong data.");
    }

    SClients.remove(idusersoc);

}

void MyServer::sendToClient(QTcpSocket* pSocket, qint16 choice, const QString& login, const QString& password) {
    QByteArray  arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);

    out << quint16(0) << choice << login << password;
    out.device() -> seek(0);
    out << quint16(arrBlock.size() - sizeof(quint16));

    pSocket -> write(arrBlock);
}

void MyServer::sendToClient(QTcpSocket* pSocket, const qint16 res) {
    QByteArray  arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);

    out << quint16(0) << res;
    out.device() -> seek(0);
    out << quint16(arrBlock.size() - sizeof(quint16));

    pSocket -> write(arrBlock);
}

void MyServer::writeFile(const std::string& login, const std::string& password) {
    StreamTable st(file);
    st.AddCol(20);
    st.AddCol(20);
    st.MakeBorderExt(true);
    st.SetDelimRow(true, '-');
    st.SetDelimCol(true, '|');

    file.open(m_filename, std::ios_base::app);
    st << login << password;
    file.close();
}

void MyServer::readFile() {
    std::string login, password;
    char buf[44], log[22], pass[22];

    StreamTable st(file);
    st.AddCol(20);
    st.AddCol(20);
    st.MakeBorderExt(true);
    st.SetDelimRow(true, '-');
    st.SetDelimCol(true, '|');

    file.open(m_filename, std::ios_base::in);
    while (!file.eof())
    {
        login.clear();
        password.clear();
        file.read(buf, 44);
        std::strcpy(log, "");
        file.read(log, 22);
        for (size_t i = 0; i < std::strlen(log); i++) {
            if (log[i] != ' ' && log[i] != '|' && log[i] != '\n') {
                login += log[i];
            }
        }
        std::strcpy(pass, "");
        file.read(pass, 22);
        for (size_t i = 0; i < std::strlen(pass); i++) {
            if (pass[i] != ' ' && pass[i] != '|' && pass[i] != '\n') {
                password += pass[i];
            }
        }
        if (file.eof()) break;
        DATABASE.add(new Account(login, password));
    }
    file.close();
}

void MyServer::rewriteBD()
{
    StreamTable st(file);
    st.AddCol(20);
    st.AddCol(20);
    st.MakeBorderExt(true);
    st.SetDelimRow(true, '-');
    st.SetDelimCol(true, '|');

    file.open(m_filename, std::ios_base::out);
    for (std::int32_t i = DATABASE.length() - 1; i >= 0; i--)
    {
        st << DATABASE[i] -> m_object -> getLogin() << DATABASE[i] -> m_object -> getPassword();
    }
    file.close();
}
