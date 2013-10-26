#include "bfs.h"
#include<QQueue>

BFS::BFS()
    :VisualGraph()
{

}

void BFS::initializeAnimation(){
    setMenu(false);
    int r=255, g=255, b=0;
    int color_step= 25;
    m_operations.push_back([=,this](){ getNode(_currentNode)->setColor(QColor(r, g, b));colorNode(_currentNode); });
    r-=color_step;
    g-=color_step;
    QQueue<int> queue;
    queue.append(_currentNode);
    getNode(_currentNode)->setVisited(true);
    while(!queue.empty()){
        int curr = queue.at(0);
        queue.pop_front();
        NodeList list = getNeighbours(curr);
        for (auto iter : list){
            if(!getNode(iter)->visited()){
                getNode(iter)->setVisited(true);
                m_operations.push_back([=,this](){getEdge(curr, iter)->setColor(QColor(r,g,b)); colorEdge(curr, iter); emit highlightLine(10);});
            m_operations.push_back([=,this](){getNode(iter)->setColor(QColor(r,g,b)); colorNode(iter);});
                queue.append(iter);
            }
        }
        r-=color_step;
        g-=color_step;
    }

    m_animationInitialized= true;
    m_currentStepInAnimation= 0;
    m_numberOfStepsInAnimation= m_operations.size();
}
