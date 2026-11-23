#include <memory>
#include <vector>

struct Node {
    Tracked tag;
    std::vector<std::shared_ptr<Node>> children;   // owning edges point down the tree
    std::weak_ptr<Node> parent;                    // the back edge does not count: no cycle
};

std::shared_ptr<Node> make_node(int value) {
    auto n = std::make_shared<Node>();
    n->tag.value = value;
    return n;
}

std::shared_ptr<Node> add_child(const std::shared_ptr<Node>& parent, int value) {
    auto c = make_node(value);
    c->parent = parent;                 // weak_ptr from shared_ptr: use_count unchanged
    parent->children.push_back(c);      // shared_ptr copy: the parent now co-owns c
    return c;
}

int depth(const std::shared_ptr<Node>& n) {
    int d = 0;
    auto cur = n;
    while (auto p = cur->parent.lock()) {   // lock(): temporary owner, empty once the parent is gone
        ++d;
        cur = p;
    }
    return d;
}

std::shared_ptr<Node> parent_of(const std::shared_ptr<Node>& n) {
    return n->parent.lock();                // empty shared_ptr for a root or an expired parent
}
