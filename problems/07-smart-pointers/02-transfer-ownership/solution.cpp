#include <cstddef>
#include <memory>
#include <utility>
#include <vector>

class Tray {
public:
    void put(std::unique_ptr<Tracked> item) {
        items_.push_back(std::move(item));     // item is an lvalue here; the cast selects the move
    }

    std::unique_ptr<Tracked> take() {
        if (items_.empty()) return nullptr;
        auto out = std::move(items_.back());   // steal the pointer out of the last slot
        items_.pop_back();                     // remove the now-null slot
        return out;                            // returning a local: moved (or elided) automatically
    }

    std::size_t size() const { return items_.size(); }

private:
    std::vector<std::unique_ptr<Tracked>> items_;
};
