#ifndef MYTHREAD_H
#define MYTHREAD_H

#include "server.h"
#include <QObject>
#include <QThread>
#include <string>

class MyThread : public QThread
{
     Q_OBJECT
public:
    MyThread(MyServer* tcp_server) : m_tcpServer(tcp_server) {}
    void account_management_menu();
    void techbase_management_menu();
    void tech_management_menu(TechBase* DATABASE);
    void run(); // переопределенная функция для запуска куска кода в отдельном потоке

private:
    MyServer* m_tcpServer;
};

#endif // MYTHREAD_H
