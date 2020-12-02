#ifndef ACTION_WINDOW_H
#define ACTION_WINDOW_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QString>
#include <QLineEdit>

class ActionWindow : public QWidget
{
    Q_OBJECT
public:
    explicit ActionWindow(QWidget *parent = nullptr);

signals:
    void add_record_clicked();
    void delete_record_clicked();
    void change_record_clicked();
    void print_all_records_clicked();
    void find_record_clicked();
    void sort_records_clicked();

private slots:
    //void add_recordButton_clicked();
    //void delete_recordButton_clicked();
    //void change_recordButton_clicked();
    //void print_all_recordsButton_clicked();
    //void find_recordButton_clicked();
    //void sort_recordsButton_clicked();

private:
    QPushButton *m_addRecord, *m_deleteRecord, *m_changeRecord, *m_printRecords, *m_findRecord, *m_sortRecords;
};

#endif // ACTION_WINDOW_H
