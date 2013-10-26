#ifndef SHELLSORT_H
#define SHELLSORT_H
#include "DATA_STRUCTURES/visualarray.h"

class ShellSort : public VisualArray
{
public:
    ShellSort( qreal x, qreal y, qreal width, qreal height,
                  QObject * parent = 0, const vector<double> &values= vector<double>());
    ShellSort();
private:
    void initializeAnimation();
    void setCurrentGapText(int);
};

#endif // SHELLSORT_H
