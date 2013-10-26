#ifndef NODE_H
#define NODE_H

#include<iostream>
#include<QList>
#include<QString>
#include<QGraphicsWidget>
#include<QGraphicsItem>
#include<QPainter>
#include<QMenu>
#include<QGraphicsSceneMouseEvent>
#include<QInputDialog>
#include<QMessageBox>

class Node :public QGraphicsWidget
{
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)
public:
    static bool _indName;
    static bool _indValue;
    static bool _indHeuristic;
    static bool _indHelp;
    static bool _indId;
    static bool _enabledMenu;
    static double _graphicRadius;
    static bool _disableH;
    static QColor _chosenColor;
        static void setVName(bool foo) { _indName = foo; }
        static void setVHeuristic(bool foo) { _indHeuristic = foo; }
        static void setVHelp(bool foo) { _indHelp = foo; }
        static void setVValue(bool foo) { _indValue = foo; }
        static void setVId(bool foo) { _indId = foo; }

        static bool VName() { return  _indName; }
        static bool VHeuristic() { return _indHeuristic; }
        static bool VHelp() { return _indHelp; }
        static bool VValue() { return _indValue; }
        static bool VId() { return _indId; }
public:

    friend class VisualGraph;
    Node(int value = 0, int id = 0, QString name = "Node", QColor color = Qt::blue);
    /*getters*/
    QString name() const {return _name;}
    QColor color() const {return _color;}
    int value() const {return _value;}

    void setColor(const QColor& color) { _color= color; }
    double getX(){return _graphicXCoord;}
    double getY(){return _graphicYCoord;}

    void setVisited(bool foo) { _visited = foo ; }
    bool visited() const { return _visited; }


    void setValue(int foo) { _value = foo; }
    int Heuristic(){ return _heuristicField; }

    void x(double x){ _graphicXCoord = x;}
    void y(double y){_graphicYCoord = y;}

    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *);
    void setGeometryNode();
    static void setDisableHeuristic(bool foo) {_disableH = foo;}
    QRectF boundingRect() {return QRectF(0,0,_graphicRadius,_graphicRadius);}

private slots:
    void callEntryName();
    void callEntryHeuristic();
    void callEntryHelp();
    void callEntryFID();
    void callDelete();
signals:
    void deleteN(int);

private:

    void menuSet(bool foo);
    /*attributes*/
    int _value;
    int _id;
    int _heuristicField;
    bool _visited;
    QString _name;
    QColor _color;
    int _FakeId;

    /* Graphics data */
    double _graphicXCoord;
    double _graphicYCoord;

    QWidget * style;

    /* Interaction data */
    QMenu _rightClickMenu;

    QAction * _changeValue;
    QAction * _changeName;
    QAction * _changeHeuristic;
    QAction * _changeFID;
    QAction * _deleteNode;

};

#endif // NODE_H
