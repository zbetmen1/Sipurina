#ifndef VISUALTREE_H
#define VISUALTREE_H
#include "DATA_STRUCTURES/VisualTreeElement.h"
#include "CORE_CLASSES/animatedalgorithm.h"
#include <QParallelAnimationGroup>
#include <map>
#include <QPushButton>
class VisualTree : public AnimatedAlgorithm
{
public:
    VisualTree();
    //VisualTree(int v, VisualTree* left, VisualTree* right, VisualTree* parrent);

    ~VisualTree();

    void parseInput(const QString &input);
    std::vector<int> generateRandom(int num_of_nodes);

    void nacrtajSe(VisualTreeElement* e, int nivo, int rb, int korak);
    int calculateDepth(VisualTreeElement* root);
    QHBoxLayout *returnControls();

    //Operacije
protected slots:
    void omotac();
    bool find_node(int value,VisualTreeElement* node);
    void visit_node(VisualTreeElement* node);
    void add_node(int value);
    void move_node(VisualTreeElement *pom, VisualTreeElement *novi, int ind, int korak);
    void crtajOpet(VisualTreeElement* e, int nivo, int rb, int korak,int value);
    void delete_node(int value, VisualTreeElement *e, int korak);
    void siftuj_podstablo(VisualTreeElement* root, int korak, int strana);
    void dodaj_u_pgr(QParallelAnimationGroup *&pgr);
    void obrisi_sve_grane(VisualTreeElement* e);
    void nacrtaj_sve_grane(VisualTreeElement* e, int nivo, int rb, int korak);
    void pomeri_sve_cvorove(VisualTreeElement* e);
    void swap_values(VisualTreeElement *e1, VisualTreeElement *e2);
    void swap_values2(VisualTreeElement *e1, VisualTreeElement *e2);
    void clear_color(VisualTreeElement* e);
    VisualTreeElement* getRoot(){return m_root;}

public slots:
    void drawYourself();
private slots:
    void dodajGranu();
    void blockControls();
    void unblockControls();
    void omotac_dodaj();
    void omotac_brisi();
    void omotac_nadji();
    void obrisiDrvo(VisualTreeElement *current);
private:
    VisualTreeElement* m_root;
    VisualTreeElement *m_last_added;
    int m_depth;
    std::map<VisualTreeElement *, QPointF> m_pozicije_cvorova;

    //Kontrole
    QHBoxLayout * m_controls;
    QLineEdit * m_line_edit;
    QPushButton *m_dodaj, *m_brisi, *m_nadji;

//    Functions:
    void izracunajPozicije(VisualTreeElement* e, int nivo, int rb, int korak);

};

#endif // VISUALTREE_H
