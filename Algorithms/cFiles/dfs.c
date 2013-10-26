void (int index_of_current_node)
{
    Node * node = getNode(index_of_current_node);
    node->setVisited(true);
    NodeList neighbours_list = getNeighbours(index_of_current_node);
    foreach(int curr_node, neighbours_list) {
        if (!getNode(curr_node)->visited()) {
            dfs(curr_node);
        }
    }
}
