#ifndef VISUALLIST_H
#define VISUALLIST_H
#include "CORE_CLASSES/animatedalgorithm.h"
#include "visuallistelement.h"
#include <list>
#include <vector>
#include <QtGui>

class QPushButton;

class VisualList : public AnimatedAlgorithm
{
    Q_OBJECT
public:
    VisualList(QObject *parent = 0, double gap=30);
    virtual ~VisualList();

    QColor color() const { return m_color; }
    void setColor(const QColor &color) { m_color= color; }
    void setElements(const std::vector<double> & vec);
    std::list<double> list() const {
        std::list<double> ret_list;
        for (auto i :m_list)
            ret_list.push_back(i->value());
        return ret_list;
    }
    double gap() const { return m_gap; }
    void setGap(double gap) { m_gap= gap; }

    void drawYourself();
    void parseInput(const QString &input);
    QHBoxLayout *returnControls();

protected:
    void operationAddFront();
    void operationAddBack();
    void operationDeleteFront();
    void operationDeleteBack();
    void operationArrowFadeInOrOut(VisualListElement *, bool, QPointF);

private slots:
    void cleanUp(QAbstractAnimation::State current, QAbstractAnimation::State prev);
    void cleanUp();
    void unblockControls();
    void fixListPosition();

    void addArrow() {
        Arrow *saver= 0;
        VisualListElement * front= m_list.front();
        front->setArrow(saver = new Arrow(QLineF(front->pos()+QPointF(front->width(), front->height()/2), front->next()->pos()+QPointF(0, front->next()->height()/2))));
        addItem(saver);
        fixListPosition();
    }

private:
//    Members:
    std::list<VisualListElement *> m_list;
    double m_gap;
    QColor m_color;
    Arrow *m_pointer_to_list_start;
    QGraphicsTextItem *m_number_text_item, *m_question_text_item, *m_warning_text_item;
    VisualListElement *m_for_deletition;
    QPushButton *m_add_front_button, *m_add_back_button, *m_delete_front_button, *m_delete_back_button;
    QLineEdit *m_line_edit;
    QHBoxLayout *m_controls;

//    Functions:
    QPointF centerList();
    void blockControls();
};

#endif // VISUALLIST_H
