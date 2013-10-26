#include<iostream>
#include "astar.h"

astar::astar()
    :VisualGraph()
{
}

void astar::initializeAnimation()
{
    setMenu(false);
    int curr = _currentNode;
    m_operations.push_back([=,this](){getNode(_endNode)->setColor(Qt::green);colorNode(_endNode);});
    NodeSet closeSet;
    NodeSet openSet;
    NodeMap cameFrom;
    openSet.insert(curr);
    getNode(curr)->setValue(0);
    while(!openSet.isEmpty()){
        curr = findNode(openSet);
        if(curr == _endNode){
             reconstruct_path(cameFrom,curr); /* <-- na kraju */
             m_operations.push_back([=, this](){ getNode(_currentNode)->setColor(Qt::red); colorNode(_currentNode); });
             m_animationInitialized= true;
             m_currentStepInAnimation= 0;
             m_numberOfStepsInAnimation= m_operations.size();
             break;
        }
        openSet.remove(curr);
        /*if(curr != temp)
            m_operations.push_back([=,this]{ getEdge(temp, curr)->setColor(Qt::yellow); colorEdge(temp, curr); })*/;
        m_operations.push_back([=, this](){ getNode(curr)->setColor(Qt::yellow); colorNode(curr); emit highlightLine(15); });

        closeSet.insert(curr);
        for (int i : getNeighbours(curr)){
            int testScore = getNode(curr)->value() + getEdge(curr, i)->weight();
            if(closeSet.find(i) != closeSet.constEnd())
                continue;
            if(openSet.find(i) == openSet.constEnd() || testScore < getNode(i)->value()){
                m_operations.push_back([=,this]{ getEdge(curr, i)->setColor(Qt::yellow); colorEdge(curr, i); });
                cameFrom[i] = curr;
                int cubic= getNode(i)->value();
                getNode(i)->setValue(testScore);
                m_operations.push_back([=, this]() {
                if (isGoingForward())
                    getNode(i)->setValue(testScore);
                else
                    getNode(i)->setValue(cubic);
                 emit ready();
                });
                if(openSet.find(i) == openSet.constEnd())
                    openSet.insert(i);
            }

        }
      //  std::cout << curr << std::endl;
    }
    /*ispis da nema*/
    setAllNodes(9999);
    m_animationInitialized= true;
    m_currentStepInAnimation= 0;
    m_numberOfStepsInAnimation= m_operations.size();
}
int astar::reconstruct_path(NodeMap &foo, int curr){

    if(foo.find(curr) != foo.constEnd()){
        m_operations.push_back([=, this](){ getNode(curr)->setColor(Qt::red); colorNode(curr); emit highlightLine(32); });
        m_operations.push_back([=,this]{
                               if(!isDirected()){
                                    getEdge(curr, foo[curr])->setColor(Qt::red);
                                    colorEdge(curr, foo[curr]);
                               }
                              else{
getEdge(foo[curr],curr)->setColor(Qt::red);
colorEdge(foo[curr],curr);
}}
);
        int p = reconstruct_path(foo,foo[curr]);
        return (p+curr);
        }
    else{
        if(curr == _currentNode)
            return 0;
        m_operations.push_back([=, this](){ getNode(curr)->setColor(Qt::red); colorNode(curr); emit highlightLine(36); });
        m_operations.push_back([=,this]{
                               if(!isDirected()){
                                    getEdge(curr, foo[curr])->setColor(Qt::red);
                                    colorEdge(curr, foo[curr]);
                               }
                              else{
getEdge(foo[curr],curr)->setColor(Qt::red);
colorEdge(foo[curr],curr);
}}
);
        return curr;
    }
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


