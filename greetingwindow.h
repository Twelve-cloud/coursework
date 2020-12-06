#ifndef GREETINGWINDOW_H
#define GREETINGWINDOW_H

#include <QWidget>
#include <QPixmap>

class GreetingWindow : public QWidget
{
    Q_OBJECT

public:
    explicit GreetingWindow(QWidget *parent = nullptr);

private:
    QPixmap* m_pixmap;
};

#endif // GREETINGWINDOW_H
