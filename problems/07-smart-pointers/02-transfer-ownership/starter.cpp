#include <cstddef>
#include <memory>
#include <utility>
#include <vector>

class Tray {
public:
    // Take ownership of item. After the call the caller's pointer is empty.
    void put(std::unique_ptr<Tracked> item) {
        // TODO: store it in items_. items_.push_back(item) will not compile; read the error.
    }

    // Hand back the most recently put item, or nullptr if the tray is empty.
    std::unique_ptr<Tracked> take() {
        // TODO
        return nullptr;
    }

    std::size_t size() const {
        // TODO
        return 0;
    }

private:
    std::vector<std::unique_ptr<Tracked>> items_;
};
