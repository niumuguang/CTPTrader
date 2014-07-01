#include "widget.h"

#include <QPainter>
#include <QTimer>
#include <help.h>

widget::widget(QWidget *parent) :
    QWidget(parent)
{
}

widget::widget(Help *help, QWidget *parent):QWidget(parent), help(help)
{
    elapsed = 0;
    setFixedSize(200, 200);
}

void widget::animate()
{
    elapsed = (elapsed + qobject_cast<QTimer*>(sender())->interval()) % 1000;
    repaint();
}

void widget::paintEvent(QPaintEvent *event)
{
    QPainter painter;
    painter.begin(this);
    painter.setRenderHint(QPainter::Antialiasing);
    help->paint(&painter, event, elapsed);
    painter.end();
}
