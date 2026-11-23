#include <cstddef>
#include <memory>
#include <utility>
#include <vector>

class List {
public:
    void push_front(int v) {
        auto n = std::make_unique<Node>();
        n->value = v;
        n->next = std::move(head_);   // the new node takes the old chain
        head_ = std::move(n);         // the list takes the new node
        ++size_;
    }

    void push_back(int v) {
        auto n = std::make_unique<Node>();
        n->value = v;
        if (!head_) {
            head_ = std::move(n);
        } else {
            Node* cur = head_.get();                 // raw pointer: looking, not owning
            while (cur->next) cur = cur->next.get();
            cur->next = std::move(n);
        }
        ++size_;
    }

    bool pop_front(int& out) {
        if (!head_) return false;
        out = head_->value;
        head_ = std::move(head_->next);   // old head is released and deleted; its next was already moved out
        --size_;
        return true;
    }

    std::size_t size() const { return size_; }

    std::vector<int> to_vector() const {
        std::vector<int> out;
        out.reserve(size_);
        for (const Node* cur = head_.get(); cur; cur = cur->next.get()) out.push_back(cur->value);
        return out;
    }

    void reverse() {
        std::unique_ptr<Node> prev;                // the reversed prefix
        while (head_) {
            auto next = std::move(head_->next);    // detach the rest of the list
            head_->next = std::move(prev);         // old head now points at the reversed prefix
            prev = std::move(head_);               // the prefix grows by one
            head_ = std::move(next);               // advance
        }
        head_ = std::move(prev);
    }

private:
    struct Node {
        int value;
        std::unique_ptr<Node> next;
    };
    std::unique_ptr<Node> head_;
    std::size_t size_ = 0;
    // No destructor, no copy, no move: unique_ptr supplies all of them (rule of zero).
};
