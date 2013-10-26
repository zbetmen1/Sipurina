#include "quicksort.h"
#include "DATA_STRUCTURES/visualarray.h"
#include "DATA_STRUCTURES/visualarrayelement.h"
#include <QGraphicsView>

const QColor QuickSort::m_pivotColor(Qt::blue);
const QColor QuickSort::m_pivotInPlaceColor(204,0,0);

QuickSort::QuickSort( qreal x, qreal y, qreal width, qreal height,
              QObject * parent, const vector<double> &values)
    : VisualArray(x, y, width, height, parent, values)
{
}

QuickSort::QuickSort()
    :VisualArray()
{

}

int QuickSort::partitioning(vector<double> &array, int left, int right) {
    int positionOfPivot = left;
    for (int j = left+1; j <= right; j++) {
        m_operations.push_back([=,this](){emit highlightLine(11); operationCompareElements(j, left);});
        if (array[j] < array[left]) {
            positionOfPivot++;
            m_operations.push_back([=,this](){emit highlightLine(13); operationSwapElements(j, positionOfPivot);});
            double tmp=array[j];
            array[j]=array[positionOfPivot];
            array[positionOfPivot]=tmp;
        }
    }
//    Putting pivot from left to its place:
    m_operations.push_back([=,this](){emit highlightLine(16); operationSwapElements(left, positionOfPivot);});
    if (right-left == 1) {
        m_operations.push_back([=,this](){ sortedElementColor(left); });
    }
    double tmp=array[left];
    array[left]=array[positionOfPivot];
    array[positionOfPivot]=tmp;
    return positionOfPivot;
}

void QuickSort::qsort(vector<double> &array, int left, int right) {
    if (left < right) {
        int positionOfPivot = partitioning(array, left, right);
        m_operations.push_back([=,this](){ sortedElementColor(positionOfPivot);});
        qsort(array, left, positionOfPivot - 1);
        qsort(array, positionOfPivot + 1, right);
    }
    else if (left == right) {
        m_operations.push_back([=,this](){ sortedElementColor(left);});
    }
}

void QuickSort::initializeAnimation() {
//    Set up the viewport update mode:
    views().first()->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
//    Drawing some useful text:
    m_explanationText= new QGraphicsTextItem(tr("Current pivot will be colored blue!"));
    m_explanationText->setFont(QFont("Arial", 16, -1, true));
    m_explanationText->setDefaultTextColor(QColor(30,144,255));
    this->addItem(m_explanationText);
    m_explanationText->setProperty("pos", QPointF(0, this->height()-m_explanationText->boundingRect().height()));
//    Enabling blue coloring for pivot:
    m_quickSort=true;
//    Continuing with animation:
    vector<double> varray= values();
    int n=varray.size();
    qsort(varray, 0, n-1);
    m_animationInitialized= true;
    m_currentStepInAnimation= 0;
    m_numberOfStepsInAnimation= m_operations.size();
}


