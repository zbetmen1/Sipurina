#include "visuallist.h"
#include <sstream>
#include <iterator>
#include <list>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QSequentialAnimationGroup>
#include <QPushButton>
#include <unistd.h>
#include <QPalette>
#include <QVariant>
#include <QColor>

using namespace std;

VisualList::VisualList(QObject *parent, double gap) :
    AnimatedAlgorithm(), m_gap(gap), m_pointer_to_list_start(0), m_number_text_item(0), m_question_text_item(0), m_warning_text_item(0), m_for_deletition(0)
{
    setParent(parent);

//    Fixing bug (animationFinished is called at the end of operations but this variables weren't init...):
    m_animationInitialized= true;
    m_currentStepInAnimation= 0;
    m_numberOfStepsInAnimation= 0;

//    Init controls:
    m_add_front_button = new QPushButton("&Add front");
    m_add_front_button->setStyleSheet("QPushButton{ background-color: rgb(52, 52, 52); color: white; }QPushButton:hover{ background-color: rgb(90,90,90); color: white; font-weight: bold;}");
    m_add_back_button = new QPushButton("Add &back");
    m_add_back_button->setStyleSheet("QPushButton{ background-color: rgb(52, 52, 52); color: white; }QPushButton:hover{ background-color: rgb(90,90,90); color: white; font-weight: bold;}");
    m_delete_front_button = new QPushButton("Delete &front");
    m_delete_front_button->setStyleSheet("QPushButton{ background-color: rgb(52, 52, 52); color: white; }QPushButton:hover{ background-color: rgb(90,90,90); color: white; font-weight: bold;}");
    m_delete_back_button= new QPushButton("&Delete back");
    m_delete_back_button->setStyleSheet("QPushButton{ background-color: rgb(52, 52, 52); color: white; }QPushButton:hover{ background-color: rgb(90,90,90); color: white; font-weight: bold;}");
    m_line_edit= new QLineEdit;
    QRegExp regexp("-?[0-9]+");
    m_line_edit->setValidator(new QRegExpValidator(regexp));
    m_line_edit->setMaximumWidth(100);
    m_line_edit->setStyleSheet("QLineEdit {background-color: rgb(102, 102, 102); color: white;}");
    m_controls= new QHBoxLayout;
    m_controls->addWidget(m_add_front_button);
    m_controls->addWidget(m_add_back_button);
    m_controls->addWidget(m_line_edit);
    m_controls->addWidget(m_delete_front_button);
    m_controls->addWidget(m_delete_back_button);

//    Connect controls:
    connect(m_add_front_button, SIGNAL(clicked()), this, SLOT(operationAddFront()));
    connect(m_add_back_button, SIGNAL(clicked()), this, SLOT(operationAddBack()));
    connect(m_delete_front_button, SIGNAL(clicked()), this, SLOT(operationDeleteFront()));
    connect(m_delete_back_button, SIGNAL(clicked()), this, SLOT(operationDeleteBack()));
}

VisualList::~VisualList()
{
    for (auto &i : m_list)
        delete i;
    m_list.clear();
    delete m_controls;
    delete m_add_front_button;
    delete m_add_back_button;
    delete m_delete_front_button;
    delete m_delete_back_button;
    delete m_line_edit;

}

void VisualList::setElements(const std::vector<double> &vec)
{
//    Fill the list:
    bool ind= false;
    for (auto & i : vec) {
        if (ind) {
            VisualListElement *old_back=m_list.back();

            m_list.push_back(new VisualListElement(i));
            old_back->setNext(m_list.back());
        }
        else {
            m_list.push_back(new VisualListElement(i));
            ind= true;
        }
    }

//    Add null pointer at the end:
    if (!m_list.empty()) {
        VisualListElement *old_back=m_list.back();
        VisualListElement * null_element= new VisualListElement(5);//value doesn't matter
        null_element->setNull();
        m_list.push_back(null_element);
        old_back->setNext(null_element);
    }
    else {
        VisualListElement * null_element= new VisualListElement(5);
        null_element->setNull();
        m_list.push_back(null_element);
    }
}

void VisualList::parseInput(const QString &input)
{
    if (input.isEmpty()) {
        vector<double> pinput;
        setElements(pinput);
        return;
    }
    if (input.at(0) == QChar('r')) {
        QString str= input.mid(2);
        int array_size= str.toInt();
        vector<double> rvinput= vector<double>(array_size);
        uniform_real_distribution<double> dist(-100, 100);
        default_random_engine re;
        for (int i=0; i < array_size; i++) {
            double rounded_2_dec= dist(re);
            rounded_2_dec*=100;
            rounded_2_dec= ceil(rounded_2_dec);
            rounded_2_dec/=100;
            rvinput[i]= rounded_2_dec;
        }
        setElements(rvinput);
        return;
    }

    vector<double> vinput;
    istringstream sinput(input.toStdString());
    copy(istream_iterator<double>(sinput), istream_iterator<double>(), back_inserter(vinput));
    setElements(vinput);
}

QHBoxLayout *VisualList::returnControls()
{
    return m_controls;
}

void VisualList::operationAddFront()
{
    blockControls();
    double value= m_line_edit->text().toDouble();
    m_line_edit->setText("");
//    Init new element and add it to scene:
    VisualListElement * new_element= new VisualListElement(value);
    VisualListElement * front= m_list.front();
    new_element->setWidth(front->width());
    new_element->setHeight(front->height());
    addItem(new_element);
    new_element->setPos(front->pos()-QPointF(0, 1.5*new_element->height()));

//    Make fade in animation:
    QPropertyAnimation *fade_in= new QPropertyAnimation(new_element, "opacity");
    fade_in->setDuration(1000);
    fade_in->setStartValue(0);
    fade_in->setEndValue(1);
    fade_in->setEasingCurve(QEasingCurve::InCubic);

//    Make shifting list animation:
    QParallelAnimationGroup *pgroup= new QParallelAnimationGroup;
    for (auto & i : m_list) {
        QPropertyAnimation *shift_element= new QPropertyAnimation(i, "pos");
        shift_element->setDuration(1000);
        shift_element->setStartValue(i->pos());
        shift_element->setEndValue(i->pos()+QPointF(i->width()+m_gap, 0));
        shift_element->setEasingCurve(QEasingCurve::InCubic);

        Arrow *arrow= i->arrow();
        if (arrow) {
            QPropertyAnimation *shift_arrow= new QPropertyAnimation(arrow, "pos");
            shift_arrow->setDuration(1000);
            shift_arrow->setStartValue(i->arrow()->pos());
            shift_arrow->setEndValue(arrow->pos()+QPointF(i->width()+m_gap, 0));
            shift_arrow->setEasingCurve(QEasingCurve::InCubic);
            pgroup->addAnimation(shift_arrow);
        }
        pgroup->addAnimation(shift_element);
    }
    m_list.push_front(new_element);
    new_element->setNext(front);

//    Make going down animation:
    QPropertyAnimation *go_down= new QPropertyAnimation(new_element, "pos");
    go_down->setDuration(1000);
    go_down->setStartValue(new_element->pos());
    go_down->setEndValue(new_element->pos()+QPointF(0, 1.5*new_element->height()));

//    Putting it all together:
    QSequentialAnimationGroup *sgroup= new QSequentialAnimationGroup;
    sgroup->addAnimation(fade_in);
    sgroup->addAnimation(pgroup);
    sgroup->addAnimation(go_down);
    connect(sgroup, SIGNAL(finished()), this, SLOT(addArrow()));
    connect(sgroup, SIGNAL(finished()), this, SLOT(unblockControls()));
    sgroup->start(QAbstractAnimation::DeleteWhenStopped);
}

void VisualList::operationAddBack()
{
    blockControls();
    double value= m_line_edit->text().toDouble();
    m_line_edit->setText("");
//    Call add front if the list is empty:
    if (m_list.size() == 1) {
        operationAddFront();
        return;
    }

//    Init text and number:
    QGraphicsTextItem *number= m_number_text_item = new QGraphicsTextItem(QString::number(value));
    QGraphicsTextItem *question= m_question_text_item = new QGraphicsTextItem("Is next element NULL?");
    question->setFont(QFont("Arial", -1, 2.5));
    number->setFont(QFont("Arial", -1, 2.5));
    addItem(number);
    addItem(question);
    VisualListElement *front= m_list.front();
    QPointF number_pos;
    QPointF question_pos;
    QPointF current_pos= front->pos();
    QPointF delta= QPointF(front->width()+m_gap, 0);
    number->setPos(number_pos= front->pos()+QPointF((front->width()-number->boundingRect().width())/2, -number->boundingRect().height()));
    question->setPos(question_pos= front->pos()+QPointF(-(question->boundingRect().width()-front->width())/2, front->height()));

//    Animate trough list (moving text and changing color of list elements):
    QSequentialAnimationGroup *sgroup= new QSequentialAnimationGroup;
    sgroup->addPause(1000);
    for (auto &i : m_list) {
        if (!i->next()->isNull()) {
            QParallelAnimationGroup *pgroup= new QParallelAnimationGroup;
            QPropertyAnimation *move_number= new QPropertyAnimation(number, "pos");
            QPropertyAnimation *move_question= new QPropertyAnimation(question, "pos");
            QPropertyAnimation *color_animation= new QPropertyAnimation(i, "color");

            move_number->setDuration(1000);
            move_number->setStartValue(number_pos);
            number_pos= number_pos + delta;
            move_number->setEndValue(number_pos);

            move_question->setDuration(1000);
            move_question->setStartValue(question_pos);
            question_pos= question_pos + delta;
            move_question->setEndValue(question_pos);

            color_animation->setDuration(1000);
            color_animation->setStartValue(QColor(204, 0, 0));
            color_animation->setKeyValueAt(0.5, QColor(204, 0, 0));
            color_animation->setEndValue(QColor(255, 0, 0));

            pgroup->addAnimation(move_number);
            pgroup->addAnimation(move_question);
            pgroup->addAnimation(color_animation);
            sgroup->addAnimation(pgroup);
            sgroup->addPause(1000);
            current_pos= current_pos + delta;
        }
        else {
            QPropertyAnimation *color_animation= new QPropertyAnimation(i, "color");
            color_animation->setDuration(1000);
            color_animation->setStartValue(QColor(0, 255, 0));
            color_animation->setKeyValueAt(0.5, QColor(0, 255, 0));
            color_animation->setEndValue(QColor(255, 0, 0));
            sgroup->addAnimation(color_animation);
            break;
        }
    }

//    When element whose next element is NULL is reached (move null element):
    current_pos= current_pos + delta;
    VisualListElement *null_element= m_list.back();
    QPropertyAnimation *move_null= new QPropertyAnimation(null_element, "pos");
    move_null->setDuration(1000);
    move_null->setStartValue(null_element->pos());
    move_null->setEndValue(null_element->pos()+delta);

//    Delete number and question when moving starts:
    connect(move_null, SIGNAL(stateChanged(QAbstractAnimation::State,QAbstractAnimation::State)), this, SLOT(cleanUp(QAbstractAnimation::State,QAbstractAnimation::State)));
    sgroup->addAnimation(move_null);

//    Fade in new element and his arrow (init element):
    VisualListElement *new_element= new VisualListElement(value);
    std::list<VisualListElement *>::iterator place= m_list.end();
    place--;
    m_list.insert(place, new_element);
    place--;
    place--;
    (*place)->setNext(new_element);
    new_element->setNext(null_element);
    new_element->setOpacity(0);
    addItem(new_element);
    new_element->setPos(current_pos);
    new_element->setHeight(front->height());
    new_element->setWidth(front->width());

//    Init arrow:
    Arrow *new_arrow= new Arrow(QLineF(current_pos+QPointF(new_element->width(), new_element->height()/2), current_pos+QPointF(new_element->width()+m_gap, new_element->height()/2)));//arrow
    new_element->setArrow(new_arrow);
    new_arrow->setOpacity(0);
    addItem(new_arrow);
    new_arrow->setPos(current_pos+QPointF(new_element->width(), new_element->height()/2));

//    Init animations:
    QPropertyAnimation *element_opacity= new QPropertyAnimation(new_element, "opacity");
    element_opacity->setDuration(1000);
    element_opacity->setStartValue(0);
    element_opacity->setEndValue(1);
    element_opacity->setEasingCurve(QEasingCurve::InCubic);

    QPropertyAnimation *arrow_opacity= new QPropertyAnimation(new_arrow, "opacity");
    arrow_opacity->setDuration(1000);
    arrow_opacity->setStartValue(0);
    arrow_opacity->setEndValue(1);
    arrow_opacity->setEasingCurve(QEasingCurve::InCubic);
    sgroup->addAnimation(element_opacity);
    sgroup->addAnimation(arrow_opacity);

//    Finally start damn animation:
    connect(sgroup, SIGNAL(finished()), this, SLOT(fixListPosition()));
    connect(sgroup, SIGNAL(finished()), this, SLOT(unblockControls()));
    sgroup->start(QAbstractAnimation::DeleteWhenStopped);

}

void VisualList::operationDeleteFront()
{
    blockControls();
    QSequentialAnimationGroup *sgroup=new QSequentialAnimationGroup;
    QParallelAnimationGroup *main_pgroup= new QParallelAnimationGroup;
    QGraphicsTextItem *warning;
    VisualListElement* front= m_list.front();
    QPointF delta(front->width()+m_gap, 0);
    QParallelAnimationGroup *pgroup= new QParallelAnimationGroup;
    Arrow *arrow= front->arrow();

//    If we have an empty list:
    if (m_list.size() == 1) {
        sgroup->addPause(1500);
        warning= m_warning_text_item = new QGraphicsTextItem("Cannot delete from empty list (always check that)!");
        warning->setFont(QFont("Arial", -1, 2.5));
        addItem(warning);
        warning->setPos((width()-warning->boundingRect().width())/2, warning->boundingRect().height());
        connect(sgroup, SIGNAL(finished()), this, SLOT(cleanUp()));
        connect(sgroup, SIGNAL(finished()), this, SLOT(unblockControls()));
        sgroup->start(QAbstractAnimation::DeleteWhenStopped);
        return;
    }
    m_for_deletition= front;
//    If we have only one element:
    if (m_list.size() == 2) {
//        Init variables for animation:
        VisualListElement* back= m_list.back();
        QPropertyAnimation *arrow_fade_out=new QPropertyAnimation(arrow, "opacity");
        QPropertyAnimation *element_fade_out= new QPropertyAnimation(front, "opacity");
        QPropertyAnimation *move_null= new QPropertyAnimation(back, "pos");

        arrow_fade_out->setDuration(1000);
        arrow_fade_out->setStartValue(1);
        arrow_fade_out->setEndValue(0);

        element_fade_out->setDuration(1000);
        element_fade_out->setStartValue(1);
        element_fade_out->setEndValue(0);

        move_null->setDuration(1000);
        move_null->setStartValue(back->pos());
        move_null->setEndValue(back->pos()-delta);

        pgroup->addAnimation(arrow_fade_out);
        pgroup->addAnimation(element_fade_out);
        sgroup->addAnimation(pgroup);
        sgroup->addAnimation(move_null);
        connect(sgroup, SIGNAL(finished()), this, SLOT(cleanUp()));
        connect(sgroup, SIGNAL(finished()), this, SLOT(unblockControls()));
        m_list.pop_front();
        sgroup->start(QAbstractAnimation::DeleteWhenStopped);
        return;
    }

//    List is not empty (init warning text and its animation):
    warning= m_warning_text_item = new QGraphicsTextItem("Must save pointer \n  to this element!");
    QPointF save_pointer_pos= m_list.front()->pos() + delta;
    addItem(warning);
    warning->setPos(save_pointer_pos + QPointF((front->width()-warning->boundingRect().width())/2, -warning->boundingRect().height()));
    QPropertyAnimation *position_warning= new QPropertyAnimation(warning, "pos");
    position_warning->setDuration(1500);
    position_warning->setStartValue(warning->pos());
    position_warning->setEndValue(warning->pos()-delta);

//    Color save pointer to dark red:
    QPropertyAnimation *color_save_pointer= new QPropertyAnimation(*(++m_list.begin()), "color");
    color_save_pointer->setDuration(3000);
    color_save_pointer->setStartValue(QColor(190, 0, 0));
    color_save_pointer->setEndValue(QColor(255, 0, 0));
    main_pgroup->addAnimation(color_save_pointer);

//    Real deal (fade out of first element):
    QParallelAnimationGroup *subpgroup= new QParallelAnimationGroup;
    QPropertyAnimation *arrow_fade_out=new QPropertyAnimation(arrow, "opacity");
    QPropertyAnimation *element_fade_out= new QPropertyAnimation(front, "opacity");
    arrow_fade_out->setDuration(1500);
    arrow_fade_out->setStartValue(1);
    arrow_fade_out->setEndValue(0);
    element_fade_out->setDuration(1500);
    element_fade_out->setStartValue(1);
    element_fade_out->setEndValue(0);
    subpgroup->addAnimation(arrow_fade_out);
    subpgroup->addAnimation(element_fade_out);
    sgroup->addAnimation(subpgroup);

    QPointF start= save_pointer_pos - delta;
    VisualListElement *previous= 0;
    m_list.pop_front();

//    Move whole list to left:
    for (auto & i : m_list) {
        QPropertyAnimation *shift_element= new QPropertyAnimation(i, "pos");
        shift_element->setDuration(1500);
        shift_element->setStartValue(i->pos());
        shift_element->setEndValue(start);
        start= start + QPointF(i->width() + m_gap, 0);
        shift_element->setEasingCurve(QEasingCurve::InCubic);

        if (previous) {
            Arrow *arrow= previous->arrow();
            QPropertyAnimation *shift_arrow= new QPropertyAnimation(arrow, "pos");
            shift_arrow->setDuration(1500);
            shift_arrow->setStartValue(arrow->pos());
            arrow->setLine(QLineF(start - QPointF(i->width() + m_gap, 0)+QPointF(previous->width(), previous->height()/2), start+QPointF(0, i->height()/2)));
            shift_arrow->setEndValue(start-QPointF(i->width() + 2*m_gap, -i->height()/2));
            shift_arrow->setEasingCurve(QEasingCurve::InCubic);
            pgroup->addAnimation(shift_arrow);
        }
        else {
            QPropertyAnimation *shift_arrow= new QPropertyAnimation(m_pointer_to_list_start, "pos");
            shift_arrow->setDuration(1500);
            shift_arrow->setStartValue(m_pointer_to_list_start->pos());
            QPointF arrow_start(m_gap, 0);
            m_pointer_to_list_start->setLine(QLineF(start-arrow_start-QPointF(i->width() + m_gap, 0), start-QPointF(i->width() + m_gap, 0)));
            shift_arrow->setEndValue(start-arrow_start-QPointF(i->width() + m_gap, 0));
            shift_arrow->setEasingCurve(QEasingCurve::InCubic);
            pgroup->addAnimation(shift_arrow);
        }
        pgroup->addAnimation(shift_element);
        previous= i;
    }

//    Start animation:
    pgroup->addAnimation(position_warning);
    sgroup->addAnimation(pgroup);
    main_pgroup->addAnimation(sgroup);
    connect(main_pgroup, SIGNAL(finished()), this, SLOT(cleanUp()));
    connect(main_pgroup, SIGNAL(finished()), this, SLOT(fixListPosition()));
    connect(main_pgroup, SIGNAL(finished()), this, SLOT(unblockControls()));
    main_pgroup->start(QAbstractAnimation::DeleteWhenStopped);
}

void VisualList::operationDeleteBack()
{
//    If we have an empty or single element list:
    if (m_list.size() == 1 || m_list.size() == 2) {
        operationDeleteFront();
        return;
    }

//    Iterate trough list to the element before last element:
    QGraphicsTextItem *question= m_question_text_item = new QGraphicsTextItem("Is the next element of \n next element NULL?");
    QPointF question_pos;
    VisualListElement *front= m_list.front();
    QPointF delta= QPointF(front->width()+m_gap, 0);
    QPointF current_pos= front->pos();
    question->setFont(QFont("Arial", -1, 2.5));
    addItem(question);
    question->setPos(question_pos= front->pos()+QPointF(-(question->boundingRect().width()-front->width())/2, -front->height()));
    QSequentialAnimationGroup *sgroup= new QSequentialAnimationGroup;
    sgroup->addPause(1000);

//    Loop starts:
    for (auto &i : m_list) {
        if (!i->next()->next()->isNull()) {
            QParallelAnimationGroup *pgroup= new QParallelAnimationGroup;
            QPropertyAnimation *move_question= new QPropertyAnimation(question, "pos");
            QPropertyAnimation *color_animation= new QPropertyAnimation(i, "color");

            move_question->setDuration(1000);
            move_question->setStartValue(question_pos);
            question_pos= question_pos + delta;
            move_question->setEndValue(question_pos);

            color_animation->setDuration(1000);
            color_animation->setStartValue(QColor(204, 0, 0));
            color_animation->setKeyValueAt(0.5, QColor(204, 0, 0));
            color_animation->setEndValue(QColor(255, 0, 0));

            pgroup->addAnimation(move_question);
            pgroup->addAnimation(color_animation);
            sgroup->addAnimation(pgroup);
            sgroup->addPause(1000);
            current_pos= current_pos + delta;
        }
        else {
            QPropertyAnimation *color_animation= new QPropertyAnimation(i, "color");
            color_animation->setDuration(1000);
            color_animation->setStartValue(QColor(0,255,0));
            color_animation->setKeyValueAt(0.5, QColor(0,255,0));
            color_animation->setEndValue(QColor(255,0,0));
            sgroup->addAnimation(color_animation);
            break;
        }
    }

//    Actually delete (fade out last element):
    connect(sgroup, SIGNAL(finished()), this, SLOT(cleanUp()));
    std::list<VisualListElement *>::iterator iter;
    VisualListElement* element= (*(iter = --(--m_list.end())));
    QParallelAnimationGroup *second_pgroup= new QParallelAnimationGroup;
    Arrow *arrow= element->arrow();
    VisualListElement* back= m_list.back();
    (*(--iter))->setNext(back);
    QPropertyAnimation *arrow_fade_out=new QPropertyAnimation(arrow, "opacity");
    QPropertyAnimation *element_fade_out= new QPropertyAnimation(element, "opacity");
    QPropertyAnimation *move_null= new QPropertyAnimation(back, "pos");

    arrow_fade_out->setDuration(1000);
    arrow_fade_out->setStartValue(1);
    arrow_fade_out->setEndValue(0);

    element_fade_out->setDuration(1000);
    element_fade_out->setStartValue(1);
    element_fade_out->setEndValue(0);

    move_null->setDuration(1000);
    move_null->setStartValue(back->pos());
    move_null->setEndValue(back->pos()-delta);

    second_pgroup->addAnimation(arrow_fade_out);
    second_pgroup->addAnimation(element_fade_out);
    sgroup->addAnimation(second_pgroup);
    sgroup->addAnimation(move_null);
    connect(sgroup, SIGNAL(finished()), this, SLOT(cleanUp()));
    connect(sgroup, SIGNAL(finished()), this, SLOT(fixListPosition()));
    connect(sgroup, SIGNAL(finished()), this, SLOT(unblockControls()));
    m_list.remove(element);
    sgroup->start(QAbstractAnimation::DeleteWhenStopped);
}

void VisualList::operationArrowFadeInOrOut(VisualListElement *elem, bool fade_in, QPointF show_point)
{
    Arrow *arrow= elem->arrow();
    if (fade_in)
        arrow->setPos(show_point);
    QPropertyAnimation *fade_int_or_out= new QPropertyAnimation(arrow, "opacity");
    double start= (fade_in) ? 0 : 1;
    double end= (fade_in) ? 1 : 0;
    fade_int_or_out->setDuration(1000);
    fade_int_or_out->setStartValue(start);
    fade_int_or_out->setEndValue(end);
    connect(fade_int_or_out, SIGNAL(finished()), this, SLOT(animationFinished()));
    fade_int_or_out->start(QAbstractAnimation::DeleteWhenStopped);
}

void VisualList::cleanUp(QAbstractAnimation::State current, QAbstractAnimation::State prev)
{
    if (prev == QAbstractAnimation::Stopped && current == QAbstractAnimation::Running) {
        if (m_number_text_item) {
            delete m_number_text_item;
            m_number_text_item = 0;
        }
        if (m_question_text_item) {
            delete m_question_text_item;
            m_question_text_item = 0;
        }
        if (m_warning_text_item) {
            delete m_warning_text_item;
            m_warning_text_item= 0;
        }
        if (m_for_deletition) {
            delete m_for_deletition;
            m_for_deletition = 0;
        }
    }
}

void VisualList::cleanUp()
{
    cleanUp(QAbstractAnimation::Running, QAbstractAnimation::Stopped);
}

void VisualList::unblockControls()
{
    m_add_back_button->setEnabled(true);
    m_add_front_button->setEnabled(true);
    m_delete_back_button->setEnabled(true);
    m_delete_front_button->setEnabled(true);
    m_line_edit->setEnabled(true);
}

void VisualList::fixListPosition()
{
    QPointF start= centerList();
    QParallelAnimationGroup *pgroup= new QParallelAnimationGroup;
    VisualListElement *previous= 0;
    for (auto & i : m_list) {
        QPropertyAnimation *shift_element= new QPropertyAnimation(i, "pos");
        shift_element->setDuration(1000);
        shift_element->setStartValue(i->pos());
        shift_element->setEndValue(start);
        start= start + QPointF(i->width() + m_gap, 0);
        shift_element->setEasingCurve(QEasingCurve::InCubic);

        if (previous) {
            Arrow *arrow= previous->arrow();
            QPropertyAnimation *shift_arrow= new QPropertyAnimation(arrow, "pos");
            shift_arrow->setDuration(1000);
            shift_arrow->setStartValue(arrow->pos());
            arrow->setLine(QLineF(start - QPointF(i->width() + m_gap, 0)+QPointF(previous->width(), previous->height()/2), start+QPointF(0, i->height()/2)));
            shift_arrow->setEndValue(start-QPointF(i->width() + 2*m_gap, -i->height()/2));
            shift_arrow->setEasingCurve(QEasingCurve::InCubic);
            pgroup->addAnimation(shift_arrow);
        }
        else {
            QPropertyAnimation *shift_arrow= new QPropertyAnimation(m_pointer_to_list_start, "pos");
            shift_arrow->setDuration(1000);
            shift_arrow->setStartValue(m_pointer_to_list_start->pos());
            QPointF arrow_start(m_gap, 0);
            m_pointer_to_list_start->setLine(QLineF(start-arrow_start-QPointF(i->width() + m_gap, 0), start-QPointF(i->width() + m_gap, 0)));
            shift_arrow->setEndValue(start-arrow_start-QPointF(i->width() + m_gap, 0));
            shift_arrow->setEasingCurve(QEasingCurve::InCubic);
            pgroup->addAnimation(shift_arrow);
        }
        pgroup->addAnimation(shift_element);
        previous= i;
    }

    connect(pgroup, SIGNAL(finished()), this, SLOT(animationFinished()));
    pgroup->start(QAbstractAnimation::DeleteWhenStopped);
}

QPointF VisualList::centerList()
{
    double scene_width= width();
    double scene_height= height();
    QPointF start;
    if (!m_list.empty()) {
        double single_element_height= m_list.front()->height();
        double initial_height= (scene_height-single_element_height)/2;
        unsigned number_of_elem= m_list.size();
        double single_elem_width= m_list.front()->width();
        double initial_width= number_of_elem*single_elem_width + (number_of_elem-1)*m_gap + m_gap / 2;
        if (initial_width < scene_width && single_elem_width >= 60.0) {
            start= QPointF((scene_width-initial_width)/2, initial_height);
        }
        else {
            double scale= scene_width / initial_width;
            for (auto & i : m_list) {
                i->setWidth(scale*single_elem_width);
                i->setHeight(scale*single_element_height);
            }
            m_gap*= scale;
            initial_height= (scene_height-single_element_height*scale)/2;
            start= QPointF(m_gap/2, initial_height);
        }
    }
    else {
        start= QPointF((scene_width-60 -m_gap / 2)/2, (scene_height-60)/2);
    }
    return start;
}

void VisualList::blockControls()
{
    m_add_back_button->setDisabled(true);
    m_add_front_button->setDisabled(true);
    m_delete_back_button->setDisabled(true);
    m_delete_front_button->setDisabled(true);
    m_line_edit->setDisabled(true);
}

void VisualList::drawYourself()
{
    QPointF start = centerList();
    VisualListElement *previous= 0;
    Arrow *saver=0;
    for (auto & i : m_list) {
        addItem(i);
        i->setProperty("pos", start);
        start= start + QPointF(i->width() + m_gap, 0);
        if (previous != 0) {
            previous->setArrow(saver= new Arrow(QLineF(start - QPointF(i->width() + m_gap, 0)+QPointF(previous->width(), previous->height()/2), start+QPointF(0, i->height()/2))));
            addItem(saver);
            saver->setProperty("pos", start-QPointF(i->width() + 2*m_gap, -i->height()/2));
        }
        else {
            QPointF arrow_start(m_gap, 0);
            m_pointer_to_list_start= new Arrow(QLineF(start-arrow_start-QPointF(i->width() + m_gap, 0), start-QPointF(i->width() + m_gap, 0)));
            addItem(m_pointer_to_list_start);
            m_pointer_to_list_start->setProperty("pos", start-arrow_start-QPointF(i->width() + m_gap, 0));
            QTransform transform;
            transform.translate(m_gap*.5, -m_gap*sqrt(3)/2.0);
            transform.rotate(60);
            m_pointer_to_list_start->setTransform(transform);
        }
        previous= i;
    }
}
