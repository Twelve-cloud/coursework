#include "action_window.h"
#include <QPushButton>
#include <QVBoxLayout>

ActionWindow::ActionWindow(QWidget *parent) : QWidget(parent)
{
    QWidget::setFixedSize(QSize(240, 640));

    m_addRecord    = new QPushButton("Добавить запись");
    m_addRecord    -> setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_deleteRecord = new QPushButton("Удалить запись");
    m_deleteRecord -> setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_changeRecord = new QPushButton("Изменить запись");
    m_changeRecord -> setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_findRecord   = new QPushButton("Найти запись");
    m_findRecord   -> setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_sortRecords  = new QPushButton("Сортировать записи");
    m_sortRecords  -> setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QVBoxLayout  *vbox = new QVBoxLayout(this);

    vbox -> addWidget(m_addRecord);
    vbox -> addWidget(m_deleteRecord);
    vbox -> addWidget(m_changeRecord);
    vbox -> addWidget(m_findRecord);
    vbox -> addWidget(m_sortRecords);

    connect(m_addRecord, SIGNAL(clicked()), this, SLOT(add_recordButton_clicked()));
    connect(m_deleteRecord, SIGNAL(clicked()), this, SLOT(delete_recordButton_clicked()));
    connect(m_changeRecord, SIGNAL(clicked()), this, SLOT(change_recordButton_clicked()));
    connect(m_findRecord, SIGNAL(clicked()), this, SLOT(find_recordButton_clicked()));
    connect(m_sortRecords, SIGNAL(clicked()), this, SLOT(sort_recordsButton_clicked()));

    setLayout(vbox);
}

void ActionWindow::add_recordButton_clicked()
{
    emit add_record_clicked();
}

void ActionWindow::delete_recordButton_clicked()
{
    emit delete_record_clicked();
}

void ActionWindow::change_recordButton_clicked()
{
    emit change_record_clicked();
}

void ActionWindow::find_recordButton_clicked()
{
    emit find_record_clicked();
}

void ActionWindow::sort_recordsButton_clicked()
{
    emit sort_records_clicked();
}
