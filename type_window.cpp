#include "type_window.h"
#include <QPushButton>
#include <QVBoxLayout>

TypeWindow::TypeWindow(QWidget *parent) : QWidget(parent)
{
    QWidget::setFixedSize(QSize(240, 640));

    m_computer   = new QPushButton("Компьютер");
    m_computer -> setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_mobilePhone      = new QPushButton("Мобильный телефон");
    m_mobilePhone -> setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_tv       = new QPushButton("Телевизор");
    m_tv -> setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_toaster = new QPushButton("Тостер");
    m_toaster -> setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_coffeMaker = new QPushButton("Кофемашина");
    m_coffeMaker -> setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_elKettle = new QPushButton("Электрический чайник");
    m_elKettle -> setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_fridge = new QPushButton("Холодильник");
    m_fridge -> setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_conditioner = new QPushButton("Кондиционер");
    m_conditioner -> setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_microwave = new QPushButton("Микроволновка");
    m_microwave -> setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QVBoxLayout  *vbox = new QVBoxLayout(this);

    vbox -> addWidget(m_computer);
    vbox -> addWidget(m_mobilePhone);
    vbox -> addWidget(m_tv);
    vbox -> addWidget(m_toaster);
    vbox -> addWidget(m_coffeMaker);
    vbox -> addWidget(m_elKettle);
    vbox -> addWidget(m_fridge);
    vbox -> addWidget(m_conditioner);
    vbox -> addWidget(m_microwave);

    connect(m_computer, SIGNAL(clicked()), this, SLOT(computerButton_clicked()));
    connect(m_mobilePhone, SIGNAL(clicked()), this, SLOT(mobile_phoneButton_clicked()));
    connect(m_tv, SIGNAL(clicked()), this, SLOT(tvButton_clicked()));
    connect(m_toaster, SIGNAL(clicked()), this, SLOT(toasterButton_clicked()));
    connect(m_coffeMaker, SIGNAL(clicked()), this, SLOT(coffee_makerButton_clicked()));
    connect(m_elKettle, SIGNAL(clicked()), this, SLOT(elkettleButton_clicked()));
    connect(m_fridge, SIGNAL(clicked()), this, SLOT(fridgeButton_clicked()));
    connect(m_conditioner, SIGNAL(clicked()), this, SLOT(conditionerButton_clicked()));
    connect(m_microwave, SIGNAL(clicked()), this, SLOT(microwaveButton_clicked()));
    setLayout(vbox);
}

void TypeWindow::computerButton_clicked()
{
    setStandartColor();
    m_computer -> setStyleSheet("background-color: rgb(65, 65, 65);");
    type = "Компьютер";
    emit menu_actions_call();
}

void TypeWindow::mobile_phoneButton_clicked()
{
    setStandartColor();
    m_mobilePhone -> setStyleSheet("background-color: rgb(65, 65, 65);");
    type = "Моб.Телефон";
    emit menu_actions_call();
}

void TypeWindow::tvButton_clicked()
{
    setStandartColor();
    m_tv -> setStyleSheet("background-color: rgb(65, 65, 65);");
    type = "Телевизор";
    emit menu_actions_call();
}

void TypeWindow::toasterButton_clicked()
{
    setStandartColor();
    m_toaster -> setStyleSheet("background-color: rgb(65, 65, 65);");
    type = "Тостер";
    emit menu_actions_call();
}

void TypeWindow::coffee_makerButton_clicked()
{
    setStandartColor();
    m_coffeMaker -> setStyleSheet("background-color: rgb(65, 65, 65);");
    type = "Кофемашина";
    emit menu_actions_call();
}

void TypeWindow::elkettleButton_clicked()
{
    setStandartColor();
    m_elKettle -> setStyleSheet("background-color: rgb(65, 65, 65);");
    type = "Эл.Чайник";
    emit menu_actions_call();
}

void TypeWindow::fridgeButton_clicked()
{
    setStandartColor();
    m_fridge -> setStyleSheet("background-color: rgb(65, 65, 65);");
    type = "Холодильник";
    emit menu_actions_call();
}

void TypeWindow::conditionerButton_clicked()
{
    setStandartColor();
    m_conditioner -> setStyleSheet("background-color: rgb(65, 65, 65);");
    type = "Кондиционер";
    emit menu_actions_call();
}

void TypeWindow::microwaveButton_clicked()
{
    setStandartColor();
    m_microwave -> setStyleSheet("background-color: rgb(65, 65, 65);");
    type = "Микроволновка";
    emit menu_actions_call();
}

void TypeWindow::setStandartColor()
{
    m_computer -> setStyleSheet("background-color: rgb(90, 90, 90);");
    m_mobilePhone -> setStyleSheet("background-color: rgb(90, 90, 90);");
    m_tv -> setStyleSheet("background-color: rgb(90, 90, 90);");
    m_toaster -> setStyleSheet("background-color: rgb(90, 90, 90);");
    m_coffeMaker -> setStyleSheet("background-color: rgb(90, 90, 90);");
    m_elKettle -> setStyleSheet("background-color: rgb(90, 90, 90);");
    m_fridge -> setStyleSheet("background-color: rgb(90, 90, 90);");
    m_conditioner -> setStyleSheet("background-color: rgb(90, 90, 90);");
    m_microwave -> setStyleSheet("background-color: rgb(90, 90, 90);");
}
