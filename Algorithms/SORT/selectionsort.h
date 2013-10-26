#ifndef SELECTIONSORT_H
#define SELECTIONSORT_H
#include "DATA_STRUCTURES/visualarray.h"

class SelectionSort : public VisualArray
{
public:
    SelectionSort( qreal x, qreal y, qreal width, qreal height,
                  QObject * parent = 0, const vector<double> &values= vector<double>());
    SelectionSort();
private:
    void initializeAnimation();
};

#endif // SELECTIONSORT_H
