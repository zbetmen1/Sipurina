#include "arrow.h"
#include <QtGui>
#include <iostream>

using namespace std;

Arrow::Arrow(const QLineF &line, const QColor &color, QObject *parent) :
    QGraphicsWidget(), m_line(line), m_color(color)
{
    setParent(parent);
}

void Arrow::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QLineF line= QLineF(QPointF(0,0), m_line.p2()-m_line.p1());
    painter->setBrush(m_color);
    painter->drawLine(line);
    painter->setPen(QPen(m_color, 1.5));
    QLineF upper_arrow, lower_arrow;
    upper_arrow= QLineF(line.p2(), line.p2() + (line.p1()-line.p2())*.4);
    lower_arrow= upper_arrow;
    double angle_degrees= line.angle();
    upper_arrow.setAngle(angle_degrees+150);
    lower_arrow.setAngle(angle_degrees-150);
    painter->drawLine(upper_arrow);
    painter->drawLine(lower_arrow);

//    painter->setPen(QPen(Qt::blue, 4));
//    painter->drawPoint(0,0);
}

QRectF Arrow::boundingRect() const
{
    return QRectF(start(), end());
}
