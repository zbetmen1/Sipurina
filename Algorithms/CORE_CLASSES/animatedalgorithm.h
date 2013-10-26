#ifndef ANIMATEDALGORITHM_H
#define ANIMATEDALGORITHM_H

#include <QGraphicsScene>
#include <vector>
#include <functional>
#include <QHBoxLayout>
#include <QLineEdit>

using std::vector;
using std::function;

class AnimatedAlgorithm : public QGraphicsScene
{
    Q_OBJECT
public:
//Constructor that is making the scene and initializing members required for animation control:
    AnimatedAlgorithm( qreal x, qreal y, qreal width, qreal height,
                  QObject * parent = 0);

    AnimatedAlgorithm();

//Destructor:
    virtual ~AnimatedAlgorithm();

    virtual void setRegExpValidator(QLineEdit *&random_edit, QLineEdit *&by_hand_edit);

//Sets relative speed of animation, and calls method to handle this change:
inline void setAnimationSpeed(const double speed) {m_speedOfAnimation=speed; regulateSpeedChange();}

//Simple getter for relative animation speed
inline double speedOfAnimation() const { return m_speedOfAnimation;}

//Way for knowing which way animation goes:
inline bool isGoingForward() const {return m_stepGoingForward;}

inline bool animationOnGoing() const { return !m_animationInPause; }

virtual QHBoxLayout* returnControls();

signals:
//Signal fired when animation is ready to take next action:
    void ready();
    void singleStepFinished();
    void highlightLine(int);
    void endOfAnimation();

public slots:
//Interface that provides communication with main window for control of animation:
    void startAnimation();
    inline void toggleAnimationPause() {
        m_animationInPause=!m_animationInPause;
        animationFinished();
    }

    inline bool isStepByStep() const { return m_stepByStepExec; }

    inline void toggleExecuteStepByStep() {
        m_stepByStepExec=!m_stepByStepExec;
        m_animationInPause=m_stepByStepExec;
        if (! m_stepByStepExec)
            m_stepGoingForward=true;
    }

    inline void callNextStep() { if (m_stepByStepExec) {m_stepGoingForward=true; nextStep();} }
    inline void callPreviousStep() { if (m_stepByStepExec) {m_stepGoingForward=false; nextStep();} }
    virtual void drawYourself();

public slots:
    virtual void operationPop();
    virtual void operationPush();

protected slots:
//This function is firing ready() when its ok to continue with animation (not paused etc.):
    void animationFinished();
    virtual void operationAddFront();
    virtual void operationAddBack();
    virtual void operationDeleteFront();
    virtual void operationDeleteBack();

//Performing next action determined by members that control animation:
    void nextStep();

    virtual void omotac();
    virtual void omotac_dodaj();
    virtual void omotac_brisi();
    virtual void omotac_nadji();
private slots:
    virtual void sizeChanged(const QRectF& new_rect);
    virtual void removeElement();
    virtual void unblockControls();
    virtual void dodajGranu();

private:
//List of few functions that are meant to be implemented lower in class hierarchy:
    virtual void regulateSpeedChange();//Implemented in data structure level with comment there
    virtual void initializeAnimation();//Implemented in algorithm level with comment there

//Members for controling animation actions that are needed for abstract actions like pause etc:
    bool m_animationInPause;
    bool m_stepByStepExec;
    bool m_stepGoingForward;
    double m_speedOfAnimation;

protected:
//Members for controling animation actions and behavior that inherited classes need to be aware of:
    int m_currentStepInAnimation;//Used for animation flow control
    int m_numberOfStepsInAnimation;//Used for animation flow control
    bool m_animationInitialized;//Used for control of step by step and continuos animation interaction
    bool m_quickSort;//Dirty checker for QuickSort specific operations
    QGraphicsTextItem *m_explanationText;//Used for explaning animation a bit
    vector< function<void()> > m_operations;

public:
//    Parsing input from Entry (see TABS/entry.h):
    virtual void parseInput(const QString & );
};

#endif // ANIMATEDALGORITHM_H
