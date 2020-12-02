#include "action_window.h"
#include <QPushButton>
#include <QVBoxLayout>

ActionWindow::ActionWindow(QWidget *parent) : QWidget(parent)
{
    QWidget::setFixedSize(QSize(240, 640));

    m_addRecord    = new QPushButton("Add record");
    m_addRecord    -> setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_deleteRecord = new QPushButton("Delete record");
    m_deleteRecord -> setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_changeRecord = new QPushButton("Change record");
    m_changeRecord -> setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_printRecords = new QPushButton("Print records");
    m_printRecords -> setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_findRecord   = new QPushButton("Find record");
    m_findRecord   -> setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_sortRecords  = new QPushButton("Sort records");
    m_sortRecords  -> setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QVBoxLayout  *vbox = new QVBoxLayout(this);

    vbox -> addWidget(m_addRecord);
    vbox -> addWidget(m_deleteRecord);
    vbox -> addWidget(m_changeRecord);
    vbox -> addWidget(m_printRecords);
    vbox -> addWidget(m_findRecord);
    vbox -> addWidget(m_sortRecords);

    setLayout(vbox);
}
