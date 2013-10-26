#ifndef VISUALARRAYELEMENT_H
#define VISUALARRAYELEMENT_H

#include <QGraphicsWidget>
#include <QPointF>

class VisualArrayElement : public QGraphicsWidget
{
    Q_OBJECT
public:
    VisualArrayElement(bool empty, QObject *parent = 0, QColor color=Qt::red,
                       double value=0, double width=40, double height=40);
    virtual ~VisualArrayElement();
//    Reimplemented functions:
    void paint( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0 );
    QRectF boundingRect();
//    Getters and setters:
    inline QColor color() const { return m_color;}
    inline void setColor(const QColor &color) {m_color= color;}
    inline double value() const {return m_value;}
    inline void setValue(const double &value) {m_value=value;}
    inline double width() const {return m_width;}
    inline void setWidth(const double width) {m_width=width;}
    inline double height() const {return m_height;}
    inline void setHeight(const double height) {m_height=height;}
    inline bool isEmptyElement() const { return m_emptyElement; }
    inline void setEmptyElement(const bool empty) { m_emptyElement=empty; }
    inline void setSpecificText(const QString &text) { m_specific_text= text; }
private:
//    Members:
    double m_value;
    double m_height;
    double m_width;
    bool m_emptyElement;
    QString m_specific_text;
    static double constexpr m_fontWidthRatio= 40.0/15;
    QColor m_color;
};

#endif // VISUALARRAYELEMENT_H
