#include "node.h"

using namespace std;

bool Node::_indName;
bool Node::_indValue;
bool Node::_indHeuristic;
bool Node::_indHelp;
bool Node::_indId;
bool Node::_enabledMenu;
bool Node::_disableH;
QColor Node::_chosenColor = Qt::blue;

double Node::_graphicRadius = 50;
Node::Node(int value,int id, QString name, QColor color)
    :QGraphicsWidget() ,_value(value), _id(id), _name(name), _color(color),_FakeId(id)

{
    _color = _chosenColor;
    setFlag(QGraphicsItem::ItemIsMovable);
    //_graphicRadius = 50;
    _value = 9999;
    _heuristicField = 0;
    style = new QWidget();
    style->setStyleSheet("QInputDialog {background-color: rgb(140, 140, 140); color: white;} QPushButton { background-color: rgb(17, 17, 17); color: white; } QSpinBox{background-color: black; color: white;} QLineEdit{backtroung-color: black; color: white;} ");
    _visited = false;
    _rightClickMenu.setStyleSheet("QMenu {background-color: rgb(140, 140, 140)}");
    _rightClickMenu.addAction(QString("Node: ") +QString::number(_id))->setEnabled(false);
    _changeFID = _rightClickMenu.addAction("Change FID");
    connect(_changeFID,SIGNAL(triggered()),this,SLOT(callEntryFID()));
    _changeName = _rightClickMenu.addAction("Change Name");
    connect(_changeName,SIGNAL(triggered()),this,SLOT(callEntryName()));
    _changeValue = _rightClickMenu.addAction("Change Value");
    connect(_changeValue,SIGNAL(triggered()),this,SLOT(callEntryHelp()));
    _changeHeuristic = _rightClickMenu.addAction("Change Heuristic");
    _changeHeuristic->setDisabled(_disableH);
    connect(_changeHeuristic,SIGNAL(triggered()),this,SLOT(callEntryHeuristic()));
    _rightClickMenu.addSeparator();
    _deleteNode = _rightClickMenu.addAction("Delete Node");
    connect(_deleteNode,SIGNAL(triggered()),this,SLOT(callDelete()));
}

void Node::menuSet(bool foo){
    _changeValue->setDisabled(foo);
    _changeName->setDisabled(foo);
    _changeHeuristic->setDisabled(foo);
    _changeFID->setDisabled(foo);
    _deleteNode->setDisabled(foo);
}

void Node::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button() == Qt::RightButton){
        event->accept();
        ungrabMouse();
        if(_enabledMenu)
            _rightClickMenu.popup(QCursor::pos());
    }

    QGraphicsWidget::mousePressEvent(event);
}



void Node::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    QRadialGradient * grad = new QRadialGradient(QPointF(_graphicRadius/2,_graphicRadius/2),_graphicRadius/2,QPointF( _graphicRadius/4, _graphicRadius/4));
    grad->setColorAt(0.9,color());
    grad->setColorAt(0.25,Qt::white);
    painter->setBrush(*(new QBrush(*grad)));
    //painter->setBrush(*(new QBrush(color())));
    painter->drawEllipse(0,0,_graphicRadius,_graphicRadius);
    painter->setFont(QFont("Arial Bold",20));
    if(Node::VId())
        painter->drawText(QRectF(0,0,_graphicRadius,_graphicRadius
                             ), Qt::AlignCenter, QString::number(_FakeId));
    painter->setFont(QFont("Arial Bold",12));

    /* Drawing distance and heuristic field of Node */
    if(Node::VHeuristic())
        painter->drawText(QRectF(_graphicRadius,0,_graphicRadius+10,_graphicRadius+10
                                 ), Qt::AlignLeft|Qt::AlignTop, QString::number(_heuristicField));
    if(Node::VHelp())
        painter->drawText(QRectF(_graphicRadius,_graphicRadius-15,_graphicRadius+10,_graphicRadius+10
                             ), Qt::AlignLeft|Qt::AlignTop, QString::number(_value));
    if(Node::VName())
        painter->drawText(QRectF(0,-16,_graphicRadius+150,_graphicRadius+150
                             ), Qt::AlignLeft|Qt::AlignTop, _name);
}

void Node::setGeometryNode()
{
    setGeometry(_graphicXCoord,_graphicYCoord,_graphicRadius,_graphicRadius);
}

void Node::callEntryName()
{
    _name = QInputDialog::getText(style,QString("Enter Name"),QString("Enter Name for for Node")+QString::number(_id));
}

void Node::callEntryHeuristic()
{
    _heuristicField = QInputDialog::getInt(style,QString("Enter Heuristic"),QString("Enter heuristic value for Node: ")+QString::number(_id));
}

void Node::callEntryHelp()
{
    _value = QInputDialog::getInt(style,QString("Enter Value"),QString("Enter help value for Node: ")+QString::number(_id));
}

void Node::callEntryFID()
{
    _FakeId = QInputDialog::getInt(style,QString("Enter FID"),QString("Enter FID value for Node: ")+QString::number(_id));
}
void Node::callDelete()
{
    emit deleteN(_id);
}
