#include "edge.h"

bool Edge::_isWeight;

void Edge::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    painter->setPen(QPen(QBrush(color()),.5));
    painter->setFont(QFont("Arial Bold", 16));
    QPointF delta(25, 25);
    QPointF center= (_start->pos()+_end->pos())/2+delta;
    QPointF control_point(center.rx()+(_end->pos().ry()-_start->pos().ry())/5, center.ry()-(_end->pos().rx()-_start->pos().rx())/5);
    QPointF weight_draw_point(center.rx()+(_end->pos().ry()-_start->pos().ry())/7, center.ry()-(_end->pos().rx()-_start->pos().rx())/7);
    if(_arcNeed){
        QPainterPath path;
        path.moveTo(_start->pos()+delta);
        path.quadTo(control_point, _end->pos()+delta);
        painter->drawPath(path);

        if(_isWeight) {

            if (_start->pos().ry() >= control_point.ry()) {
                painter->drawText(QRectF(weight_draw_point+QPointF(-10, 25),weight_draw_point+QPointF(10, 0)),Qt::AlignCenter, QString::number(_weight));
            }
            else {
                painter->drawText(QRectF(weight_draw_point+QPointF(-10, -25),weight_draw_point+QPointF(10, 0)),Qt::AlignCenter, QString::number(_weight));
            }
        }
    }

//    Undirected graph edge:
    else{
        painter->drawLine(_start->pos()+delta, _end->pos()+delta);
        if(_isWeight) {
            painter->drawText(QRectF(center+QPointF(-10, -25),center+QPointF(10, 0)),Qt::AlignCenter, QString::number(_weight));
        }
    }

//    Adding arrow if graph is directed:
    if (_directed) {
        QLineF s = QLineF(_end->pos()+QPointF(25,25),(_arcNeed)? control_point : (_start->pos()+QPointF(25,25)));
        s.setLength(25);
        QLineF l1 = QLineF(s.p2(),center);
        QLineF l2 = QLineF(s.p2(),center);
        l1.setAngle(s.angle()+25);
        l2.setAngle(s.angle()-25);
        l1.setLength(10);
        l2.setLength(10);
        painter->drawLine(l1);
        painter->drawLine(l2);
    }
}

void Edge::setGeometryEdge()
{
    setGeometry(QRectF(_start->pos(),_end->pos()));
}

Edge::Edge(Node * start, Node * end, QString name, QColor color, int value, int heuristic)
    :QGraphicsWidget(),_name(name), _color(color), _weight(value), _heuristicField(heuristic)
{
    _start = start;
    _end = end;
    _isWeight = false;
    _directed = false;
    _arcNeed = false;
}

QPointF Edge::absolute_point(QPointF p) {
    return QPointF(fabs(p.rx()), fabs(p.ry()));
}

double Edge::scalar_product(QPointF a, QPointF b) {
    return a.rx()*b.rx()+a.ry()*b.ry();
}

double Edge::norm(QPointF a) {
    return sqrt(a.rx()*a.rx()+a.ry()*a.ry());
}
