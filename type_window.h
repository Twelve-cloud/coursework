#ifndef TYPE_WINDOW_H
#define TYPE_WINDOW_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QString>
#include <QLineEdit>
#include <QPalette>

class TypeWindow : public QWidget // окно типа техники
{
    Q_OBJECT
public:
    explicit TypeWindow(QWidget *parent = nullptr);
    QString getType() const { return  type; }
    void setStandartColor();

signals:
    void menu_actions_call();

private slots:
    void computerButton_clicked();
    void mobile_phoneButton_clicked();
    void tvButton_clicked();
    void toasterButton_clicked();
    void coffee_makerButton_clicked();
    void elkettleButton_clicked();
    void fridgeButton_clicked();
    void conditionerButton_clicked();
    void microwaveButton_clicked();

private:
    QPushButton *m_computer, *m_mobilePhone, *m_tv, *m_toaster, *m_coffeMaker, *m_elKettle, *m_fridge, *m_conditioner, *m_microwave;
    QString type;
};

#endif // TYPE_WINDOW_H
