#ifndef VISUALARRAY_H
#define VISUALARRAY_H

#include <QGraphicsScene>
#include <QGraphicsTextItem>
#include <vector>
#include <iostream>
#include "CORE_CLASSES/animatedalgorithm.h"
#include "DATA_STRUCTURES/visualarrayelement.h"

using namespace std;

class VisualArrayElement;

class VisualArray : public AnimatedAlgorithm
{
    Q_OBJECT

public:
//Constructor that initializes m_arrayElements with provided values:
    VisualArray( qreal x, qreal y, qreal width, qreal height,
                  QObject * parent = 0, const vector<double> &values= vector<double>());

//Constructor for empty array, planed for MergeSort class as helper array:
    VisualArray(qreal x, qreal y, qreal width, qreal height, QObject *parent, int n);

    VisualArray();

    ~VisualArray();

//Some getters and setters for private members:
    inline vector<VisualArrayElement*> arrayElements() const {return m_arrayElements;}
    inline void setVector(const vector<VisualArrayElement*> v) {m_arrayElements=v;}
    inline VisualArrayElement* & at(int i) { return m_arrayElements[i];}
    vector<double> values();//Returns values, as vector of doubles, contained in m_arrayElements elements
    inline void setVector(const vector<double> v) {
        for (auto i=v.cbegin(); i != v.cend(); i++)
            m_arrayElements.push_back(new VisualArrayElement(false, 0, Qt::red, *i));
    }

signals:
    void binaryOperationFinished(int, int);

protected slots:
//Operations on array:
    void operationSwapElements(int , int );
    void operationCompareElements(int , int );
    void regulatePairColor();

public slots:
    void drawYourself();

public:
    void parseInput(const QString &input);

private:
//Inherited function explained before:
    void regulateSpeedChange();
    void initializeAnimation();

//Sets duration of swap operations on array elements as function of distance between them:
    void initializeSwapTime(int i, int j, double width);

//Some constants that are determined empirically:
    static int const m_compareTime=1000;
    static double constexpr m_minimalElementWidth=28.3333;
    static double constexpr m_swapTimePerPath=5.5;
    static int const m_maximumElementWidth=62;

//Members storing and coloring elements of array that we see on the screen:
    vector<VisualArrayElement*> m_arrayElements;
    QColor m_colorOfArrayElements_i;
    QColor m_colorOfArrayElements_j;
    VisualArrayElement* m_firstAnimated;
    VisualArrayElement* m_secondAnimated;

//Operations timers:
    double m_swapTime;
    int m_actuallCompareTime;
 protected:
    void returnOriginalColorToPair(int, int);
    void sortedElementColor(int i);
    static const QColor m_sorted;
};

#endif // VISUALARRAY_H
