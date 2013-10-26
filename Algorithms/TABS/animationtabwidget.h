#ifndef ANIMATIONTABWIDGET_H
#define ANIMATIONTABWIDGET_H

#include"tabWidget.h"
#include<QCheckBox>
#include<QGraphicsView>
#include<QResizeEvent>
#include<QSlider>

class AnimatedAlgorithm;

class animationTabWidget : public tabWidget
{
public:
    animationTabWidget(tabWidget *parent = 0);
    ~animationTabWidget();
    QSlider * speedRegulation;
    QPushButton * play;
    QPushButton * pause;
    QPushButton * next;
    QPushButton * previous;
    QCheckBox * step;
    QPushButton * reset;
    friend class MainWindow;
    void fitView();
private slots:
    void speedChange(int a);
    /* Start Under Construction! */
    void resetControls();
    void activeStep(int);
    void togglePlay();
    void playDisable();
    void stepDisable();
    void stepMethod();
    void toPlay();
    void toPause();
    /* End Under Construction! */

private:
    bool indL;
    void upControls(QHBoxLayout * foo);
    void resizeEvent(QResizeEvent * );
    bool playPauseInd;
    QGraphicsView * m_view;
    AnimatedAlgorithm * m_algorithm;
    QWidget * m_center;
    void connectAnimatedAlgorithm();
    void initControls();
    static int constexpr m_startSpeed= 50;
    double m_maxSpeed;
    double m_minSpeed;
};

#endif // ANIMATIONTABWIDGET_H
