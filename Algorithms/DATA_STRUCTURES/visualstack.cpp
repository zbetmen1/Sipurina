#include "visualstack.h"
#include <QPropertyAnimation>
#include <QSequentialAnimationGroup>
#include <QParallelAnimationGroup>
#include <QPushButton>
#include "HELPER_CLASSES/cross.h"
#include <iterator>
#include <vector>
#include <sstream>

using namespace std;

VisualStack::VisualStack()
    :AnimatedAlgorithm()
{
//    Set up animation:
    m_numberOfStepsInAnimation= 0;
    m_animationInitialized= true;

//    Preparing empty stack:
    VisualArrayElement *empty_stack= new VisualArrayElement(true, 0, Qt::black);
    empty_stack->setSpecificText(QString("EMPTY"));
    m_stack.push_back(empty_stack);

//    Init controls:
    QPushButton *pop_button= m_pop = new QPushButton("P&op");
    m_pop->setStyleSheet("QPushButton{ background-color: rgb(52, 52, 52); color: white; }QPushButton:hover{ background-color: rgb(90,90,90); color: white; font-weight: bold;}");
    m_pop->setFixedWidth(150);
    QPushButton *push_button= m_push = new QPushButton("P&ush");
    m_push->setStyleSheet("QPushButton{ background-color: rgb(52, 52, 52); color: white;}QPushButton:hover{ background-color: rgb(90,90,90); color: white; font-weight: bold;}");
    m_push->setFixedWidth(150);
    QLineEdit *input= m_line_edit = new QLineEdit();
    m_line_edit->setStyleSheet("QLineEdit {background-color: rgb(102, 102, 102); color: white;}");
    input->setMaximumWidth(150);
    QRegExp regexp("-?[0-9]+");
    m_line_edit->setValidator(new QRegExpValidator(regexp));
    m_controls= new QHBoxLayout;
    m_controls->addWidget(pop_button);
    m_controls->addWidget(push_button);
    m_controls->addWidget(input);
    connect(pop_button, SIGNAL(clicked()), this, SLOT(operationPop()));
    connect(push_button, SIGNAL(clicked()), this, SLOT(operationPush()));
    connect(this, SIGNAL(sceneRectChanged(QRectF)), this, SLOT(sizeChanged(QRectF)));
}

VisualStack::~VisualStack() {
    for (auto i : m_stack)
        delete i;
    delete m_controls;
    delete m_line_edit;
    delete m_push;
    delete m_pop;
}

void VisualStack::calculateStackElementSize() {
//    Making stack that can hold 10 elements:
    const unsigned number_of_stack_elem=11;
    if (width() > m_max_width)
        m_stack_elem_width= m_max_width;
    else if (width() < m_min_width)
        m_stack_elem_width= m_min_width;
    else
        m_stack_elem_width= width();
    m_stack_elem_height= (height() / number_of_stack_elem);
}

void VisualStack::operationPush() {
    blockControls();
//    Get input:
    double value= m_line_edit->text().toDouble();
    m_line_edit->setText("");

//    Put it on the top of the screen:
    VisualArrayElement *new_element= new VisualArrayElement(false, 0, Qt::red, value, m_stack_elem_width, m_stack_elem_height);
    m_stack.push_back(new_element);
    addItem(new_element);
    new_element->setPos(m_stack_pointer.rx(), 0);

//    Animate fade in:
    QPropertyAnimation *fade_in= new QPropertyAnimation(new_element, "opacity");
    fade_in->setEasingCurve(QEasingCurve::InCubic);
    fade_in->setDuration(1500);
    fade_in->setStartValue(0);
    fade_in->setEndValue(1);

//    Animate fall down:
    QPropertyAnimation *fall_down= new QPropertyAnimation(new_element, "pos");
    fall_down->setDuration(1500);
    fall_down->setEasingCurve(QEasingCurve::InCubic);
    fall_down->setStartValue(new_element->pos());
    fall_down->setEndValue(m_stack_pointer);
    m_stack_pointer= m_stack_pointer-QPointF(0, m_stack_elem_height);

//    Running animation:
    QSequentialAnimationGroup *group= new QSequentialAnimationGroup;
    group->addAnimation(fade_in);
    group->addAnimation(fall_down);
    connect(group, SIGNAL(finished()), this, SLOT(animationFinished()));
    connect(group, SIGNAL(finished()), this, SLOT(unblockControls()));
    group->start(QAbstractAnimation::DeleteWhenStopped);
}

void VisualStack::operationPop() {
    if (m_stack.size() <= 1)
        return;
    blockControls();
//    Reset stack pointer and initialize points for lines:
    m_stack_pointer= m_stack_pointer+QPointF(0, m_stack_elem_height);
    VisualArrayElement *top_of_the_stack= m_stack.back();
    QPointF bottom_right= QPointF(m_stack_pointer.rx()+m_stack_elem_width, m_stack_pointer.ry()+m_stack_elem_height);
    QPointF bottom_left= QPointF(m_stack_pointer.rx(), m_stack_pointer.ry()+m_stack_elem_height);
    QPointF top_right= QPointF(m_stack_pointer.rx()+m_stack_elem_width, m_stack_pointer.ry());

//    Draw lines:
    QLineF ascending(m_stack_pointer, bottom_right);
    QLineF descending(top_right, bottom_left);
    Cross *cross= new Cross(ascending, descending);
    addItem(cross);

//    Fade out top of the stack:
    QParallelAnimationGroup *group =new QParallelAnimationGroup;
    QPropertyAnimation *animate_opacity_stack_el= new QPropertyAnimation(top_of_the_stack, "opacity");
    QPropertyAnimation *animate_opacity_cross= new QPropertyAnimation(cross, "opacity");

    animate_opacity_stack_el->setDuration(2000);
    animate_opacity_cross->setDuration(2000);

    animate_opacity_stack_el->setStartValue(1);
    animate_opacity_cross->setStartValue(1);

    animate_opacity_stack_el->setEndValue(0);
    animate_opacity_cross->setEndValue(0);

    group->addAnimation(animate_opacity_stack_el);
    group->addAnimation(animate_opacity_cross);

    connect(group, SIGNAL(finished()), this, SLOT(animationFinished()));
    connect(group, SIGNAL(finished()), this, SLOT(removeElement()));
    connect(group, SIGNAL(finished()), this, SLOT(unblockControls()));
    group->start(QAbstractAnimation::DeleteWhenStopped);
}

void VisualStack::sizeChanged(const QRectF &new_rect) {
    calculateStackElementSize();
    for (auto& i: m_stack) {
        i->setWidth(m_stack_elem_width);
        i->setHeight(m_stack_elem_height);
    }
}

void VisualStack::drawYourself() {
    calculateStackElementSize();
    m_stack_pointer=QPointF((width()-m_stack_elem_width)/2, height()-m_stack_elem_height);
    for (auto &i: m_stack) {
        i->setWidth(m_stack_elem_width);
        i->setHeight(m_stack_elem_height);
        i->setPos(m_stack_pointer);
        addItem(i);
        m_stack_pointer= m_stack_pointer - QPointF(0, m_stack_elem_height);
    }
}

VisualStack::VisualStack(const std::vector<double> &values)
    :AnimatedAlgorithm()
{
}

void VisualStack::removeElement() {
    removeItem(m_stack.back());
    m_stack.pop_back();
}

void VisualStack::blockControls()
{
    m_push->setDisabled(true);
    m_pop->setDisabled(true);
    m_line_edit->setDisabled(true);
}

void VisualStack::setVector(const std::vector<double> &values) {
    for (auto i: values) {
        m_stack.push_back(new VisualArrayElement(false, 0, Qt::red, i, m_stack_elem_width, m_stack_elem_height));
    }
}

void VisualStack::parseInput(const QString &input) {
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
        setVector(rvinput);
        return;
    }
    vector<double> vinput;
    istringstream sinput(input.toStdString());
    copy(istream_iterator<double>(sinput), istream_iterator<double>(), back_inserter(vinput));
    setVector(vinput);
}

QHBoxLayout *VisualStack::returnControls()
{
    return m_controls;
}


void VisualStack::unblockControls()
{
    m_push->setEnabled(true);
    m_pop->setEnabled(true);
    m_line_edit->setEnabled(true);
}
