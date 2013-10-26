#ifndef ASTAR_H
#define ASTAR_H
#include "DATA_STRUCTURES/visualgraph.h"

class astar : public VisualGraph
{
public:
    astar();
protected:
    void initializeAnimation();
private:
    int findNode(NodeSet &foo);
    void prepareForAStar();
    int reconstruct_path(NodeMap &foo, int curr);
};

#endif // ASTAR_H
