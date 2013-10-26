#ifndef VISUALLISTELEMENT_H
#define VISUALLISTELEMENT_H

#include <QGraphicsWidget>
#include <QAbstractAnimation>
#include "HELPER_CLASSES/arrow.h"

class VisualListElement : public QGraphicsWidget
{
    Q_OBJECT
    Q_PROPERTY(QColor color READ color WRITE setColor)
public:
    VisualListElement(double value, QObject *parent=0, double width=60.0, double height=60.0, QColor color=Qt::red, bool is_null_pointer=false);
    void paint( QPainter * painter, const QStyleOptionGraphicsItem * option=0, QWidget * widget = 0 );
    QRectF boundingRect() const;
    virtual ~VisualListElement();

    double value() const { return m_value; }
    void setValue(double value) { m_value= value; }
    double height() const { return m_height; }
    void setHeight(double height) { m_height = height; }
    double width() const { return m_width; }
    void setWidth(double width) { m_width= width; }
    QColor color() const { return m_color; }
    void setColor(const QColor& color) { m_color= color; update(boundingRect()); }
    VisualListElement * next() const { return m_next; }
    void setNext(VisualListElement *next) { m_next= next; }
    void setArrow( Arrow *arrow) { m_arrow= arrow; }
    Arrow *arrow() const { return m_arrow; }
    bool isNull() const { return m_is_null_pointer; }
    void setNull() { m_is_null_pointer = true; }
    void setNotNull(double value) { m_is_null_pointer= false; m_value= value; }

public slots:
    void changeColor(QAbstractAnimation::State prev, QAbstractAnimation::State current);
private:
    double m_value;
    double m_width;
    double m_height;
    QColor m_color, m_previous_color;
    Arrow *m_arrow;
    VisualListElement *m_next;
    bool m_is_null_pointer;
};

#endif // VISUALLISTELEMENT_H
