#ifndef BUBBLESORT_H
#define BUBBLESORT_H

#include "DATA_STRUCTURES/visualarray.h"

class BubbleSort: public VisualArray
{
public:
    BubbleSort( qreal x, qreal y, qreal width, qreal height,
                  QObject * parent = 0, const vector<double> &values= vector<double>());
    BubbleSort();

private:
    void initializeAnimation();
};


#endif // BUBBLESORT_H
