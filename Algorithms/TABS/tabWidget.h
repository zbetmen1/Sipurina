#ifndef MOJWIDGET_H
#define MOJWIDGET_H

#include <QWidget>
#include<QVBoxLayout>
#include<QHBoxLayout>
#include<QPushButton>
#include<QString>

namespace Ui {
class tabWidget;
}

class tabWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit tabWidget(QWidget *parent = 0);
    virtual ~tabWidget();
    /* Pointers for title controls
     * its public because behavior for clicked() signal is defined in MainWindow
     */
    QPushButton * closeButton;
    QPushButton * minimizeButton;
    QPushButton * maximizeButton;
    QWidget * getSlotWidget();
    QWidget * getControlWidget();
    QWidget * getControl2Widget();
    void setTitle(QString msg);

private slots:
    /* animTabSlots */
    virtual void speedChange(int) { }
    virtual void toPlay(){ }
    virtual void toPause(){ }
    virtual void playDisable(){ }
    virtual void activeStep(int){ }
    virtual void stepDisable(){ }
    virtual void stepMethod(){ }
    /* textTabSlots */
    virtual void textSearch(QString) { }
    virtual void nextSlot() {}
    virtual void previousSlot() {}
public slots:
    virtual void resetControls(){}

private:

    Ui::tabWidget *ui;

    /* Shapes title control buttons */
    void setControls();

};

#endif // MOJWIDGET_H
