#ifndef DFS_H
#define DFS_H
#include "DATA_STRUCTURES/visualgraph.h"

class DFS: public VisualGraph
{
public:
    DFS();
private:
    void initializeAnimation();
    void dfs(int index_of_current_node);
};

#endif // DFS_H
