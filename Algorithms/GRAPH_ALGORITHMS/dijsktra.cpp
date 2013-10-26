#include "dijsktra.h"

dijsktra::dijsktra()
    :astar()
{
    Node::_disableH = true;
}

void dijsktra::initializeAnimation()
{
    astar::initializeAnimation();
}
