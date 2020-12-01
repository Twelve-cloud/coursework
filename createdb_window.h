#ifndef CREATEDB_WINDOW_H
#define CREATEDB_WINDOW_H

#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QString>

class CreateDB_Window : public QWidget
{
    Q_OBJECT
public:
    explicit CreateDB_Window(QWidget *parent = nullptr);
    void changeOkName(QString btnName);
    void clearLines();
    bool checkAction();
    void setError(const QString& error_str);
    QString getName() { return m_dbName; }
    QString getPass() { return m_dbPass; }

signals:
    void send_db_settings_clicked();

private slots:
    void Ok_clicked();
    void dbNameLine_edited();
    void dbPassLine_edited();

protected:
   virtual void keyPressEvent(QKeyEvent *event);

private:
   QPushButton *m_okBtn;
   QLineEdit *m_dbNameLine, *m_dbPassLine;
   QLabel *m_dbSettingsLbl, *m_dbNameLbl, *m_dbPasswordLbl;
   QString m_dbName, m_dbPass;
};

#endif // CREATEDB_WINDOW_H
