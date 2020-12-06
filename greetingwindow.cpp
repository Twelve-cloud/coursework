#include "greetingwindow.h"

#include <QHBoxLayout>
#include <QLabel>

GreetingWindow::GreetingWindow(QWidget *parent) : QWidget(parent)
{
    QHBoxLayout *hbox = new QHBoxLayout(this);

    QPixmap pixmap("D:\\Qt\\Projects\\coursework\\greetings\\greeting_picture.jpg");

    QLabel *label = new QLabel(this);
    label->setPixmap(pixmap);

    hbox->addWidget(label, 0, Qt::AlignTop);
}


