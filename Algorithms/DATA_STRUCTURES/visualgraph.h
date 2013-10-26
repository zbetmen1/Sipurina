#ifndef VISUALGRAPH_H
#define VISUALGRAPH_H


/* Boost headers */
#include <boost/random/linear_congruential.hpp>
#include <boost/graph/fruchterman_reingold.hpp>
#include <boost/graph/random_layout.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/topology.hpp>
#include <boost/lexical_cast.hpp>

/* STL headers */
#include <string>
#include <iostream>
#include <map>
#include <vector>
#include <sstream>
#include <map>
#include <ctime>
#include <iterator>
#include <algorithm>

/* Qt headers */
#include <QList>
#include <QGraphicsWidget>
#include <QGraphicsScene>
#include <QMap>
#include <QPair>
#include <QMenu>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QAction>
#include <QInputDialog>
#include <QRegExpValidator>
#include <QSet>
#include <QMessageBox>

/* My headers*/
#include "node.h"
#include "edge.h"

/*Kocka headers*/
#include "CORE_CLASSES/animatedalgorithm.h"


class VisualGraph : public AnimatedAlgorithm
{

    Q_OBJECT

public:

    /* Boost typedefs */
    typedef boost::rectangle_topology<> topology_type;
    typedef topology_type::point_type point_type;
    typedef boost::adjacency_list<boost::listS, boost::vecS, boost::directedS > BoostGraph;
    typedef boost::graph_traits<BoostGraph>::vertex_descriptor Vertex;
    typedef std::vector<point_type> PositionVec;
    typedef boost::graph_traits<BoostGraph>::vertex_iterator graphVertexIterator;
    typedef boost::iterator_property_map<PositionVec::iterator, boost::property_map<BoostGraph, boost::vertex_index_t>::type> PositionMap;

    /* QtGraph typedefs */
    typedef QMap<int, Node*> GraphNodes;
    typedef QPair<int, int> EdgeIndex;
    typedef QMap<EdgeIndex, Edge*> GraphEdges;
    typedef GraphNodes::iterator NodeIterator;
    typedef GraphEdges::iterator EdgeIterator;
    typedef QList<Edge*> EdgeList;
    typedef EdgeList::iterator EdgeListIterator;
    typedef QList<int> NodeList;
    typedef QSet<int> NodeSet;
    typedef NodeList::iterator NodeListIterator;
    typedef QMap<int, int> NodeMap;

    enum directed{
        Directed = true,
        Undirected = false
    };

    enum weighted{
        Weighted = true,
        Unweighted = false
    };

    /* Graph behaviour methods */
    VisualGraph(VisualGraph::directed flagD = VisualGraph::Undirected, VisualGraph::weighted flagW = VisualGraph::Unweighted);
    ~VisualGraph();
    void addNode(int position);
    void deleteNode(int node);
    void addEdge(int start, int end, int weight=1);
    void deleteEdge(int start, int end);
    int currentNode() { return _currentNode;}
    void setCurrentNode(int node){ _currentNode = node; }
    Edge* getEdge(int a, int b){ EdgeIterator iter =  _edges.find(EdgeIndex(a,b)); return (iter != _edges.end()) ? iter.value() : nullptr;  }
    Node* getNode(int index){ return _nodes[index]; }
    NodeList getNeighbours(int node);
    void visitNode(int node){ _previousNode = _currentNode; _currentNode = node;  _nodes[node]->_visited = true; }
    void setMenu(bool foo){ Node::_enabledMenu = foo; _menu = foo; }
    void entryProcess(QString & str);
    void testNode();
    /*Graph visualization methods*/
    void parseInput(const QString & input);
    void setRegExpValidator(QLineEdit *&random_edit, QLineEdit *&by_hand_edit);
    bool isDirected(){return _directed;}
    void setAllNodes(int foo);
public slots:
    void drawYourself();
protected slots:
    void colorNode(int);
    void colorEdge(int, int);
private slots:
    void addNodeSlot();
    void deleteEdgeSlot();
    void addEdgeSlot();
    void deleteNodeSlot(int node);
    void setCurrent();
    void setEnd();
    void save();
private:

    bool _menu;
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    BoostGraph createBoostGraph();
     /* Fruchterman-Reingold layout*/
    void getFRLayout(double width, double height);
    QString toString();
    /* Number of node, List of edges FROM that node */
    GraphEdges _edges;

    /* */
    /* Number of node, Node */
    GraphNodes _nodes;
    int _numberOfNodes;
    bool _directed;
    bool _weighted;
    QString generateRandomGraph(int nodes, int edges, bool directed, bool weigthed);
    QMenu *_rightClickMenu;
    QAction * _addNodeAction;
    QAction * _addEdgeAction;
    QAction * _deleteEdgeAction;
    QAction * _currNodeAction;
    QAction * _endNodeAction;
    QAction * _save;
    QWidget * style;
//    Method:

protected:
    int _previousNode;
    int _currentNode;
    int _endNode;
};

#endif // VISUALGRAPH_H
