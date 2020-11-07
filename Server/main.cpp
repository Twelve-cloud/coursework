#include "server.h"
#include "mythread.h"
#include <QApplication>
#include <iostream>
#include <cstdint>

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    std::string filename = "D:\\Qt\\Projects\\coursework\\Server\\test.txt";
    MyServer tcp_server(filename, "127.0.0.1", 1280);
    MyThread admin(&tcp_server, filename);
    admin.start();

    return app.exec();
}
