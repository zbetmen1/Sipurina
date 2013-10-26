#include "shellsort.h"
#include "DATA_STRUCTURES/visualarray.h"
#include <QGraphicsTextItem>
#include <QString>

ShellSort::ShellSort( qreal x, qreal y, qreal width, qreal height,
              QObject * parent, const vector<double> &values)
    : VisualArray(x, y, width, height, parent, values)
{
}

ShellSort::ShellSort()
    :VisualArray()
{

}


void ShellSort::initializeAnimation() {
//    Drawing some nice text to help with gap size:
    m_explanationText= new QGraphicsTextItem("Current gap size is: "+QString::number(arrayElements().size()/2));
    m_explanationText->setFont(QFont("Arial", 16, -1, true));
    m_explanationText->setProperty("pos", QPointF(0, this->height()-m_explanationText->boundingRect().height()));
    this->addItem(m_explanationText);
//    Continuing with animation initialization:
    vector<double> varray= values();
    int n=varray.size();
    for (int gap = n/2; gap > 0; gap /= 2) {
        m_operations.push_back([=, this](){ setCurrentGapText(gap);});
        for (int i = gap; i < n; i++) {
            for (int j=i-gap; j>=0; j-=gap) {
                m_operations.push_back([=,this](){operationCompareElements(j, j+gap); highlightLine(13);});
                if (varray[j] > varray[j+gap]) {
                    m_operations.push_back([=,this](){operationSwapElements(j, j+gap); highlightLine(14);});
                    double tmp=varray[j];
                    varray[j]=varray[j+gap];
                    varray[j+gap]=tmp;
                }
                else
                    break;
            }
        }
    }
    m_animationInitialized=true;
    m_currentStepInAnimation= 0;
    m_numberOfStepsInAnimation= m_operations.size();
}

void ShellSort::setCurrentGapText(int gap) {
    QString text("Current gap size is:"+QString::number(gap));
    m_explanationText->setPlainText(text);
    animationFinished();
}
