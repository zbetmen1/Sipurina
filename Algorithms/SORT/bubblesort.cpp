#include "bubblesort.h"
#include "DATA_STRUCTURES/visualarray.h"

BubbleSort::BubbleSort( qreal x, qreal y, qreal width, qreal height,
              QObject * parent, const vector<double> &values)
    : VisualArray(x, y, width, height, parent, values)
{
}

BubbleSort::BubbleSort()
    :VisualArray()
{

}

void BubbleSort::initializeAnimation() {
    vector<double> varray= values();
    int n=varray.size();
    for (int i = n - 1; i > 0; i--) {
        for (int j = 0; j < i; j++) {
            m_operations.push_back([=,this](){operationCompareElements(j, j+1); emit highlightLine(12);});
            if (varray[j] > varray[j + 1]) {
                m_operations.push_back([=,this](){operationSwapElements(j, j+1); emit highlightLine(13);});
                double tmp=varray[j];
                varray[j]=varray[j+1];
                varray[j+1]=tmp;
            }
        }
        m_operations.push_back([=,this](){ sortedElementColor(i); });
    }
    m_operations.push_back([=,this](){ sortedElementColor(0); });
    m_animationInitialized= true;
    m_currentStepInAnimation= 0;
    m_numberOfStepsInAnimation= m_operations.size();
}
