#include <cstddef>
#include <functional>
#include <map>
#include <string>
#include <vector>

class EventBus {
public:
    using Handler = std::function<void(int)>;

    // Append h to the list of handlers for `event`.
    void subscribe(const std::string& event, Handler h) {
        // TODO
    }

    // Call every handler subscribed to `event` with `payload`, in subscription
    // order. Return how many were called. Must not modify handlers_ (it is const).
    std::size_t emit(const std::string& event, int payload) const {
        // TODO: find, not operator[]
        return 0;
    }

    // Remove all handlers for `event`.
    void clear(const std::string& event) {
        // TODO
    }

private:
    std::map<std::string, std::vector<Handler>> handlers_;
};
