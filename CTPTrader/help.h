#ifndef HELP_H
#define HELP_H

#include <QPainter>
#include <QBrush>
#include <QFont>
#include <QPen>
#include <QWidget>

class Help
{
public:
    Help();


public:
    void paint(QPainter *painter, QPaintEvent *event, int elapsed);

private:
    QBrush background;
    QBrush circleBrush;
    QFont textFont;
    QPen circlePen;
    QPen textPen;
};

#endif // HELP_H
