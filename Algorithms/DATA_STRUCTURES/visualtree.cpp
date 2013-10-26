#include "visualtree.h"
#include "DATA_STRUCTURES/VisualTreeElement.h"
#include <QtGui>
#include <iostream>
#include <iterator>
#include <vector>
#include <sstream>
#include <random>
#include <algorithm>
#include <ctime>

using namespace std;

//Empty constructor
VisualTree::VisualTree()
    :AnimatedAlgorithm(), m_last_added(0)
{
    m_root = new VisualTreeElement();
    m_depth = 0;
    m_pozicije_cvorova= map<VisualTreeElement *, QPointF>();
    m_animationInitialized= true;
    m_currentStepInAnimation= 0;
    m_numberOfStepsInAnimation=0;

    //    Init controls:
    QPushButton *dodaj_button= m_dodaj = new QPushButton("Dodaj");
    m_dodaj->setStyleSheet("QPushButton{ background-color: rgb(52, 52, 52); color: white; }QPushButton:hover{ background-color: rgb(90,90,90); color: white; font-weight: bold;}");
    m_dodaj->setMinimumWidth(120);
    QPushButton *brisi_button= m_brisi = new QPushButton("Brisi");
    brisi_button->setStyleSheet("QPushButton{ background-color: rgb(52, 52, 52); color: white; }QPushButton:hover{ background-color: rgb(90,90,90); color: white; font-weight: bold;}");
    m_brisi->setMinimumWidth(120);
    QPushButton *nadji_button= m_nadji = new QPushButton("Nadji");
    m_nadji->setStyleSheet("QPushButton{ background-color: rgb(52, 52, 52); color: white; }QPushButton:hover{ background-color: rgb(90,90,90); color: white; font-weight: bold;}");
    m_nadji->setMinimumWidth(120);
    QLineEdit *input= m_line_edit = new QLineEdit();
    input->setMaximumWidth(150);
    m_line_edit->setStyleSheet("QLineEdit {background-color: rgb(102, 102, 102); color: white;}");
    QRegExp regexp("[0-9]+");
    m_line_edit->setValidator(new QRegExpValidator(regexp));
    m_controls= new QHBoxLayout;
    m_controls->addWidget(dodaj_button);
    m_controls->addWidget(brisi_button);
    m_controls->addWidget(nadji_button);
    m_controls->addWidget(input);
    connect(nadji_button, SIGNAL(clicked()), this, SLOT(omotac_nadji()));
    connect(dodaj_button, SIGNAL(clicked()), this, SLOT(omotac_dodaj()));
    connect(brisi_button, SIGNAL(clicked()), this, SLOT(omotac_brisi()));

    connect(this, SIGNAL(endOfAnimation()), this, SLOT(unblockControls()));

}

VisualTree::~VisualTree()
{
    obrisiDrvo(m_root);
    delete m_dodaj;
    delete m_brisi;
    delete m_nadji;
    delete m_line_edit;
    delete m_controls;
}

//Omotaci
void VisualTree::omotac_dodaj(){
    m_pozicije_cvorova.clear();
    add_node(m_line_edit->text().toInt());
    m_animationInitialized= true;
    m_currentStepInAnimation= 0;
    m_numberOfStepsInAnimation= m_operations.size();
    clear_color(m_root);
    startAnimation();
}
void VisualTree::omotac_brisi(){
    m_operations.clear();
    m_animationInitialized= true;
    m_currentStepInAnimation= 0;
    m_numberOfStepsInAnimation= 0;
    blockControls();
    clear_color(m_root);
    m_pozicije_cvorova.clear();
    delete_node(m_line_edit->text().toInt(), m_root,18.75*(1<<(m_depth-1)));
    m_animationInitialized= true;
    m_currentStepInAnimation= 0;
    m_numberOfStepsInAnimation= m_operations.size();
    startAnimation();
}
void VisualTree::omotac_nadji(){
    m_operations.clear();
    m_animationInitialized= true;
    m_currentStepInAnimation= 0;
    m_numberOfStepsInAnimation= 0;
    blockControls();
    clear_color(m_root);
    m_pozicije_cvorova.clear();
    find_node(m_line_edit->text().toInt(), m_root);
    m_animationInitialized= true;
    m_currentStepInAnimation= 0;
    m_numberOfStepsInAnimation= m_operations.size();
    startAnimation();
}

void VisualTree::clear_color(VisualTreeElement* e){
    if(e){
        e->setColor(Qt::green);
        if(e->getLine())
            e->getLine()->setPen(QPen(Qt::black));
        clear_color(e->getLeft());
        clear_color(e->getRight());
    }
}

void VisualTree::obrisiDrvo(VisualTreeElement *current)
{
    if (current) {
        obrisiDrvo(current->getLeft());
        obrisiDrvo(current->getRight());
        delete current;
    }
}

//Parse input

void VisualTree::parseInput(const QString & input) {
    srand(time(0));
    if (input.isEmpty())
        return;
    if (input.at(0) == QChar('r')) {
        QString str= input.mid(2);
        int array_size= str.toInt();
        vector<int> rvinput= generateRandom(array_size);
        m_root->addElements(rvinput);
        return;
    }

    vector<int> vinput;
    istringstream sinput(input.toStdString());
    copy(istream_iterator<int>(sinput), istream_iterator<int>(), back_inserter(vinput));
    m_root->addElements(vinput);
}

std::vector<int> VisualTree::generateRandom(int num_of_nodes)
{
    srand(time(0));
    int step;
    int start_step;
    int delta;
    int stop_point;
    vector<int> help_array= vector<int>(num_of_nodes);
    if (num_of_nodes >= 16) {
        step = (num_of_nodes > 20) ? 100 :75;
        delta= (num_of_nodes > 20) ? 7 : 5;
        stop_point = 16;
    }
    else if (num_of_nodes >= 8) {
        step = 37;
        delta = 4;
        stop_point = 8;
    }
    else {
        int sign;
        for (int i=0; i < num_of_nodes; i++) {
            sign= (rand() % 2) ? -1 : 1;
            help_array[i]= sign*(rand()%num_of_nodes);
        }
        return help_array;
    }

    //    Init helper vector:
    start_step = step;
    int sign= (rand() % 2) ? -1 : 1;
    help_array[0] = step + sign*(rand() % delta);
    for (int i=0; i < num_of_nodes; i++) {
        step = (step / 1.85);
        sign= (rand() % 2) ? -1 : 1;
        if (2*i + 1 >= stop_point)
            break;
        help_array[2*i+1]= help_array[i] - step + sign*(rand() % delta);
        if (2*i + 2 >= stop_point)
            break;
        help_array[2*i+2]= help_array[i] + step + sign*(rand() % delta);
    }
    auto paired= minmax_element(help_array.begin(), help_array.end());
    int min= help_array[paired.first-help_array.begin()];
    int max= help_array[paired.second-help_array.begin()];
    int diff= max - min;

    for (int i= stop_point; i < num_of_nodes; i++) {
        help_array[i]= min + (rand()%diff);
    }
    return help_array;
}

void VisualTree::drawYourself(){
    m_depth = calculateDepth(m_root);
    nacrtajSe(m_root, 0,1, 18.75*(1<<(m_depth-1)));
}

void VisualTree::dodajGranu()
{
    if (m_last_added) {
        QGraphicsLineItem *spasilac= new QGraphicsLineItem(QLineF(m_last_added->pos() + QPointF(15, 15), m_last_added->getParrent()->pos()+ QPointF(15, 15)));
        addItem(spasilac);
        spasilac->setZValue(-100);
        m_last_added->setLine(spasilac);
        m_last_added = 0;
    }
}

void VisualTree::blockControls()
{
    m_dodaj->setEnabled(false);
    m_brisi->setEnabled(false);
    m_nadji->setEnabled(false);
    m_line_edit->setEnabled(false);
}

void VisualTree::unblockControls()
{
    m_dodaj->setEnabled(true);
    m_brisi->setEnabled(true);
    m_nadji->setEnabled(true);
    m_line_edit->setEnabled(true);
}

void VisualTree::izracunajPozicije(VisualTreeElement *e, int nivo, int rb, int korak)
{
    if (e->getParrent() == nullptr) {
        m_pozicije_cvorova.insert(make_pair(e, QPointF(width()/2, 0)));
    }
    else {
        if (rb % 2 == 0) {
            QPointF p= m_pozicije_cvorova[e->getParrent()];
            m_pozicije_cvorova.insert(make_pair(e, p+QPointF(korak, 50)));
        }
        else {
            QPointF p= m_pozicije_cvorova[e->getParrent()];
            m_pozicije_cvorova.insert(make_pair(e, p+QPointF(-korak, 50)));
        }
    }
    if(e->getLeft())
        izracunajPozicije(e->getLeft(), nivo+1, 2*rb-1, korak/2);
    if(e->getRight())
        izracunajPozicije(e->getRight(), nivo+1, 2*rb, korak/2);
}

void VisualTree::nacrtajSe(VisualTreeElement* e, int nivo, int rb, int korak){
    addItem(e);
    if (e->getParrent() == nullptr) {
        e->setPos(width()/2, 0);
    }
    else {
        if (rb % 2 == 0)
            e->setPos(e->getParrent()->pos()+QPointF(korak, 50));
        else
            e->setPos(e->getParrent()->pos()+QPointF(-korak, 50));
    }
    if(e->getParrent()){
        e->setLine(new QGraphicsLineItem(e->getParrent()->pos().rx()+15, e->getParrent()->pos().ry()+15, e->pos().rx()+15, e->pos().ry()+15));
        addItem(e->getLine());
        e->getLine()->setZValue(-100);
    }
    if(e->getLeft())
        nacrtajSe(e->getLeft(), nivo+1, 2*rb-1, korak/2);
    if(e->getRight())
        nacrtajSe(e->getRight(), nivo+1, 2*rb, korak/2);
}

int VisualTree::calculateDepth(VisualTreeElement* root){
    if(root)
        return max(calculateDepth(root->getLeft()),calculateDepth(root->getRight())) + 1;
    else return 0;
}

QHBoxLayout *VisualTree::returnControls()
{
    return m_controls;
}

void VisualTree::omotac()
{
    //find_node(5, m_root);
    add_node(17);
    //    add_node(17);
    //delete_node(12, m_root, 18.75*(1<<(m_depth-1)));

    m_animationInitialized= true;
    m_currentStepInAnimation= 0;
    m_numberOfStepsInAnimation= m_operations.size();

}

void VisualTree::visit_node(VisualTreeElement* node){
    QColor moja_boja;
    if (isGoingForward()) {
       moja_boja= Qt::yellow;
    }
    else {
       moja_boja= Qt::green;
    }
    node->setColor(moja_boja);
    if(node->getLine())
        node->getLine()->setPen(QPen( (isGoingForward()) ? moja_boja : Qt::black));
    QPropertyAnimation *anim= new QPropertyAnimation(node, "opacity");
    anim->setDuration(1500);
    anim->setStartValue(0);
    anim->setEndValue(1);
    connect(anim, SIGNAL(finished()), this, SLOT(animationFinished()));
    anim->start(QAbstractAnimation::DeleteWhenStopped);
}

bool VisualTree::find_node(int value, VisualTreeElement*e)
{
    if(e){
        m_operations.push_back([=, this] () { visit_node(e); emit highlightLine(97);});
        if(e->getValue() < value){
            return find_node(value, e->getRight());
        }
        else if(e->getValue() > value){
            return find_node(value, e->getLeft());
        }
        else{
            return true;
        }
    }
    else return false;
}

void VisualTree::move_node(VisualTreeElement *pom, VisualTreeElement *novi, int ind, int korak){

    QPropertyAnimation *anim= new QPropertyAnimation(novi, "pos");
    anim->setDuration(1500);
    anim->setStartValue(novi->pos());
    if(ind)
        anim->setEndValue(pom->getRight()->pos() + QPointF(40,0));
    else
        anim->setEndValue(pom->getLeft()->pos() + QPointF(40,0));
    connect(anim, SIGNAL(finished()), this, SLOT(animationFinished()));
    anim->start(QAbstractAnimation::DeleteWhenStopped);
}

void VisualTree::add_node(int value){
    m_operations.clear();
    m_animationInitialized= true;
    m_currentStepInAnimation= 0;
    m_numberOfStepsInAnimation= 0;
    blockControls();

    VisualTreeElement* novi= new VisualTreeElement(value);
    int korak = 18.75*(1<<(m_depth-1));
    addItem(novi);
    novi->setPos(width()/2+40, 0);
    VisualTreeElement *pom = m_root;

    if(pom == nullptr){
        //dodajemo u prazno stablo
        novi->setPos(width()/2, 0);
    }
    else {
        while(1){
            if(value < pom->getValue()){
                if(pom->getLeft()){
                    m_operations.push_back([=, this] () { move_node(pom, novi, 0, 0); emit highlightLine(8);});
                    pom = pom->getLeft();
                }
                else{
                    m_operations.push_back([=, this] () {m_last_added= novi;  siftuj_podstablo(m_root, 0, 0);
                    emit highlightLine(11);
                    });
                    break;
                }
            }
            else if (value > pom->getValue()){
                if(pom->getRight()){
                    m_operations.push_back([=, this] () { move_node(pom, novi, 1, 0); emit highlightLine(17); });
                    pom = pom->getRight();
                }
                else{
                    m_operations.push_back([=, this] () { m_last_added= novi; siftuj_podstablo(m_root, 0, 0);
                    emit highlightLine(20);
                    });
                    break;
                }
            }
            else {
                m_operations.push_back([=, this] () { delete novi; });
                break;
            }
            korak/=2;
        }
    }
    m_root->add_node(m_root, value, novi);
    m_root->write_tree(m_root);

    m_animationInitialized= true;
    m_currentStepInAnimation= 0;
    m_numberOfStepsInAnimation= m_operations.size();
}

void VisualTree::crtajOpet(VisualTreeElement* e, int nivo, int rb, int korak, int value){
    if (e->getParrent() == nullptr) {
        e->setPos(width()/2, 0);
    }
    else {
        if (rb % 2 == 0)
            e->setPos(e->getParrent()->pos()+QPointF(korak, 50));
        else
            e->setPos(e->getParrent()->pos()+QPointF(-korak, 50));
    }
    if(e->getParrent()){
        e->setLine(new QGraphicsLineItem(e->getParrent()->pos().rx()+15, e->getParrent()->pos().ry()+15, e->pos().rx()+15, e->pos().ry()+15));
        e->getLine()->setZValue(-100);
    }
    if(e->getValue() == value){
        addItem(e->getLine());
        addItem(e);
    }
    if(e->getLeft())
        crtajOpet(e->getLeft(), nivo+1, 2*rb-1, korak/2, value);
    if(e->getRight())
        crtajOpet(e->getRight(), nivo+1, 2*rb, korak/2, value);
}

void VisualTree::delete_node(int value, VisualTreeElement* e, int korak)
{
    if(e){
        m_operations.push_back([=, this] () {
            visit_node(e);
        });
        if(e->getValue() < value){
            return delete_node(value, e->getRight(), korak/2);
        }
        else if(e->getValue() > value){
            return delete_node(value, e->getLeft(), korak/2);
        }
        else{
            if(e->getLeft() == nullptr && e->getRight() == nullptr){
                //brisemo list
                m_operations.push_back([=, this] () {
                    removeItem(e);
                    int t;
                    m_root->delete_node(e->getParrent(), t=e->getValue());
                    cout << "moje t= "<<t<<endl;
                    siftuj_podstablo(0, 0, 0);
                    emit highlightLine(35);
                });
            }
            else if(e->getLeft() == nullptr){
                //nema levo podstablo, siftovati tako da koren desnog podstabla bude na mestu cvora koji se brise.
                m_operations.push_back([=, this] () {
                                       emit highlightLine(45);
                    delete e->getLine();
                    if(e->getParrent()->getLeft() == e){
                        e->getParrent()->setLeft(e->getRight());
                        e->getRight()->setParrent(e->getParrent());
                    }
                    else{
                        e->getParrent()->setRight(e->getRight());
                        e->getRight()->setParrent(e->getParrent());
                    }
                    removeItem(e);
                    m_root->delete_node(m_root, e->getValue());
                    siftuj_podstablo(e->getRight(), korak,-1);
                });
            }
            else if(e->getRight() == nullptr){
                //nema desno podstablo
                m_operations.push_back([=, this] () {
                                       emit highlightLine(56);
                    delete e->getLine();
                    if(e->getParrent()->getLeft() == e){
                        e->getParrent()->setLeft(e->getLeft());
                        e->getLeft()->setParrent(e->getParrent());
                    }
                    else{
                        e->getParrent()->setRight(e->getLeft());
                        e->getLeft()->setParrent(e->getParrent());
                    }
                    removeItem(e);
                    m_root->delete_node(m_root, e->getValue());
                    siftuj_podstablo(e->getLeft(), korak, 1);
                });
            }
            else{
                //ima oba podstabla, bira se minimum desnog...
                VisualTreeElement* min = m_root->nadji_minimum(e->getRight(), 5000);
                m_operations.push_back([=, this] () {
                    swap_values(min, e);
                });
                m_operations.push_back([=, this] () {
                    swap_values2(min,e);
                });
                delete_node(min->getValue(), e->getRight() ,korak);
            }
        }
    }
}

void VisualTree::swap_values(VisualTreeElement *e1, VisualTreeElement *e2)
{
    QParallelAnimationGroup * pgr = new QParallelAnimationGroup;

    e1->setColor(Qt::cyan);
    e2->setColor(Qt::cyan);
    QPropertyAnimation *a1= new QPropertyAnimation(e1, "opacity");
    a1->setDuration(1000);
    a1->setStartValue(1);
    a1->setEndValue(0);
    QPropertyAnimation *a2= new QPropertyAnimation(e2, "opacity");
    a2->setDuration(1000);
    a2->setStartValue(1);
    a2->setEndValue(0);

    int tmp;
    tmp = e1->getValue();
    e1->setValue(e2->getValue());
    e2->setValue(tmp);

    pgr->addAnimation(a1);
    pgr->addAnimation(a2);
    pgr->start(QAbstractAnimation::DeleteWhenStopped);

    connect(pgr, SIGNAL(finished()), this, SLOT(animationFinished()));

}

void VisualTree::swap_values2(VisualTreeElement *e1, VisualTreeElement *e2)
{
    QParallelAnimationGroup * pgr = new QParallelAnimationGroup;
    QPropertyAnimation *a1= new QPropertyAnimation(e1, "opacity");
    a1->setDuration(1000);
    a1->setStartValue(0);
    a1->setEndValue(1);
    QPropertyAnimation *a2= new QPropertyAnimation(e2, "opacity");
    a2->setDuration(1000);
    a2->setStartValue(0);
    a2->setEndValue(1);
    pgr->addAnimation(a1);
    pgr->addAnimation(a2);
    pgr->start(QAbstractAnimation::DeleteWhenStopped);

    connect(pgr, SIGNAL(finished()), this, SLOT(animationFinished()));
}

void VisualTree::siftuj_podstablo(VisualTreeElement* root, int korak, int strana)
{
    std::cout << "Usao" << std::endl;
    QParallelAnimationGroup * pgr = new QParallelAnimationGroup;
    m_depth= calculateDepth(m_root);
    izracunajPozicije(m_root, 0,1, 18.75*(1<<(m_depth-1)));
    dodaj_u_pgr( pgr );
    connect(pgr, SIGNAL(finished()), this, SLOT(animationFinished()));
    connect(pgr, SIGNAL(finished()), this, SLOT(dodajGranu()));
    pgr->start(QAbstractAnimation::DeleteWhenStopped);
}

void VisualTree::dodaj_u_pgr(QParallelAnimationGroup* &pgr){
    for (auto &e : m_pozicije_cvorova) {
        QPropertyAnimation *e1= new QPropertyAnimation(e.first, "pos");
        e1->setDuration(1000);
        e1->setStartValue(e.first->pos());
        e1->setEndValue(e.second);
        pgr->addAnimation(e1);

        if (e.first->getLine()) {
            QPropertyAnimation *e2= new QPropertyAnimation(e.first, "grana");
            e2->setDuration(1000);
            e2->setStartValue(0);
            e2->setEndValue(1);
            pgr->addAnimation(e2);
        }
    }
}

void VisualTree::nacrtaj_sve_grane(VisualTreeElement *e, int nivo, int rb, int korak)
{
    if(e->getParrent()){
        e->setLine(new QGraphicsLineItem(e->getParrent()->pos().rx()+15, e->getParrent()->pos().ry()+15, e->pos().rx()+15, e->pos().ry()+15));
        addItem(e->getLine());
        e->getLine()->setZValue(-100);
    }
    if(e->getLeft())
        nacrtaj_sve_grane(e->getLeft(), nivo+1, 2*rb-1, korak/2);
    if(e->getRight())
        nacrtaj_sve_grane(e->getRight(), nivo+1, 2*rb, korak/2);
}

void VisualTree::obrisi_sve_grane(VisualTreeElement *e)
{
    if(e){
        if(e->getLine())
            delete e->getLine();
        obrisi_sve_grane(e->getLeft());
        obrisi_sve_grane(e->getRight());
    }
}


