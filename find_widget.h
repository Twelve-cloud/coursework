#ifndef FIND_WIDGET_H
#define FIND_WIDGET_H

#include <QWidget>
#include <QString>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

class FindWidget : public QWidget // окно появляющееся при нажатии поиск записи
{
    Q_OBJECT
public:
    explicit FindWidget(QWidget *parent = nullptr);

    QString getID() { return m_ID; }
    void setError(const QString& error_str);
    bool isEmptyLine();
    void clearLines();

signals:
    void find_ok_clicked();
    void find_cancel_clicked();

private slots:
    void slot_ok_clicked();
    void slot_cancel_clicked();

private:
    QString m_ID;
    QPushButton *m_okBtn, *m_cancelBtn;
    QLabel* m_idLbl, * m_generalLbl;
    QLineEdit* m_idLineEdit;
};

#endif // FIND_WIDGET_H
