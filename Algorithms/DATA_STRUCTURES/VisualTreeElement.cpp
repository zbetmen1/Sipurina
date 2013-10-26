
#include "DATA_STRUCTURES/VisualTreeElement.h"
#include <QtGui>
#include <iostream>
#include <iterator>
#include <vector>
#include <sstream>
#include <random>
#include <unistd.h>

using namespace std;

//Constructor

VisualTreeElement::VisualTreeElement()
    :m_left(nullptr), m_right(nullptr), m_parrent(nullptr), QGraphicsWidget(), m_line(nullptr), m_color(Qt::green)
{
}

VisualTreeElement::~VisualTreeElement()
{
}

VisualTreeElement::VisualTreeElement(int v)
    :m_value(v), m_left(nullptr), m_right(nullptr), m_parrent(nullptr), QGraphicsWidget(), m_line(nullptr), m_color(Qt::green)
{}

VisualTreeElement::VisualTreeElement(int v, VisualTreeElement* left, VisualTreeElement* right, VisualTreeElement* parrent)
    :m_value(v), m_left(left), m_right(right), m_parrent(parrent), QGraphicsWidget(), m_line(nullptr), m_color(Qt::green)
{}
void VisualTreeElement::setValue(int v){
    m_value = v;
}

void VisualTreeElement::setParrent(VisualTreeElement* left){
    m_parrent = left;
}

void VisualTreeElement::setGrana(int whatever)
{
    Q_UNUSED(whatever)
    m_line->setLine(QLineF(pos()+QPointF(15, 15), m_parrent->pos()+QPointF(15, 15)));
}

int VisualTreeElement::grana()
{
    return 0;
}

void VisualTreeElement::setLeft(VisualTreeElement* left){
    m_left = left;
}

void VisualTreeElement::setRight(VisualTreeElement* right){
    m_right = right;
}

int VisualTreeElement::getValue(){
    return m_value;
}

VisualTreeElement* VisualTreeElement::getParrent(){
    return m_parrent;
}

VisualTreeElement* VisualTreeElement::getLeft(){
    return m_left;
}

VisualTreeElement* VisualTreeElement::getRight(){
    return m_right;
}
QGraphicsLineItem* VisualTreeElement::getLine(){
    return m_line;
}

void VisualTreeElement::setLine(QGraphicsLineItem *l)
{
    m_line = l;
}


void VisualTreeElement::paint( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget){
    painter->setBrush(QBrush(m_color));
    painter->drawEllipse(0,0, 30, 30);
    QFont f=QFont("Arial Bold", 12);
    painter->setFont(f);
    painter->setPen(Qt::magenta);
    painter->drawText(QRectF(0,0,30,30), Qt::AlignCenter, QString::number(m_value));
}

QRectF VisualTreeElement::boundingRect()
{ return QRectF();}

//Add node
void VisualTreeElement::add_node(VisualTreeElement *t, int value, VisualTreeElement *novi){
    if(!t){
        t = new VisualTreeElement(value, nullptr, nullptr, this);
        return;
    }
    if(value < t->getValue()) {
        if(t->m_left) {
           t->m_left->add_node(t->m_left, value, novi);
        }
        else {
            t->m_left = novi;
            novi->m_parrent= t;
        }
    }
    else if(value > t->getValue()) {
        if(t->m_right) {
            t->m_right->add_node(t->m_right, value, novi);
        }
        else {
            t->m_right = novi;
            novi->m_parrent= t;
        }
    }
}

//Write elements
void VisualTreeElement::write_tree(VisualTreeElement* t){
    if(t){
        t->getLeft()->write_tree(t->getLeft());
        cout << t->getValue() << " " ;
        if(t->m_parrent)
            cout << "Roditelj: " <<t->m_parrent->m_value << endl;
        t->getRight()->write_tree(t->getRight());
    }
}

//Delete node
void VisualTreeElement::delete_node(VisualTreeElement* t, int value){
    if(t){
        if(value < t->m_value)
            delete_node(t->m_left, value);
        else if(value > t->m_value)
            delete_node(t->m_right, value);
        else{
            if(t->m_left == nullptr && t->m_right == nullptr){
                VisualTreeElement* pom = t;
                if(t->m_parrent->m_left == t)
                    t->m_parrent->m_left = nullptr;
                else
                    t->m_parrent->m_right = nullptr;
                delete pom->m_line;
                delete pom;
            }
            else if(t->m_left == nullptr) {
                VisualTreeElement* pom = t;
                if(t->m_parrent->m_left == t){
                    t->m_parrent->m_left = t->m_right;
                    t->m_right->m_parrent = t->m_parrent;
                }
                else{
                    t->m_parrent->m_right = t->m_right;
                    t->m_right->m_parrent = t->m_parrent;
                }
                delete pom->m_line;
                delete pom;
            }else if(t->m_right == nullptr){
                VisualTreeElement* pom = t;
                if(t->m_parrent->m_left == t){
                    t->m_parrent->m_left = t->m_left;
                    t->m_left->m_parrent = t->m_parrent;
                }
                else{
                    t->m_parrent->m_right = t->m_left;
                    t->m_left->m_parrent = t->m_parrent;
                }
                delete pom->m_line;
                delete pom;
            }
            else{
                //nadji minimum desnog podstabla
                VisualTreeElement* min = nadji_minimum(t->m_right, 5000);
                //zameni vrednosti
                int tmp = min->m_value;
                min->m_value = t->m_value;
                t->m_value = tmp;
                delete_node(t->m_right, value);
            }
        }
    }
}

VisualTreeElement* VisualTreeElement::nadji_minimum(VisualTreeElement* e, int min){
    VisualTreeElement* minimalni;
    while(1){
        if(e->getValue() < min){
            minimalni = e;
            min = e->m_value;
        }
        else
            e=e->m_left;
        if(!e)
            break;
    }
    return minimalni;
}
