#include "visualarrayelement.h"
#include <QtGui>
#include <iostream>
#include <cassert>

VisualArrayElement::VisualArrayElement(bool empty, QObject *parent, QColor color, double value, double width, double height)
    :m_specific_text("")
{
    if (! empty) {
        m_color=color;
    }
    else {
        m_color=QColor(200,200,200);
    }
    m_value=value;
    m_emptyElement=empty;
    m_width=width;
    m_height=height;
    setParent(parent);
}

VisualArrayElement::~VisualArrayElement() {
//    Will be filled if needed!
}

void VisualArrayElement::paint( QPainter * painter, const QStyleOptionGraphicsItem * option,
                                QWidget * widget )
{
    painter->setBrush(QBrush(color()));
    painter->drawRect(0,0,width(), height());
    if (!isEmptyElement()) {
        int fontSize=width()/m_fontWidthRatio;
        painter->setFont(QFont("Arial Bold", 12));
        painter->drawText(QRectF(0,0,width(), height()), Qt::AlignCenter, QString::number(value()));
    }
    else {
        setColor(Qt::black);
        painter->setBrush(QBrush(color()));
        QFont f=QFont("Arial Bold", 12);
        painter->setFont(f);
        painter->setPen(Qt::white);
        painter->drawText(QRectF(0,0,width(), height()), Qt::AlignCenter, m_specific_text);
    }
}

QRectF VisualArrayElement::boundingRect() {
    return QRectF(pos().rx(), pos().ry(), m_width, m_height);
}
