#ifndef POSTORDER_H
#define POSTORDER_H
#include "DATA_STRUCTURES/visualtree.h"

class Postorder : public VisualTree
{
    Q_OBJECT
public:
    Postorder(QObject *parent = 0);
    void initializeAnimation();
    void obidji_ldk(VisualTreeElement* e);
    QHBoxLayout * returnControls();

};

#endif // POSTORDER_H
