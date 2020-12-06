#ifndef SORT_WIDGET_H
#define SORT_WIDGET_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>

class SortWidget : public QWidget // окно для выбора типа сортировки
{
    Q_OBJECT
public:
    explicit SortWidget(QWidget *parent = nullptr);
    QString getType() { return m_type; }

signals:
    void sort_type_clicked();
    void sort_cancel_clicked();

private slots:
    void slot_id_clicked();
    void slot_price_clicked();
    void slot_date_clicked();
    void slot_manufacturer_clicked();
    void slot_serial_clicked();
    void slot_cancel_clicked();

private:
    QString m_type;
    QLabel* m_generalLbl;
    QPushButton *m_IdBtn, *m_PriceBtn, *m_DateBtn, *m_SerialBtn, *m_ManufacturerBtn, *m_cancelBtn;
};

#endif // SORT_WIDGET_H
