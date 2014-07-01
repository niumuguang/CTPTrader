#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
//#include <help.h>

class Help;

class widget : public QWidget
{
    Q_OBJECT
public:
    explicit widget(QWidget *parent = 0);

public:
    widget(Help *helper, QWidget *parent);

//signals:

//public slots:

public slots:
    void animate();

protected:
    void paintEvent(QPaintEvent *event);

private:
    Help *help;
    int elapsed;

};

#endif // WIDGET_H
