#include "sort_widget.h"
#include <QVBoxLayout>

SortWidget::SortWidget(QWidget *parent) : QWidget(parent)
{
    QWidget::setFixedSize(QSize(360, 480));
    setWindowTitle("Варианты сортировки");

    m_IdBtn = new QPushButton("ИД");
    m_IdBtn -> setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    m_PriceBtn = new QPushButton("Цена");
    m_PriceBtn -> setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    m_DateBtn = new QPushButton("Дата");
    m_DateBtn -> setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    m_SerialBtn = new QPushButton("Серия");
    m_SerialBtn -> setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    m_ManufacturerBtn = new QPushButton("Производитель");
    m_ManufacturerBtn -> setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    m_cancelBtn = new QPushButton("Назад");
    m_cancelBtn -> setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QVBoxLayout *main_layout = new QVBoxLayout;

    main_layout -> addWidget(m_IdBtn);
    main_layout -> addWidget(m_PriceBtn);
    main_layout -> addWidget(m_DateBtn);
    main_layout -> addWidget(m_SerialBtn);
    main_layout -> addWidget(m_ManufacturerBtn);
    main_layout -> addWidget(m_cancelBtn);
    main_layout -> setContentsMargins(40, 40, 40, 40);

    connect(m_IdBtn, SIGNAL(clicked()), this, SLOT(slot_id_clicked()));
    connect(m_PriceBtn, SIGNAL(clicked()), this, SLOT(slot_price_clicked()));
    connect(m_DateBtn, SIGNAL(clicked()), this, SLOT(slot_date_clicked()));
    connect(m_SerialBtn, SIGNAL(clicked()), this, SLOT(slot_serial_clicked()));
    connect(m_ManufacturerBtn, SIGNAL(clicked()), this, SLOT(slot_manufacturer_clicked()));
    connect(m_cancelBtn, SIGNAL(clicked()), this, SLOT(slot_cancel_clicked()));
    setLayout(main_layout);
}

void SortWidget::slot_id_clicked()
{
    m_type = m_IdBtn -> text();
    emit sort_type_clicked();
}

void SortWidget::slot_price_clicked()
{
    m_type = m_PriceBtn -> text();
    emit sort_type_clicked();
}

void SortWidget::slot_date_clicked()
{
    m_type = m_DateBtn -> text();
    emit sort_type_clicked();
}

void SortWidget::slot_serial_clicked()
{
    m_type = m_SerialBtn -> text();
    emit sort_type_clicked();
}

void SortWidget::slot_manufacturer_clicked()
{
    m_type = m_ManufacturerBtn -> text();
    emit sort_type_clicked();
}

void SortWidget::slot_cancel_clicked()
{
    emit sort_cancel_clicked();
}
