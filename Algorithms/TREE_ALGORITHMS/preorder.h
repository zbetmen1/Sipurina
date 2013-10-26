#ifndef PREORDER_H
#define PREORDER_H
#include "DATA_STRUCTURES/visualtree.h"

class Preorder : public VisualTree
{
    Q_OBJECT
public:
    Preorder(QObject *parent = 0);
    void initializeAnimation();
    void obidji_kld(VisualTreeElement* e);
    QHBoxLayout * returnControls();
};

#endif // PREORDER_H
