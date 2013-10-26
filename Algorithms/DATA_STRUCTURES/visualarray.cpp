#include "visualarray.h"
#include "visualarrayelement.h"
#include "SORT/quicksort.h"
#include <QtGui>
#include <iterator>
#include <vector>
#include <sstream>
#include <random>

using namespace std;

const QColor VisualArray::m_sorted=QColor(160,0,0);

//Primary constructor:
VisualArray::VisualArray( qreal x, qreal y, qreal width, qreal height,
             QObject * parent, const vector<double> &values)
    :AnimatedAlgorithm(x,y,width, height, parent),
      m_colorOfArrayElements_i(Qt::red), m_colorOfArrayElements_j(Qt::red), m_actuallCompareTime(m_compareTime)
{
    vector<double>::const_iterator i=values.begin();
    for (; i != values.end(); i++) {
        m_arrayElements.push_back(new VisualArrayElement(false, 0, m_colorOfArrayElements_i, *i));
    }
}
//Constructor for empty array (using this only in merge sort animation):
VisualArray::VisualArray(qreal x, qreal y, qreal width, qreal height, QObject *parent, int n)
    :AnimatedAlgorithm(x,y,width,height)
{
    for (int i=0; i < n; i++)
        m_arrayElements.push_back(new VisualArrayElement(true, 0, Qt::red));
    setParent(parent);
}

VisualArray::VisualArray()
    :AnimatedAlgorithm(),m_firstAnimated(0), m_secondAnimated(0),
      m_colorOfArrayElements_i(Qt::red), m_actuallCompareTime(m_compareTime)
{

}

VisualArray::~VisualArray() {
    for (auto i: m_arrayElements)
        delete i;
}

void VisualArray::drawYourself() {
    if (items().empty() && !m_arrayElements.empty()) {//term
//      Collecting needed data:
        double sceneWidth=width();
        double sceneHeight=height();
        double margin=30;
        int numberOfElements=m_arrayElements.size();
        double singleElementWidth= (sceneWidth - margin*2)/numberOfElements;

//        Checking width of one element:
        if (singleElementWidth < m_minimalElementWidth) {
            margin=0;
            singleElementWidth=sceneWidth/numberOfElements;
        }
        if (singleElementWidth > m_maximumElementWidth) {
            margin=(sceneWidth-numberOfElements*m_maximumElementWidth)/2;
            singleElementWidth=m_maximumElementWidth;
        }
        QPointF startPoint(margin, (sceneHeight-m_arrayElements[0]->height())/2);

//      Adding visual array elements to scene:
        for (int i=0; i < numberOfElements; i++) {
            addItem(m_arrayElements[i]);
            m_arrayElements[i]->setProperty("pos", QPointF(startPoint.rx()+i*singleElementWidth,
                            startPoint.ry()));
            QPointF currPosition(m_arrayElements[i]->pos());
            m_arrayElements[i]->setWidth(singleElementWidth);
            m_arrayElements[i]->setHeight(singleElementWidth);
            m_arrayElements[i]->setGeometry(currPosition.rx(), currPosition.ry(), singleElementWidth, singleElementWidth);
        }
    }
}

void VisualArray::operationSwapElements(int i, int j) {
//    Initializing some variables for simpler notation and easier access:
    VisualArrayElement *left, *right;
    if (i <= j) {
        left=m_arrayElements[i];
        right=m_arrayElements[j];
    }
    else {
        left=m_arrayElements[j];
        right=m_arrayElements[i];
    }

//    In case where its all one element we go smart way:
    if (left == right) {
        emit ready();
        return;
    }
    QPointF startPositionLeft(left->pos());
    QPointF startPositionRight(right->pos());
    double pathLength=2*left->height()+startPositionRight.rx()-startPositionLeft.rx();
    initializeSwapTime(i,j,m_arrayElements[i]->width());
    m_firstAnimated= m_arrayElements[i];
    m_secondAnimated= m_arrayElements[j];
    m_colorOfArrayElements_i= m_arrayElements[j]->color();
    m_colorOfArrayElements_j= m_arrayElements[i]->color();
//    Setting animated elements (cause is setting original color when animation is done):
    left->setColor(Qt::green);
    right->setColor(Qt::green);

//    Animation declaration (definition):
    QPropertyAnimation *leftAnimation=new QPropertyAnimation(left, "pos");
    QPropertyAnimation *rightAnimation= new QPropertyAnimation(right, "pos");
    QParallelAnimationGroup *group=new QParallelAnimationGroup;

//    Animation functionality implementation:
    leftAnimation->setDuration(m_swapTime);
    rightAnimation->setDuration(m_swapTime);
    leftAnimation->setKeyValueAt(0, startPositionLeft);
    leftAnimation->setKeyValueAt(left->height()/pathLength,
                                 QPointF(startPositionLeft.rx(), startPositionLeft.ry()-left->height()));
    leftAnimation->setKeyValueAt((pathLength-left->height())/pathLength,
                                 QPointF(startPositionRight.rx(), startPositionLeft.ry()-left->height()));
    leftAnimation->setKeyValueAt(1,startPositionRight);
    rightAnimation->setKeyValueAt(0,startPositionRight);
    rightAnimation->setKeyValueAt(left->height()/pathLength,
                                  QPointF(startPositionRight.rx(), startPositionRight.ry()+right->height()));
    rightAnimation->setKeyValueAt((pathLength-left->height())/pathLength,
                                  QPointF(startPositionLeft.rx(), startPositionRight.ry()+right->height()));
    rightAnimation->setKeyValueAt(1,startPositionLeft);
    leftAnimation->setEasingCurve(QEasingCurve::InCubic);
    rightAnimation->setEasingCurve(QEasingCurve::InCubic);
    group->addAnimation(leftAnimation);
    group->addAnimation(rightAnimation);
    connect(group, SIGNAL(finished()), this, SLOT(regulatePairColor()));
    connect(group, SIGNAL(finished()), this, SLOT(animationFinished()));
    group->start(QAbstractAnimation::DeleteWhenStopped);

//    Swaping values in visual array member:
    VisualArrayElement*tmp=m_arrayElements[i];
    m_arrayElements[i]=m_arrayElements[j];
    m_arrayElements[j]=tmp;
}

void VisualArray::operationCompareElements(int i, int j) {
//    Change color for easier recognition of comparing elements:
    m_colorOfArrayElements_i=m_arrayElements[i]->color();
    m_colorOfArrayElements_j=m_arrayElements[j]->color();
    m_arrayElements[i]->setColor(Qt::green); //Magic color for compared elements
    m_arrayElements[j]->setColor(Qt::green);
    if (m_quickSort) {                       //Coloring the pivot blue in QuickSort class
        if (i < j)
            m_arrayElements[i]->setColor(QuickSort::pivotColor());
        else
            m_arrayElements[j]->setColor(QuickSort::pivotColor());
    }
    m_firstAnimated= m_arrayElements[i];
    m_secondAnimated= m_arrayElements[j];
//    Animation declaration (definition):
    QParallelAnimationGroup *pgroup= new QParallelAnimationGroup;
    QPropertyAnimation *animationI=new QPropertyAnimation(m_arrayElements[i], "pos");
    QPropertyAnimation *animationJ=new QPropertyAnimation(m_arrayElements[j], "pos");

//    Animation functionality implementation:
    animationI->setDuration(m_actuallCompareTime);
    animationJ->setDuration(m_actuallCompareTime);
    QPointF startPositionI=m_arrayElements[i]->pos();
    animationI->setKeyValueAt(0, startPositionI);
    animationI->setKeyValueAt(0.33, startPositionI-QPointF(0,m_arrayElements[i]->height()));
    animationI->setKeyValueAt(0.66, startPositionI-QPointF(0,m_arrayElements[i]->height()));
    animationI->setKeyValueAt(1, startPositionI);
    QPointF startPositionJ=m_arrayElements[j]->pos();
    animationJ->setKeyValueAt(0, startPositionJ);
    animationJ->setKeyValueAt(0.33, startPositionJ-QPointF(0,m_arrayElements[j]->height()));
    animationJ->setKeyValueAt(0.66, startPositionJ-QPointF(0,m_arrayElements[j]->height()));
    animationJ->setKeyValueAt(1, startPositionJ);
    animationI->setEasingCurve(QEasingCurve::InCubic);
    animationJ->setEasingCurve(QEasingCurve::InCubic);
    pgroup->addAnimation(animationI);
    pgroup->addAnimation(animationJ);
    connect(pgroup, SIGNAL(finished()), this, SLOT(regulatePairColor()));
    connect(pgroup, SIGNAL(finished()), this, SLOT(animationFinished()));
    pgroup->start(QAbstractAnimation::DeleteWhenStopped);
}

void VisualArray::initializeSwapTime(int i, int j, double width) {
//    Empiric swap time per 1 pixel in miliseconds:
    const double swapTimePerPath=5.5;
    m_swapTime=(abs(i-j)+2)*width*swapTimePerPath/speedOfAnimation();
}

void VisualArray::regulateSpeedChange() {
    m_actuallCompareTime=m_compareTime/speedOfAnimation();
}


vector<double> VisualArray::values() {
    vector<double> dvalues;
    auto i=m_arrayElements.cbegin();
    for (; i < m_arrayElements.cend(); i++)
        dvalues.push_back((*i)->value());
    return dvalues;
}

void VisualArray::initializeAnimation() {

}

void VisualArray::regulatePairColor() {
    m_firstAnimated->setColor(m_colorOfArrayElements_i);
    m_secondAnimated->setColor(m_colorOfArrayElements_i);
}

void VisualArray::sortedElementColor(int i) {
    if (isGoingForward())
        at(i)->setColor(m_sorted);
    else
        at(i)->setColor(Qt::red);
    emit ready();
}

void VisualArray::parseInput(const QString & input) {
    if (input.isEmpty())
        return;
    if (input.at(0) == QChar('r')) {
        QString str= input.mid(2);
        int array_size= str.toInt();
        vector<double> rvinput= vector<double>(array_size);
        uniform_real_distribution<double> dist(-100, 100);
        default_random_engine re;
        for (int i=0; i < array_size; i++) {
            double rounded_2_dec= dist(re);
            rounded_2_dec*=100;
            rounded_2_dec= ceil(rounded_2_dec);
            rounded_2_dec/=100;
            rvinput[i]= rounded_2_dec;
        }
        setVector(rvinput);
        return;
    }

    vector<double> vinput;
    istringstream sinput(input.toStdString());
    copy(istream_iterator<double>(sinput), istream_iterator<double>(), back_inserter(vinput));
    setVector(vinput);
}
