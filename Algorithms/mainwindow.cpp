#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow), m_last_action(0)
{
    ui->setupUi(this);

    setActions();

    animInd = true;
    textInd = true;
    codeInd = true;

    m_codeW = new codeTabWidget();
    m_animationW = new animationTabWidget();
    m_textW = new textTabWidget();
    _entry = new Entry();

    setTitleControlsConnection();
    ui->statusAnimationMaximize->setVisible(false);
    ui->statusCodeMaximize->setVisible(false);
    ui->statusTextMaximize->setVisible(false);

    ui->main_animacija->setLayout(new QVBoxLayout);
    ui->main_kod->setLayout(new QVBoxLayout);
    ui->main_tekst->setLayout(new QVBoxLayout);

    ui->main_animacija->layout()->addWidget(m_animationW);
    m_animationW->setVisible(true);
    ui->main_animacija->layout()->addWidget(_entry);
    _entry->setVisible(false);
    ui->main_kod->layout()->addWidget(m_codeW);
    ui->main_tekst->layout()->addWidget(m_textW);

    ui->verticalLayout->setMargin(0);

    ui->splitter_2->setStretchFactor(1,2);
    ui->splitter_2->setStretchFactor(0,3);

    ui->splitter->setStretchFactor(1,2);
    ui->splitter->setStretchFactor(0,12);

    connect(_entry,SIGNAL(entryDone()),this,SLOT(entryFinished()));
    connect(_entry, SIGNAL(entryDone()), this, SLOT(loadView()));
    showMaximized();

}

void MainWindow::entryFinished(){
    _entry->setVisible(false);
    m_animationW->setVisible(true);
}
void MainWindow::setTitleControlsConnection(){
    connect(ui->statusAnimationMaximize,SIGNAL(clicked()),this,SLOT(statusMaxAnimation()));
    connect(ui->statusCodeMaximize,SIGNAL(clicked()),this,SLOT(statusMaxCode()));
    connect(ui->statusTextMaximize,SIGNAL(clicked()),this,SLOT(statusMaxText()));

    connect(m_codeW->closeButton,SIGNAL(clicked()),this,SLOT(codeClose()));
    connect(m_codeW->minimizeButton,SIGNAL(clicked()),this,SLOT(codeMinimize()));
    connect(m_codeW->maximizeButton,SIGNAL(toggled(bool)),this,SLOT(codeMaximize(bool)));

    connect(m_animationW->closeButton,SIGNAL(clicked()),this,SLOT(animationClose()));
    connect(m_animationW->minimizeButton,SIGNAL(clicked()),this,SLOT(animationMinimize()));
    connect(m_animationW->maximizeButton,SIGNAL(toggled(bool)),this,SLOT(animationMaximize(bool)));

    connect(m_textW->closeButton,SIGNAL(clicked()),this,SLOT(textClose()));
    connect(m_textW->minimizeButton,SIGNAL(clicked()),this,SLOT(textMinimize()));
    connect(m_textW->maximizeButton,SIGNAL(toggled(bool)),this,SLOT(textMaximize(bool)));
}
void MainWindow::setActions(){
    connect(ui->actionAnimation,SIGNAL(triggered()),this,SLOT(menuAnimation()));
    connect(ui->actionCode,SIGNAL(triggered()),this,SLOT(menuCode()));
    connect(ui->actionText,SIGNAL(triggered()),this,SLOT(menuText()));

    connect(ui->actionGraph_Look_Settings,SIGNAL(triggered()),this,SLOT(setSettings()));

    connectSingleAction(ui->actionBubble_Sort, SLOT(SortBubbleSort()));
    connectSingleAction(ui->actionInsertion_Sort, SLOT(SortInsertionSort()));
    connectSingleAction(ui->actionSelection_Sort, SLOT(SortSelectionSort()));
    connectSingleAction(ui->actionQuick_Sort, SLOT(SortQuickSort()));
    connectSingleAction(ui->actionShell_Sort, SLOT(SortShellSort()));
    connectSingleAction(ui->actionStack,SLOT(DSStack()));
    connectSingleAction(ui->actionTree,SLOT(DSTree()));
    connectSingleAction(ui->actionGraph, SLOT(DSGraph()));
    connectSingleAction(ui->actionList, SLOT(DSList()));
    connectSingleAction(ui->actionGraphDFS,SLOT(GraphDFS()));
    connectSingleAction(ui->actionGraphBFS,SLOT(GraphBFS()));
    connectSingleAction(ui->actionA_Search,SLOT(GraphASearch()));
    connectSingleAction(ui->actionDijkstra,SLOT(GraphDijkstra()));
    connectSingleAction(ui->actionPost_Order,SLOT(TreePostOrder()));
    connectSingleAction(ui->actionPre_Order_traverse,SLOT(TreePreOrder()));
    connectSingleAction(ui->actionIn_Order,SLOT(TreeInOrder()));
}
void MainWindow::callEntry(){
    _entry->setVisible(true);
    m_animationW->setVisible(false);
    m_animationW->fitView();
}

void MainWindow::saveLastAction()
{
    m_last_action= static_cast<QAction *>(sender());
}

void MainWindow::menuAnimation(){
    if(!ui->main_animacija->isVisible() && !ui->statusAnimationMaximize->isVisible())
        ui->main_animacija->setVisible(true);
}
void MainWindow::menuCode(){
    if(!ui->main_kod->isVisible() && !ui->statusCodeMaximize->isVisible() )
        ui->main_kod->setVisible(true);
}
void MainWindow::menuText(){
    if(!ui->main_tekst->isVisible() && !ui->statusTextMaximize->isVisible() )
        ui->main_tekst->setVisible(true);
}
void MainWindow::statusMaxAnimation(){
    ui->main_animacija->setVisible(true);
    ui->statusAnimationMaximize->setVisible(false);
}
void MainWindow::statusMaxCode(){
    ui->main_kod->setVisible(true);
    ui->statusCodeMaximize->setVisible(false);
}
void MainWindow::statusMaxText(){
    ui->main_tekst->setVisible(true);
    ui->statusTextMaximize->setVisible(false);
}
void MainWindow::codeMinimize(){
    ui->main_animacija->setVisible(animInd);
    ui->main_tekst->setVisible(textInd);
    codeInd = false;
    ui->statusCodeMaximize->setVisible(true);
    ui->main_kod->setVisible(false);
}
void MainWindow::codeMaximize(bool a){
    if(a == true){
        animInd = ui->main_animacija->isVisible();
        textInd = ui->main_tekst->isVisible();
        ui->main_animacija->setVisible(!a);
        ui->main_tekst->setVisible(!a);
    }
    else{
        ui->main_animacija->setVisible(animInd);
        ui->main_tekst->setVisible(textInd);
    }
}
void MainWindow::codeClose(){
    ui->main_kod->setVisible(false);
}
void MainWindow::textClose(){
    ui->main_tekst->setVisible(false);
}
void MainWindow::textMinimize(){
    ui->main_animacija->setVisible(animInd);
    ui->main_kod->setVisible(codeInd);
    textInd = false;
    ui->main_tekst->setVisible(false);
    ui->statusTextMaximize->setVisible(true);
}
void MainWindow::textMaximize(bool a){
    if(a == true){
        animInd = ui->main_animacija->isVisible();
        codeInd = ui->main_kod->isVisible();
        ui->main_animacija->setVisible(!a);
        ui->main_kod->setVisible(!a);
    }
    else{
        ui->main_animacija->setVisible(animInd);
        ui->main_kod->setVisible(codeInd);
    }
}
void MainWindow::animationMaximize(bool a){
    if(a == true){
        textInd = ui->main_tekst->isVisible();
        codeInd = ui->main_kod->isVisible();
        ui->main_tekst->setVisible(!a);
        ui->main_kod->setVisible(!a);
    }
    else{
        ui->main_tekst->setVisible(textInd);
        ui->main_kod->setVisible(codeInd);
    }
}
void MainWindow::animationMinimize(){
    ui->main_tekst->setVisible(textInd);
    ui->main_kod->setVisible(codeInd);
    animInd = false;
    ui->main_animacija->setVisible(false);
    ui->statusAnimationMaximize->setVisible(true);
}
void MainWindow::animationClose(){
    ui->main_animacija->setVisible(false);
}
void MainWindow::SortBubbleSort() {
    setSceneProperties(new BubbleSort());
    QString str("BubbleSort.c");
    m_codeW->m_editor->setFile(str);
    m_textW->readFile("txtFiles/bubblesort.txt");
    m_animationW->fitView();
}
void MainWindow::SortInsertionSort(){
    setSceneProperties(new InsertionSort());
    QString str("InsertionSort.c");
    m_codeW->m_editor->setFile(str);
    m_textW->readFile("txtFiles/insertionsort.txt");
    m_animationW->fitView();
}
void MainWindow::SortSelectionSort(){
    setSceneProperties(new SelectionSort());
    QString str("SelectionSort.c");
    m_codeW->m_editor->setFile(str);
    m_textW->readFile("txtFiles/selectionsort.txt");
    m_animationW->fitView();

}
void MainWindow::SortQuickSort(){
    setSceneProperties(new QuickSort());
    QString str("QuickSort.c");
    m_codeW->m_editor->setFile(str);
    m_textW->readFile("txtFiles/qsort.txt");
    m_animationW->fitView();
}
void MainWindow::SortShellSort(){
    setSceneProperties(new ShellSort());
    QString str("ShellSort.c");
    m_codeW->m_editor->setFile(str);
    m_textW->readFile("txtFiles/shellsort.txt");
    m_animationW->fitView();
}
void MainWindow::GraphDFS()
{
    setSceneProperties(new DFS());
    QString str("dfs.c");
    m_codeW->m_editor->setFile(str);
    m_textW->readFile(":text/txtFiles/bfs.txt");
    m_animationW->fitView();
}
void MainWindow::GraphBFS()
{
    setSceneProperties(new BFS());
    QString str("bfs.c");
    m_codeW->m_editor->setFile(str);
    m_textW->readFile(":text/txtFiles/BFS.txt");
    m_animationW->fitView();
}

void MainWindow::GraphASearch()
{
    setSceneProperties(new astar());
    QString str("astar.c");
    m_codeW->m_editor->setFile(str);
    m_textW->readFile(":text/txtFiles/astar.txt");
    m_animationW->fitView();
}

void MainWindow::GraphDijkstra()
{
    setSceneProperties(new dijsktra());
    QString str("dijsktra.c");
    m_codeW->m_editor->setFile(str);
    m_textW->readFile(":text/txtFiles/dijsktra.txt");
    m_animationW->fitView();
}
void MainWindow::DSList()
{
    setSceneProperties(new VisualList);
    QString str("Stack.c");
    m_codeW->m_editor->setFile(str);
    m_textW->readFile("txtFiles/stack.txt");
    m_animationW->fitView();
}
void MainWindow::DSStack(){
    setSceneProperties(new VisualStack);
    QString str("Stack.c");
    m_codeW->m_editor->setFile(str);
    m_textW->readFile("txtFiles/stack.txt");
    m_animationW->fitView();
}
void MainWindow::DSTree()
{
    setSceneProperties(new VisualTree());
    QString str("tree.c");
    m_codeW->m_editor->setFile(str);
    m_textW->readFile("txtFiles/stablo.txt");
    m_animationW->fitView();
}
void MainWindow::DSGraph()
{
    setSceneProperties(new VisualGraph());
    QString str("Stack.c");
    m_codeW->m_editor->setFile(str);
    m_textW->readFile(":text/txtFiles/graph.txt");
    m_animationW->fitView();
}
void MainWindow::setSettings()
{
    settings * blah = new settings();
    blah->show();
}

void MainWindow::loadView() {
    m_animationW->m_algorithm->parseInput(_entry->getEntry());
    m_animationW->upControls(m_animationW->m_algorithm->returnControls());
    setUpView();
}

void MainWindow::onStop()
{
    m_last_action->trigger();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::TreeInOrder()
{
    setSceneProperties(new Inorder());
    QString str("obilasci.c");
    m_codeW->m_editor->setFile(str);
    m_textW->readFile(":text/txtFiles/obilasci.txt");
    m_animationW->fitView();
}

void MainWindow::TreePostOrder()
{
    setSceneProperties(new Postorder());
    QString str("obilasci.c");
    m_codeW->m_editor->setFile(str);
    m_textW->readFile(":text/txtFiles/obilasci.txt");
    m_animationW->fitView();
}

void MainWindow::TreePreOrder()
{
    setSceneProperties(new Preorder());
    QString str("obilasci.c");
    m_codeW->m_editor->setFile(str);
    m_textW->readFile(":text/txtFiles/obilasci.txt");
    m_animationW->fitView();
}
