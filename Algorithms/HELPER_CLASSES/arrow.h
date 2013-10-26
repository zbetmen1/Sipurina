#ifndef ARROW_H
#define ARROW_H

#include <QGraphicsWidget>

class Arrow : public QGraphicsWidget
{
    Q_OBJECT
public:
    Arrow(const QLineF& line, const QColor &color=Qt::black, QObject *parent = 0);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QRectF boundingRect() const;

    QColor color() const { return m_color; }
    void setColor(const QColor& color) { m_color= color; }
    const QPointF & start() const { return m_line.p1(); }
    const QPointF & end() const { return m_line.p2(); }
    void setStart(const QPointF &p) { m_line.setP1(p); }
    void setEnd(const QPointF &p) { m_line.setP2(p); }
    const QLineF& line() const { return m_line; }
    void setLine(const QLineF &line) { m_line= line; }
    double length() const { return m_line.length(); }

private:
    QLineF m_line;
    QColor m_color;
};

#endif // ARROW_H
