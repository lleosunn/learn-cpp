#include <cstddef>
#include <functional>
#include <map>
#include <string>
#include <utility>
#include <vector>

class EventBus {
public:
    using Handler = std::function<void(int)>;

    void subscribe(const std::string& event, Handler h) {
        handlers_[event].push_back(std::move(h));  // operator[] creates the vector on first use
    }

    std::size_t emit(const std::string& event, int payload) const {
        auto it = handlers_.find(event);           // find, because operator[] would insert
        if (it == handlers_.end()) return 0;
        for (const auto& h : it->second) h(payload);
        return it->second.size();
    }

    void clear(const std::string& event) {
        handlers_.erase(event);                    // no-op if the event was never subscribed
    }

private:
    std::map<std::string, std::vector<Handler>> handlers_;
};
