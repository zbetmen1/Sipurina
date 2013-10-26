#ifndef VISUALSTACK_H
#define VISUALSTACK_H

#include "CORE_CLASSES/animatedalgorithm.h"
#include <vector>
#include "visualarrayelement.h"
#include <QPainter>
#include <QDialog>
#include <QLineEdit>

class VisualStack: public AnimatedAlgorithm
{
public:
//    Empty stack constructor:
    VisualStack();

//    Destructor:
    virtual ~VisualStack();

//    Constructor from vector of values:
    VisualStack(const std::vector<double> &values);

    double elementWidth() const { return m_stack_elem_width; }
    double elementHeight() const { return m_stack_elem_height; }
    void setVector(const std::vector<double> &values);

    void parseInput(const QString &input);
    QHBoxLayout *returnControls();

public slots:
    void drawYourself();

public slots:
    void operationPush();
    void operationPop();
private slots:
    void sizeChanged(const QRectF &);
    void removeElement();
    void blockControls();
    void unblockControls();

private:
    std::vector<VisualArrayElement *> m_stack;
    double m_stack_elem_width;
    double m_stack_elem_height;
    QPointF m_stack_pointer;
    static double constexpr m_min_width= 100.0;
    static double constexpr m_max_width= 150.0;

    QHBoxLayout * m_controls;
    QLineEdit * m_line_edit;
    QPushButton *m_push, *m_pop;

    void calculateStackElementSize();
};

#endif // VISUALSTACK_H
