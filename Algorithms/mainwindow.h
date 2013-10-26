#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <iostream>
#include <functional>
#include <QMainWindow>
#include <QtGui>

#include "DATA_STRUCTURES/visualstack.h"
#include "DATA_STRUCTURES/visualtree.h"
#include "DATA_STRUCTURES/visualgraph.h"
#include "DATA_STRUCTURES/visuallist.h"
#include "GRAPH_ALGORITHMS/dfs.h"
#include "GRAPH_ALGORITHMS/bfs.h"
#include "GRAPH_ALGORITHMS/astar.h"
#include "GRAPH_ALGORITHMS/dijsktra.h"
#include "HELPER_CLASSES/settings.h"
#include "SORT/sort.h"
#include "TABS/entry.h"
#include "TABS/animationtabwidget.h"
#include "TABS/codetabwidget.h"
#include "TABS/texttabwidget.h"
#include "SORT/sort.h"
#include "TREE_ALGORITHMS/inorder.h"
#include "TREE_ALGORITHMS/postorder.h"
#include "TREE_ALGORITHMS/preorder.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:

    void setSettings();
    void callEntry();
    void saveLastAction();

    /* Slots for "window" interaction */

    /* Imitates minimizing code window */
    void codeMinimize();
    /* Imitates maximizing code window */
    void codeMaximize(bool);
    /* Imitates closing code window */
    void codeClose();
    /* Imitates minimizing animation window */
    void animationMinimize();
    /* Imitates maximizing animation window */
    void animationMaximize(bool);
    /* Imitates closing animation window*/
    void animationClose();
    /* Imitates minimizing text window */
    void textMinimize();
    /* Imitates maximizing text window */
    void textMaximize(bool);
    /* Imitates closing text window */
    void textClose();
    /* Imitate behavior for minimized animation window  */
    void statusMaxAnimation();
    /* Imitate behavior for minimized code window  */
    void statusMaxCode();
    /* Imitate behavior for minimized text window  */
    void statusMaxText();
    /* "opening" text window */
    void menuText();
    /* "opening" animation window */
    void menuAnimation();
    /* "opening" code window */
    void menuCode();

    void entryFinished();

/* Action React slots */
    void SortQuickSort();
    void SortBubbleSort();
    void SortInsertionSort();
    void SortSelectionSort();
    void SortShellSort();

    void GraphDFS();
    void GraphBFS();
    void GraphASearch();
    void GraphDijkstra();

    void DSList();
    void DSStack();
    void DSTree();
    void DSGraph();
    void loadView();

    void TreePreOrder();
    void TreePostOrder();
    void TreeInOrder();

private slots:
    void onStop();

private:

    Ui::MainWindow *ui;
    /* Window objects */
    codeTabWidget * m_codeW;
    animationTabWidget * m_animationW;
    textTabWidget * m_textW;
    Entry * _entry;
    QAction *m_last_action;

    /* Variables for saving current states of windows when user interacts with window */
    bool animInd;
    bool codeInd;
    bool textInd;

    /* Define actions for menubar */
    void setActions();
    /* initialize window title controls */
    void setTitleControlsConnection();

void setUpView() {
    m_animationW->m_view->setScene(m_animationW->m_algorithm);
    m_animationW->connectAnimatedAlgorithm();
    connect(m_animationW->reset,SIGNAL(clicked()),this,SLOT(onStop()));
    m_animationW->m_algorithm->drawYourself();
    m_animationW->setVisible(true);
    connect(m_animationW->m_algorithm, SIGNAL(highlightLine(int)), m_codeW->m_editor, SLOT(highlightLine(int)));
    m_animationW->m_view->show();
}

template <typename T>
void setSceneProperties(T *algorithm, bool make_new_object=false) {
//    If there was object:
    if (m_animationW->m_algorithm != nullptr) {
        delete m_animationW->m_algorithm;
    }
    if (make_new_object == true)
        algorithm= new T();
    double containerWidth= m_animationW->width()-20;
    double containerHeight= m_animationW->height()-82;
    algorithm->setProperty("pos", QPointF(0,0));
    QRectF sceneRectangle(0,0,containerWidth, containerHeight);
    algorithm->setSceneRect(sceneRectangle);
    m_animationW->m_algorithm= algorithm;
    m_animationW->fitView();
    m_animationW->m_algorithm->setRegExpValidator(_entry->getRandomInputLineEdit(), _entry->getByHandInputLineEdit());
}

//This is something you don't wanna know about,
//(details http://www.qtcentre.org/threads/8392-How-to-declare-SLOT-as-a-parameter-to-member-function):
inline void connectSingleAction(QAction *action, const char *slot) {
    connect(action,SIGNAL(triggered()),this, slot);
    connect(action,SIGNAL(triggered()),this,SLOT(callEntry()));
    connect(action, SIGNAL(triggered()), this, SLOT(saveLastAction()));
}

};

#endif // MAINWINDOW_H
