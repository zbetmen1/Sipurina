#include "insertionsort.h"
#include "DATA_STRUCTURES/visualarray.h"

InsertionSort::InsertionSort( qreal x, qreal y, qreal width, qreal height,
              QObject * parent, const vector<double> &values)
    : VisualArray(x, y, width, height, parent, values)
{
}

InsertionSort::InsertionSort()
    :VisualArray()
{

}

void InsertionSort::initializeAnimation() {
    cout << "Uso!" << endl;
    vector<double> varray= values();
    int n=varray.size();
    for (int i=1; i < n; i++) {
        for (int j=i; j > 0; j--) {
            m_operations.push_back([=,this](){operationCompareElements(j, j-1); emit highlightLine(12);});
            if (varray[j] < varray[j-1]) {
                m_operations.push_back([=,this](){operationSwapElements(j, j-1); emit highlightLine(13);});
                double tmp= varray[j];
                varray[j]=varray[j-1];
                varray[j-1]=tmp;
            }
            else {
                break;
            }
        }
    }
    m_animationInitialized= true;
    m_currentStepInAnimation= 0;
    m_numberOfStepsInAnimation= m_operations.size();
}
