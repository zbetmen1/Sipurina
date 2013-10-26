#ifndef CROSS_H
#define CROSS_H

#include <QtGui>
#include <QGraphicsWidget>

class Cross: public QGraphicsWidget {
    Q_OBJECT
public:
    Cross(const QLineF & asc, const QLineF& desc);
//    Reimplemented functions:
    void paint( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0 );

    QRectF boundingRect();

private:
    QLineF m_ascending;
    QLineF m_descending;
};


#endif // CROSS_H
