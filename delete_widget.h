#ifndef DELETE_WIDGET_H
#define DELETE_WIDGET_H

#include <QWidget>
#include <QString>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

class DeleteWidget : public QWidget
{
    Q_OBJECT
public:
    explicit DeleteWidget(QWidget *parent = nullptr);
    QString getID() { return m_ID; }
    void setError(const QString& error_str);
    bool isEmptyLine();
    void clearLines();

signals:
    void delete_ok_clicked();
    void delete_cancel_clicked();

private slots:
    void slot_ok_clicked();
    void slot_cancel_clicked();

private:
    QString m_ID;
    QPushButton *m_okBtn, *m_cancelBtn;
    QLabel* m_idLbl, * m_generalLbl;
    QLineEdit* m_idLineEdit;
};

#endif // DELETE_WIDGET_H
