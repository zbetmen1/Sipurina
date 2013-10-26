#ifndef INSERTIONSORT_H
#define INSERTIONSORT_H

#include "DATA_STRUCTURES/visualarray.h"

class InsertionSort : public VisualArray
{
public:
    InsertionSort( qreal x, qreal y, qreal width, qreal height,
                  QObject * parent = 0, const vector<double> &values= vector<double>());
    InsertionSort();
private:
    void initializeAnimation();
};

#endif // INSERTIONSORT_H
