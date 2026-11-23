#include <cstddef>
#include <memory>
#include <utility>
#include <vector>

class List {
public:
    void push_front(int v) {
        // TODO: new node takes the old chain, list takes the new node
    }

    void push_back(int v) {
        // TODO: walk to the last node with a raw Node*, move the new node into its next
    }

    // Remove the first element into out. Returns false (and leaves out alone) if the list is empty.
    bool pop_front(int& out) {
        // TODO
        return false;
    }

    std::size_t size() const { return size_; }

    std::vector<int> to_vector() const {
        // TODO
        return {};
    }

    void reverse() {
        // TODO
    }

private:
    struct Node {
        int value;
        std::unique_ptr<Node> next;   // each node owns its successor
    };
    std::unique_ptr<Node> head_;      // the list owns the first node
    std::size_t size_ = 0;
};
