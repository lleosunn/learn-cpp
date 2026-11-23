#include <memory>
#include <vector>

struct Node {
    Tracked tag;                                    // tag.value is the node's value
    std::vector<std::shared_ptr<Node>> children;    // owns the children
    std::shared_ptr<Node> parent;                   // TODO: why does this leak? (implement the functions first, then come back)
};

// A new root node whose tag.value is value.
std::shared_ptr<Node> make_node(int value) {
    // TODO: make_shared, then set tag.value
    return nullptr;
}

// Create a child of parent, link it both ways, return it.
std::shared_ptr<Node> add_child(const std::shared_ptr<Node>& parent, int value) {
    // TODO
    return nullptr;
}

// Number of parent links between n and its root (root -> 0).
int depth(const std::shared_ptr<Node>& n) {
    // TODO: walk upward, counting
    return 0;
}

// n's parent, or nullptr if n is a root or its parent no longer exists.
std::shared_ptr<Node> parent_of(const std::shared_ptr<Node>& n) {
    // TODO
    return nullptr;
}
