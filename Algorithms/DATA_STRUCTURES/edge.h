#ifndef EDGE_H
#define EDGE_H

#include<iostream>
#include<QString>
#include<QGraphicsWidget>
#include<QPainter>
#include<QGraphicsLineItem>
#include<qmath.h>
#include<QRectF>
#include<QPainterPath>
#include"node.h"

class Edge : public QGraphicsWidget
{
    Q_OBJECT
    friend class VisualGraph;
public:
/*setters*/
    void setStart(Node* start) { _start = start; }
    void setEnd(Node* end) { _end = end; }
    void setName(QString name) { _name = name; }
    void setColor(const QColor & color) { _color = color; }
    void setWeight(int value) { _weight = value;}
    void setHeuristic(int heuristic) { _heuristicField = heuristic; }

    Node* start() const { return _start; }
    Node* end() const { return _end; }
    QString name() const { return _name; }

    QColor color() const { return _color;}
    int weight() const { return _weight;}
    int heuristic() const {return _heuristicField; }

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *);
    void setGeometryEdge();

    Edge(Node* start, Node* end, QString name = QString("Untitled"), QColor color = Qt::black, int value = 0, int heuristic = 0);
    void setDirected(bool foo) { _directed = foo;}
    void setWeighted(bool ind) { _isWeight = ind; }
 static bool _isWeight;
private:

    /*Attributes*/
    Node* _start;
    Node* _end;
    QString _name;
    QColor _color;
    int _weight;
    int _id;
    int _heuristicField;
    bool _directed;

    bool _arcNeed;

    QPointF absolute_point(QPointF p);
    double scalar_product(QPointF a, QPointF b);
    double norm(QPointF a);
};


#endif // EDGE_H
