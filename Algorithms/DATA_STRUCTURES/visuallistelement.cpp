#include "visuallistelement.h"
#include <QtGui>
#include <QLineF>

VisualListElement::VisualListElement(double value, QObject *parent, double width, double height, QColor color, bool is_null_pointer)
    :m_value(value), QGraphicsWidget(), m_width(width), m_height(height), m_color(color), m_previous_color(color), m_next(0), m_arrow(0), m_is_null_pointer(is_null_pointer)
{
    setParent(parent);
}

void VisualListElement::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    if (! m_is_null_pointer) {
        painter->setBrush(m_color);
        painter->drawRect(0, 0, m_width, m_height);
        painter->setBrush(Qt::black);
        painter->drawText(QRectF(0,0,m_width, m_height), Qt::AlignCenter, QString::number(m_value));
    }
    else {
        painter->setBrush(Qt::black);
        painter->drawRect(0, 0, m_width, m_height);
        painter->setPen(QPen(Qt::white, 2));
        painter->drawText(QRectF(0,0,m_width, m_height), Qt::AlignCenter, QString("NULL"));
    }
}

QRectF VisualListElement::boundingRect() const
{
    return QRectF(0, 0, m_width, m_height);
}

VisualListElement::~VisualListElement()
{
}

void VisualListElement::changeColor(QAbstractAnimation::State prev, QAbstractAnimation::State current)
{
    m_previous_color= m_color;
    if (prev == QAbstractAnimation::Stopped && current == QAbstractAnimation::Running) {
        m_color= (next() && next()->isNull()) ? Qt::green : QColor(204, 0, 0);
    }
    else if (current == QAbstractAnimation::Stopped && prev == QAbstractAnimation::Running)
        m_color= m_previous_color;
}
