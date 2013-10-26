void bfs(){
    Queue queue;
    queue.append(currentNode);
    getNode(currentNode)->setVisited(true);
    while(!queue.empty()){
        int curr = queue.at(0);
        queue.pop_front();
        NodeList list = getNeighbours(curr);
        for (auto iter : list){
            if(!getNode(iter)->visited()){
                getNode(iter)->setVisited(true);
            }
        }
    }
}