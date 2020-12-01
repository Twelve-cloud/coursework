#include "streamtable.h"
#include "dbnames_base.h"
#include "account_base.h"
#include "server.h"
#include "mythread.h"
#include <QApplication>
#include <Windows.h>

StreamTable st(std::cout);
StreamTable kt(std::cout);
StreamTable mt(std::cout);
StreamTable nt(std::cout);

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    st.AddCol(20);
    st.AddCol(20);
    st.MakeBorderExt(true);
    st.SetDelimRow(true, '-');
    st.SetDelimCol(true, '|');

    kt.AddCol(40);
    kt.AddCol(40);
    kt.MakeBorderExt(true);
    kt.SetDelimRow(true, '-');
    kt.SetDelimCol(true, '|');

    mt.AddCol(30);
    mt.MakeBorderExt(true);
    mt.SetDelimRow(true, '-');
    mt.SetDelimCol(true, '|');

    nt.AddCol(30);
    nt.AddCol(30);
    nt.MakeBorderExt(true);
    nt.SetDelimRow(true, '-');
    nt.SetDelimCol(true, '|');

    std::string filename = "db_accounts.txt"; //создаю файл для записи данных
    AccountBase DATABASE(filename); // создаю БД

    std::string dbname = "db_names.txt";
    DbNamesDatabase NAMES_BASE(dbname);

    MyServer tcp_server("127.0.0.1", 1280); //создаю сервер
    tcp_server.connectToDatabase(DATABASE);
    tcp_server.connectToDbNames(NAMES_BASE);

    MyThread admin(&tcp_server);
    admin.start();

    return app.exec();
}
