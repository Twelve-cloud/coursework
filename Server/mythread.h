#ifndef MYTHREAD_H
#define MYTHREAD_H

#include "server.h"
#include <QObject>
#include <QThread>
#include <string>

class MyThread : public QThread {
     Q_OBJECT
public:
    MyThread(MyServer* tcp_server, std::string& filename) : m_tcpServer(tcp_server), m_filename(filename) {}
    void acc_menu();

protected:
     void run();

private:
    MyServer* m_tcpServer;
    std::string m_filename;
};

#endif // MYTHREAD_H
