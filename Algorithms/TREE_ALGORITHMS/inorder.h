#ifndef INORDER_H
#define INORDER_H
#include "DATA_STRUCTURES/visualtree.h"

class Inorder : public VisualTree
{
    Q_OBJECT
public:
    Inorder(QObject *parent = 0);
    void initializeAnimation();
    void obidji_lkd(VisualTreeElement* e);
    QHBoxLayout * returnControls();

};

#endif // INORDER_H
