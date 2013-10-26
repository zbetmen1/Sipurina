void astar()
{
    int curr = _currentNode;
    NodeSet closeSet;
    NodeSet openSet;
    NodeMap cameFrom;
    openSet.insert(curr);
    getNode(curr)->setValue(0);
    while(!openSet.isEmpty()){
        curr = findNode(openSet);
        if(curr == _endNode){
             reconstruct_path(cameFrom,curr);
             break;
        }
        openSet.remove(curr);
        closeSet.insert(curr);
        for (int i : getNeighbours(curr)){
            int testScore = getNode(curr)->value() + getEdge(curr, i)->weight();
            if(closeSet.find(i) != closeSet.constEnd())
                continue;
            if(openSet.find(i) == openSet.constEnd() || testScore < getNode(i)->value()){
                cameFrom[i] = curr;
                getNode(i)->setValue(testScore);
                if(openSet.find(i) == openSet.constEnd())
                    openSet.insert(i);
            }
        }
    }
}
int reconstruct_path(NodeMap &foo, int curr){

    if(foo.find(curr) != foo.constEnd()){
        int p = reconstruct_path(foo,foo[curr]);
        return (p+curr);
        }
    else
    return curr;
}

int astar::findNode(VisualGraph::NodeSet &foo)
{
    int curr = 9999;
    int ret;
    NodeSet::Iterator iter = foo.begin();
    while(iter!= foo.end()){
        if(getNode(*iter)->value() + getNode(*iter)->Heuristic() < curr ){
            ret = *iter;
            curr = getNode(*iter)->value() + getNode(*iter)->Heuristic();
        }
        iter++;
    }
    return ret;
}