#include "CORE_CLASSES/animatedalgorithm.h"
#include <QGraphicsTextItem>
#include <QGraphicsView>
#include <QRegExpValidator>
#include <iostream>

using namespace std;

AnimatedAlgorithm::AnimatedAlgorithm( qreal x, qreal y, qreal width, qreal height,
                                      QObject * parent) :
    QGraphicsScene(x,y,width, height, parent), m_animationInPause(false), m_stepByStepExec(false),
    m_stepGoingForward(true), m_currentStepInAnimation(0), m_animationInitialized(false),
    m_quickSort(false), m_explanationText(0)
{
    setAnimationSpeed(1);
    connect(this,SIGNAL(ready()), this, SLOT(nextStep()));
}

AnimatedAlgorithm::AnimatedAlgorithm()
    :QGraphicsScene(),m_animationInPause(false), m_stepByStepExec(false),
      m_stepGoingForward(true), m_currentStepInAnimation(0), m_animationInitialized(false),
      m_quickSort(false), m_explanationText(0)
{
    setAnimationSpeed(1);
    connect(this,SIGNAL(ready()), this, SLOT(nextStep()));
}

AnimatedAlgorithm::~AnimatedAlgorithm() {
    if (m_explanationText != nullptr)
        delete m_explanationText;
    m_operations.clear();
}

void AnimatedAlgorithm::setRegExpValidator(QLineEdit *&random_edit, QLineEdit *&by_hand_edit)
{
    //    Setting default validator (does't apply to graph):
        QRegExp random_regexp("[0-9]+");
        QRegExp by_hand_regexp("(-?[0-9]+[ ])+");
        random_edit->setValidator(new QRegExpValidator(random_regexp));
        by_hand_edit->setValidator(new QRegExpValidator(by_hand_regexp));
}

QHBoxLayout *AnimatedAlgorithm::returnControls()
{
    return 0;
}

void AnimatedAlgorithm::animationFinished() {
    if ( ! m_animationInPause) {
        emit ready();
    }
    emit singleStepFinished();
}

void AnimatedAlgorithm::operationAddFront()
{
}

void AnimatedAlgorithm::nextStep() {
//  Initializing not initialized animation (will be true if nextStep is called as first call from callNextStep):
    if (!m_animationInitialized)
        initializeAnimation();

    if (m_stepGoingForward) {
//        If animation isn't over perform next step forward:
        if (m_currentStepInAnimation < m_numberOfStepsInAnimation) {
            m_operations[m_currentStepInAnimation++]();
        }
//        If animation isn't interactive (there's no usual flow init, start, end etc...):
        else if (m_numberOfStepsInAnimation != 0) {
//            Reseting some members after animation is over:
            if (m_explanationText != 0)
                delete m_explanationText;
            m_explanationText= new QGraphicsTextItem("Animation finished!");
            m_explanationText->setFont(QFont("Arial", 18, 12, true));
            m_explanationText->setDefaultTextColor(Qt::black);
            this->addItem(m_explanationText);
            m_quickSort=false;
        }
    }
    else if (m_stepByStepExec && m_currentStepInAnimation != 0) {
        m_operations[--m_currentStepInAnimation]();
    }

    if (m_numberOfStepsInAnimation != 0 && m_currentStepInAnimation == m_numberOfStepsInAnimation)
        emit endOfAnimation();
}

void AnimatedAlgorithm::omotac()
{
}

void AnimatedAlgorithm::omotac_dodaj()
{
}

void AnimatedAlgorithm::omotac_brisi()
{
}

void AnimatedAlgorithm::omotac_nadji()
{
}

void AnimatedAlgorithm::startAnimation() {
    if (m_stepByStepExec)
        return;
    if (!m_animationInitialized)
        initializeAnimation();
    m_animationInPause=false;
    nextStep();
}

void AnimatedAlgorithm::initializeAnimation() {}

void AnimatedAlgorithm::regulateSpeedChange() {}

void AnimatedAlgorithm::drawYourself() {}

void AnimatedAlgorithm::sizeChanged(const QRectF&) {}

void AnimatedAlgorithm::operationPop() {}

void AnimatedAlgorithm::removeElement() {}

void AnimatedAlgorithm::unblockControls()
{
}

void AnimatedAlgorithm::dodajGranu()
{
}

void AnimatedAlgorithm::operationPush() {}

void AnimatedAlgorithm::parseInput(const QString &) {}


void AnimatedAlgorithm::operationDeleteBack()
{
}


void AnimatedAlgorithm::operationDeleteFront()
{
}


void AnimatedAlgorithm::operationAddBack()
{
}
