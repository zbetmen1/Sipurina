#include "inorder.h"

Inorder::Inorder(QObject *parent) :
    VisualTree()
{
    setParent(parent);
    m_animationInitialized= false;
}

void Inorder::initializeAnimation()
{
    m_explanationText = new QGraphicsTextItem(" ");
    obidji_lkd(getRoot());

//    Initializing variables for animation control:
    m_animationInitialized= true;
    m_currentStepInAnimation= 0;
    m_numberOfStepsInAnimation= m_operations.size();
}

void Inorder::obidji_lkd(VisualTreeElement* e){
    if(e){
        obidji_lkd(e->getLeft());
        m_operations.push_back([=, this] () { visit_node(e); });

        m_explanationText = new QGraphicsTextItem(QString(m_explanationText->toPlainText()) + QString::number(e->getValue()));

        obidji_lkd(e->getRight());
}
}

QHBoxLayout* Inorder::returnControls()
{
    return 0;
}
