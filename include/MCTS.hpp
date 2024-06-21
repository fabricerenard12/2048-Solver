#include <memory>

using NodePtr = std::shared_ptr<Node>

struct Node {
    NodePtr parent;
    std::vector<NodePtr> children;

}

class MCTS {

}

