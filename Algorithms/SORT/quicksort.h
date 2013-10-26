#ifndef QUICKSORT_H
#define QUICKSORT_H

#include "DATA_STRUCTURES/visualarray.h"

class QuickSort : public VisualArray
{
public:
    QuickSort( qreal x, qreal y, qreal width, qreal height,
                  QObject * parent = 0, const vector<double> &values= vector<double>());
    QuickSort();
    static const QColor m_pivotColor;
    static const QColor m_pivotInPlaceColor;
    static inline QColor pivotColor() {return m_pivotColor;}
private:
    void initializeAnimation();
//    In recursion always send refrence to a variable, or it won't be changed:
    void qsort(vector<double> &array, int left, int right);
    int partitioning(vector<double> &array, int left, int right);
};

#endif // QUICKSORT_H
