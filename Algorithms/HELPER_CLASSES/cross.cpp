#include "cross.h"

Cross::Cross(const QLineF & asc, const QLineF& desc)
    :m_ascending(asc), m_descending(desc)
{}

void Cross::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    painter->drawLine(m_ascending);
    painter->drawLine(m_descending);
}

QRectF Cross::boundingRect() {
    return QRectF();
}

