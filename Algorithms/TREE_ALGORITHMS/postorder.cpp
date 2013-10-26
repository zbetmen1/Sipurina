#include "postorder.h"

Postorder::Postorder(QObject *parent) :
    VisualTree()
{
    m_animationInitialized= false;
    setParent(parent);
}

void Postorder::initializeAnimation()
{
    //TODO
    m_explanationText = new QGraphicsTextItem(" ");
    obidji_ldk(getRoot());

//    Initializing variables for animation control:
    m_animationInitialized= true;
    m_currentStepInAnimation= 0;
    m_numberOfStepsInAnimation= m_operations.size();

}

void Postorder::obidji_ldk(VisualTreeElement* e){
    if(e){
        obidji_ldk(e->getLeft());
        obidji_ldk(e->getRight());

        m_operations.push_back([=, this] () { visit_node(e); });

        m_explanationText = new QGraphicsTextItem(QString(m_explanationText->toPlainText()) + QString::number(e->getValue()));
    }
}

QHBoxLayout* Postorder::returnControls()
{
    return 0;
}
