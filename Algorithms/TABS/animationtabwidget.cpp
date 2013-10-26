#include "animationtabwidget.h"
#include<iostream>
#include"CORE_CLASSES//animatedalgorithm.h"
#include <QGraphicsView>


animationTabWidget::animationTabWidget(tabWidget * parent)
    :tabWidget(parent), m_algorithm(nullptr)
{
    setTitle("ANIMATION");
    m_maxSpeed = .6;
    m_minSpeed = .01;
    QHBoxLayout * layoutContr = new QHBoxLayout();

    initControls();
    playPauseInd = true;
    layoutContr->setMargin(0);
    layoutContr->addStretch();
    layoutContr->addWidget(speedRegulation);
    layoutContr->addStretch();
    layoutContr->addWidget(reset);
    layoutContr->addSpacerItem(new QSpacerItem(15,0));

    layoutContr->addWidget(previous);
    layoutContr->addWidget(pause);
    layoutContr->addWidget(play);
    layoutContr->addWidget(next);

    layoutContr->addWidget(step);
    layoutContr->addStretch();
    getControlWidget()->hide();
    getControlWidget()->setLayout(layoutContr);

//    Initializing view:
    m_view= new QGraphicsView();
    const QRect viewGeometry(0,0,width(), height());
    m_view->setGeometry(viewGeometry);
    m_view->setRenderHint(QPainter::Antialiasing);
    m_view->setRenderHint(QPainter::SmoothPixmapTransform);
    m_view->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);

    getSlotWidget()->setLayout(new QVBoxLayout);
    getSlotWidget()->layout()->addWidget(m_view);
    m_view->hide();
}

animationTabWidget::~animationTabWidget()
{
    delete m_algorithm;
    delete m_view;
}

void animationTabWidget::initControls(){
    speedRegulation = new QSlider(Qt::Horizontal);
    play = new QPushButton();
    pause = new QPushButton();
    next = new QPushButton();
    previous = new QPushButton();
    reset = new QPushButton();

    speedRegulation->setRange(1,100);
    speedRegulation->setMinimumWidth(50);
    speedRegulation->setMaximumHeight(135);
    speedRegulation->setSliderPosition(m_startSpeed);
    connect(speedRegulation,SIGNAL(sliderMoved(int)),this,SLOT(speedChange(int)));


    pause->setVisible(false);
    pause->setEnabled(false);

    playPauseInd = true;

    play->setFixedSize(40,40);
    next->setFixedSize(40,40);
    previous->setFixedSize(40,40);
    pause->setFixedSize(40,40);
    reset->setFixedSize(40,40);

    next->setStyleSheet("QPushButton { background-image: url(:images/images/Next.png); border-radius: 10px;} QPushButton:hover { background-image: url(:images/images/NextPressed.png)} ");
    next->setFlat(true);
    next->setAutoFillBackground(true);

    previous->setStyleSheet("QPushButton { background-image: url(:images/images/Previous.png);border-radius: 10px;} QPushButton:hover { background-image: url(:images/images/PreviousPressed.png)} ");
    previous->setFlat(true);
    previous->setAutoFillBackground(true);

    reset->setStyleSheet("QPushButton { background-image: url(:images/images/Stop.png);border-radius: 10px;} QPushButton:hover { background-image: url(:images/images/StopPressed.png)} ");
    reset->setFlat(true);
    reset->setAutoFillBackground(true);

    pause->setStyleSheet("QPushButton { background-image: url(:images/images/Pause.png); border-radius: 10px;} QPushButton:hover { background-image: url(:images/images/PausePressed.png)} ");
    pause->setFlat(true);
    pause->setAutoFillBackground(true);

    play->setStyleSheet("QPushButton { background-image: url(:images/images/Play.png); border-radius: 10px;} QPushButton:hover { background-image: url(:images/images/PlayPressed.png)} ");
    play->setFlat(true);
    play->setAutoFillBackground(true);

    step = new QCheckBox();
    step->setText("STEP BY STEP");
    step->setStyleSheet("QCheckBox{color:white}");
}

void animationTabWidget::connectAnimatedAlgorithm() {

    connect(pause,SIGNAL(clicked()),m_algorithm,SLOT(toggleAnimationPause()));
    connect(play,SIGNAL(clicked()),m_algorithm,SLOT(startAnimation()));
    connect(play,SIGNAL(clicked()),this, SLOT(toPause()));
    connect(m_algorithm, SIGNAL(singleStepFinished()), this, SLOT(playDisable()));
    connect(pause, SIGNAL(clicked()), this, SLOT(toPlay()));
    //connect(play,SIGNAL(clicked()),this,SLOT(togglePlay()));

    connect(next,SIGNAL(clicked()),m_algorithm,SLOT(callNextStep()));
    connect(next,SIGNAL(clicked()),this,SLOT(stepDisable()));
    connect(previous,SIGNAL(clicked()),this,SLOT(stepDisable()));

    connect(m_algorithm,SIGNAL(singleStepFinished()),this,SLOT(stepMethod()));
    connect(previous,SIGNAL(clicked()),m_algorithm,SLOT(callPreviousStep()));
    connect(step,SIGNAL(stateChanged(int)),m_algorithm,SLOT(toggleExecuteStepByStep()));
    connect(step,SIGNAL(stateChanged(int)),this,SLOT(activeStep(int)));

    connect(m_algorithm, SIGNAL(endOfAnimation()), this, SLOT(resetControls()));
}

/* Under Construction! */
void animationTabWidget::resetControls(){
    play->setVisible(true);
    play->setDisabled(false);
    pause->setVisible(false);
    pause->setDisabled(true);
    step->setChecked(false);
}

void animationTabWidget::activeStep(int a){
    if(a == Qt::Checked){
        toPlay();
        play->setEnabled(false);
        previous->setDisabled(false);
        next->setDisabled(false);
    }
    if(a == Qt::Unchecked){
        play->setEnabled(true);
    }
}

void animationTabWidget::togglePlay()
{
    if(!m_algorithm->animationOnGoing() && !m_algorithm->isStepByStep())
        m_algorithm->startAnimation();
}

void animationTabWidget::playDisable()
{
    if(!m_algorithm->isStepByStep())
        play->setDisabled(false);
}

void animationTabWidget::stepDisable()
{
    previous->setDisabled(true);
    next->setDisabled(true);
}

void animationTabWidget::stepMethod()
{
    if(m_algorithm->isStepByStep()){
        previous->setDisabled(false);
        next->setDisabled(false);
    }
}

void animationTabWidget::toPause()
{
    play->setVisible(false);
    play->setEnabled(false);
    pause->setVisible(true);
    pause->setEnabled(true);
}
void animationTabWidget::toPlay()
{
    play->setVisible(true);
    play->setEnabled(false);
    pause->setVisible(false);
    pause->setEnabled(false);
}


void animationTabWidget::upControls(QHBoxLayout *foo)
{
    if(foo != 0){
        getControlWidget()->hide();
        getControl2Widget()->show();
        getControl2Widget()->setLayout(foo);
    }
    else{
        resetControls();
        getControl2Widget()->hide();
        getControlWidget()->show();

    }
}
void animationTabWidget::speedChange(int a){
    if(m_algorithm != 0){
        /* FLAG: SpeedChange
         * Additional actions to execute when slider is moved (speed change)
         */
        m_algorithm->setAnimationSpeed(m_minSpeed + (double) a * m_minSpeed/m_maxSpeed );
    }
}
void animationTabWidget::fitView(){
    if(m_algorithm != NULL){
        /* FLAG:
         *  Additional actions to execute when *this is resized
         */
        m_view->fitInView(m_algorithm->sceneRect(),Qt::KeepAspectRatio );
    }
}
void animationTabWidget::resizeEvent(QResizeEvent*){
    fitView();
}
