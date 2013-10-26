#include "visualgraph.h"
#include<QTextStream>
using namespace std;

VisualGraph::VisualGraph(VisualGraph::directed flagD, VisualGraph::weighted flagW)
    :AnimatedAlgorithm()
{
    _directed = flagD;
    _weighted = flagW;
    _menu = true;
    style = new QWidget();
    style->setStyleSheet("QInputDialog {background-color: rgb(140, 140, 140); color: white;} QPushButton { background-color: rgb(17, 17, 17); color: white;} QSpinBox{background-color: black; color: white;} QLineEdit{background-color: black; color: white;} ");
    Node::setVId(true);
    _rightClickMenu = new QMenu();
     _rightClickMenu->setStyleSheet("QMenu {background-color: rgb(140, 140, 140)}");
    _addNodeAction = _rightClickMenu->addAction("Add Node");
    connect(_addNodeAction,SIGNAL(triggered()),this,SLOT(addNodeSlot()));
    _deleteEdgeAction = _rightClickMenu->addAction("Delete Edge");
    connect(_deleteEdgeAction,SIGNAL(triggered()),this,SLOT(deleteEdgeSlot()));
    _addEdgeAction = _rightClickMenu->addAction("Add edge");
    connect(_addEdgeAction,SIGNAL(triggered()),this,SLOT(addEdgeSlot()));
    _currNodeAction = _rightClickMenu->addAction("Start Node");
    connect(_currNodeAction,SIGNAL(triggered()),this,SLOT(setCurrent()));
    _endNodeAction = _rightClickMenu->addAction("End Node");
    connect(_endNodeAction,SIGNAL(triggered()),this,SLOT(setEnd()));
    _save = _rightClickMenu->addAction("Save Graph!");
    connect(_save,SIGNAL(triggered()),this,SLOT(save()));
    Node::_enabledMenu = true;
    Node::_disableH = false;
}
void VisualGraph::addNode(int position){
    if(_nodes.find(position) == _nodes.end()){
        Node * temp =new Node(0, position);
        temp->_graphicXCoord = 0;
        temp->_graphicYCoord = 0;
        connect(temp,SIGNAL(deleteN(int)),this,SLOT(deleteNodeSlot(int)));
        _nodes.insert(position,temp);
        _numberOfNodes++;
    }
}
void VisualGraph::addEdge(int start, int end, int weight){
    if(_nodes.find(start) == _nodes.end())
        addNode(start);
    if(_nodes.find(end) == _nodes.end())
            addNode(end);
    Edge * temp = new Edge(_nodes[start],_nodes[end]);
    _edges.insert(EdgeIndex(start, end),temp);
    temp->setWeight(weight);

    if(!_directed){
        Edge *temp2 = new Edge(_nodes[end], _nodes[start]);
        _edges.insert(EdgeIndex(end, start), temp2);
        temp2->setWeight(weight);
    }
    if(_directed){
        temp->setDirected(_directed);
    }
}
void VisualGraph::deleteNode(int node)
{
    if(_nodes.find(node) != _nodes.end()){
        delete _nodes[node];
        _nodes.remove(node);
        EdgeIterator iter = _edges.begin();
        while(iter != _edges.end()){
            if(iter.key().first == node || iter.key().second == node ){
                delete _edges[iter.key()];
                _edges.remove(iter.key());
            }
            iter++;
        }
        _numberOfNodes--;
    }
}
void VisualGraph::deleteEdge(int start, int end)
{
    EdgeIndex index(start, end);
    delete _edges[index];
    _edges.remove(index);
    if(!_directed){
        EdgeIndex indexReverse(end, start);
        delete _edges[indexReverse];
        _edges.remove(indexReverse);
    }
}
VisualGraph::NodeList VisualGraph::getNeighbours(int node)
{
    NodeList returnValue;
    EdgeIterator iter = _edges.lowerBound(EdgeIndex(node,0));
    while(iter.key().first == node){
        returnValue.push_back(iter.key().second);
        iter++;
    }
    return returnValue;
}
void VisualGraph::testNode()
{
    NodeIterator iter = _nodes.begin();
    while(iter != _nodes.end()){
        std::cout << "Cvor: " <<iter.key() <<", X: " << iter.value()->getX() << ", Y: " << iter.value()->getY() << std::endl;
        ++iter;
    }
}
void VisualGraph::drawYourself()
{

    NodeIterator iterN = _nodes.begin();
    _currentNode= iterN.key();
    _endNode = (--_nodes.end()).key();
    EdgeIterator iterE = _edges.begin();
    while(iterE != _edges.end()){
        if(_edges.find(EdgeIndex(iterE.key().second, iterE.key().first)) != _edges.end() && _directed)
            iterE.value()->_arcNeed = true;
        if (_weighted)
            Edge::_isWeight = true;
        iterE.value()->setGeometryEdge();
        addItem(_edges[iterE.key()]);
        iterE++;
    }
    while(iterN != _nodes.end()){
        iterN.value()->setGeometryNode();
        addItem(iterN.value());
        iterN++;
    }
}
void VisualGraph::colorNode(int index)
{
    Node *node= getNode(index);
    node->setColor((isGoingForward()) ? node->color() : Qt::blue);
    QPropertyAnimation *opacity= new QPropertyAnimation(node, "opacity");
    opacity->setDuration(1500);
    opacity->setStartValue(0);
    opacity->setEndValue(1);
    connect(opacity, SIGNAL(finished()), this, SLOT(animationFinished()));
    opacity->start(QAbstractAnimation::DeleteWhenStopped);
}
void VisualGraph::colorEdge(int first_node_index, int second_node_index)
{
    Edge* edge_first= getEdge(first_node_index, second_node_index);
    edge_first->setColor((isGoingForward()) ? edge_first->color() : Qt::black);
    QPropertyAnimation *opacity_first= new QPropertyAnimation(edge_first, "opacity");
    opacity_first->setDuration(1500);
    opacity_first->setStartValue(0);
    opacity_first->setEndValue(1);
    if (_directed) {
        connect(opacity_first, SIGNAL(finished()), this, SLOT(animationFinished()));
        opacity_first->start(QAbstractAnimation::DeleteWhenStopped);
    }
    else {
        Edge* edge_second= getEdge(second_node_index, first_node_index);
        edge_second->setColor((isGoingForward()) ? edge_first->color() : Qt::black);
        QPropertyAnimation *opacity_second= new QPropertyAnimation(edge_second, "opacity");
        opacity_second->setDuration(1500);
        opacity_second->setStartValue(0);
        opacity_second->setEndValue(1);
        QParallelAnimationGroup *pgroup= new QParallelAnimationGroup;
        pgroup->addAnimation(opacity_first);
        pgroup->addAnimation(opacity_second);
        connect(pgroup, SIGNAL(finished()), this, SLOT(animationFinished()));
        pgroup->start(QAbstractAnimation::DeleteWhenStopped);
    }
}
void VisualGraph::addNodeSlot()
{
    int foo = QInputDialog::getInt(style,"Enter ID","Entere ID of new Node");
    if(foo < 0){
       QMessageBox::critical(style,"Graph Error","Node ID cannot be negative!");
       return;
    }
    addNode(foo);
    addItem(_nodes[foo]);
}
void VisualGraph::deleteEdgeSlot()
{
    int start = QInputDialog::getInt(style,"Enter Start Node","Enter Start Node");
    int end = QInputDialog::getInt(style,"Enter end Node","Enter end Node");
    if(_nodes.find(start) == _nodes.end() || _nodes.find(end) == _nodes.end()){
        QMessageBox::critical(style,"Graph Error","One or Both Nodes which is entered doesnt exist!");
        return;
    }
    deleteEdge(start, end);
}
void VisualGraph::addEdgeSlot()
{
    int start = QInputDialog::getInt(style,"Enter Start Node","Enter Start Node");
    int end = QInputDialog::getInt(style,"Enter end Node","Enter end Node");
    if(_weighted){
        int weight = QInputDialog::getInt(style,"Enter Weight","Enter weight of Node");
        addEdge(start, end,weight);
        Edge::_isWeight = true;
    }
    else
        addEdge(start, end);
    _edges[EdgeIndex(start,end)]->setZValue(-1);
    addItem(_edges[EdgeIndex(start,end)]);
}
void VisualGraph::deleteNodeSlot(int node)
{
    deleteNode(node);
}
void VisualGraph::setCurrent()
{

    int temp =QInputDialog::getInt(style,"Current Node","Enter current (start) node");
    if(_nodes.find(temp) == _nodes.end()){
        QMessageBox::critical(style,"Graph Error","Node doesnt exists!");
        return;
    }
    _currentNode = temp;
}
void VisualGraph::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsScene::mousePressEvent(event);
    if(!event->isAccepted()){
        if(event->button() == Qt::RightButton){
            if(_menu)
                _rightClickMenu->popup(QCursor::pos());
        }
    }
}
void VisualGraph::getFRLayout(double width, double height)
{

    int iterations = 100;
    BoostGraph g = createBoostGraph();

    PositionVec position_vec(boost::num_vertices(g));
    PositionMap position(position_vec.begin(), get(boost::vertex_index, g));

    boost::minstd_rand gen;
    topology_type topo(gen, 0, 0, width, height-60);
    random_graph_layout(g, position, topo);
    fruchterman_reingold_force_directed_layout
    (g, position, topo,
     boost::cooling(boost::linear_cooling<double>(iterations)));


    graphVertexIterator vi, vi_end;
    NodeIterator iter = _nodes.begin();
    for (boost::tie(vi, vi_end) = vertices(g); vi != vi_end; ++vi) {
            iter.value()->x(position[*vi][0]);
            iter.value()->y(position[*vi][1]);
            iter++;
    }

}

QString VisualGraph::toString()
{
    QString foo;
    if(_directed)
        foo += QString("d ");
    EdgeIterator iter = _edges.begin();
    while(iter != _edges.end()){

        foo += QString::number(iter.key().first)+QString(" ");
        foo += QString::number(iter.key().second)+QString(" ");
        if(_weighted)
            foo += QString::number(iter.value()->weight())+QString(" ");
        foo+=QString(",");
        ++iter;
    }
    return foo;
}

QString VisualGraph::generateRandomGraph(int nodes, int edges, bool directed, bool weigthed)
{
    srand(time(0));
    bool is_nodes_p;
    if (is_nodes_p = (nodes % 2 == 0))
        edges= (edges < nodes/2) ? nodes/2 : edges;
    else
        edges= (edges < nodes/2+1) ? nodes/2+1 : edges;

//    Init return string:
    QString return_string;
    if (directed) {
        return_string= return_string + "d ";
    }

//    Init map:
    map<int , bool> map_pairs;
    for (int i=0; i < nodes; i++)
        map_pairs.insert(make_pair(i, false));

//    Make first nodes/2 edges so every node is used at least once:
    for (int i=0; i < nodes/2; i++) {
//        Choose elements and deal with them when they are the same element:
        int map_size= map_pairs.size();
        int first= rand() % map_size;
        int second= rand() % map_size;
        if (first == second) {
            first= (second + 1)%map_size;
        }

//        Extract pair from first and second position in std::map:
        auto iter= map_pairs.begin();
        std::advance(iter, first);
        auto first_pair= *iter;
        iter= map_pairs.begin();
        std::advance(iter, second);
        auto second_pair= *iter;

//        Fill output string:
        return_string= return_string +" "+QString::number(first_pair.first)+" " + QString::number(second_pair.first);
        if (weigthed) {
            return_string= return_string + " " + QString::number(rand()%edges)+", ";
        }
        else {
            return_string= return_string + ", ";
        }

//        Remove them from map:
        map_pairs.erase(first_pair.first);
        map_pairs.erase(second_pair.first);
    }

//    If nodes isn't divisible by 2 there is one left node in map:
    if (! is_nodes_p) {
        int left_behind= (*(map_pairs.begin())).first;
        return_string= return_string +" "+QString::number(left_behind)+" " + QString::number((left_behind+1)%nodes);
        if (weigthed) {
            return_string= return_string + " " + QString::number(rand()%edges)+", ";
        }
        else {
            return_string= return_string + ", ";
        }
    }
//    Fill in rest of return string (till edges reached):
    for (int i=nodes/2+((is_nodes_p) ? 0 : 1); i < edges; i++) {
        int first= rand() % nodes;
        int second= rand() % nodes;
        if (first == second)
            first= (second + 1) % nodes;
        return_string= return_string +" "+QString::number(first)+" " + QString::number(second);
        if (weigthed) {
            return_string= return_string + " " + QString::number(rand()%edges)+", ";
        }
        else {
            return_string= return_string + ", ";
        }
    }

    return return_string;
}

void VisualGraph::parseInput(const QString &input)
{
    if (input.isEmpty())
        return;
    QChar first_char;
    QString valid_input(input);
//    Check is random input requested (and fill valid_input):
    if (valid_input.at(0) == 'r') {
        QStringList num_of_nodes_and_edges= valid_input.mid(1).split(" ");
        int i=1;
        bool directed= false, weighted= false;
        if (num_of_nodes_and_edges.at(i).at(0) == 'd') {
            directed= true;
            i++;
        }
        if (num_of_nodes_and_edges.at(i).at(0) == 'w') {
            weighted= true;
            i++;
        }
        valid_input= generateRandomGraph(num_of_nodes_and_edges[i].toInt(), num_of_nodes_and_edges[i+1].toInt(), directed, weighted);

        if ((first_char=valid_input.at(0).toUpper()) == 'D')
            _directed= true;
        if (!first_char.isDigit())
            valid_input=valid_input.mid(1);
    }
    else {
        if ((first_char=input.at(0).toUpper()) == 'D')
            _directed= true;
        if (!first_char.isDigit())
            valid_input=input.mid(1);
    }
    QStringList edges= valid_input.split(",");
    foreach(QString edge, edges) {
        std::istringstream parse_edge(edge.toStdString());
        int first, second, third;
        parse_edge >> first >> second;
        if (parse_edge >> third) {
            addEdge(first, second, third);
            _weighted= true;
        }
        else {
            addEdge(first, second);
        }
    }
    getFRLayout(width()- Node::_graphicRadius, height() - Node::_graphicRadius);

}
void VisualGraph::setRegExpValidator(QLineEdit *&random_edit, QLineEdit *&by_hand_edit)
{
    QRegExp random_regexp("(d[ ])?(w[ ])?[0-9]+[ ][0-9]+");
    QRegExp by_hand_regexp("(d[ ])?([0-9]+[ ][0-9]+[ ]((-)?[0-9]+)?[ ]?,[ ]?)*([0-9]+[ ][0-9]+[ ]((-)?[0-9]+)?[ ]?)");
    random_edit->setValidator(new QRegExpValidator(random_regexp));
    by_hand_edit->setValidator(new QRegExpValidator(by_hand_regexp));
}

void VisualGraph::setAllNodes(int foo)
{
    GraphNodes::iterator iter = _nodes.begin();
    while(iter != _nodes.end()){
        iter.value()->setValue(foo);
        iter++;
    }
}
VisualGraph::BoostGraph VisualGraph::createBoostGraph()
{
    BoostGraph g;
    EdgeIterator iterE = _edges.begin();
    while(iterE != _edges.end()){
        add_edge(iterE.key().first,iterE.key().second,g);
        iterE++;
    }
    graphVertexIterator iterVi, iterVEnd;
    boost::tie(iterVi,iterVEnd) = vertices(g);
    while(iterVi != iterVEnd){
        if(_nodes.find(get(boost::vertex_index,g,*iterVi)) == _nodes.end()){
            remove_vertex(*iterVi, g);
        }
        ++iterVi;
    }
    return g;
}
void VisualGraph::setEnd()
{
    int temp = QInputDialog::getInt(style,"End Node","Enter last (end) node");
    if(_nodes.find(temp) == _nodes.end()){
        QMessageBox::critical(style,"Graph Error","Node doesnt exists!");
        return;
    }
    _endNode = temp;
}

void VisualGraph::save()
{
    QString foo = QInputDialog::getText(style,"Enter path","Enter path where you want to save it");
    QFile f(foo);
    if( f.open(QIODevice::WriteOnly | QIODevice::Text)){
        QTextStream out(&f);
        out << toString();
        QMessageBox::information(style,"Graph Saved", "Graph saved successfuly!");
    }
    else
        QMessageBox::critical(style,"Cant Create file","Unable to save");

}
VisualGraph::~VisualGraph()
{
    delete _rightClickMenu;
    qDeleteAll(_nodes);
    qDeleteAll(_edges);
}
