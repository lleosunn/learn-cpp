#include <cstddef>
#include <list>
#include <optional>
#include <unordered_map>
#include <utility>
#include <vector>

template <typename K, typename V>
class LruCache {
public:
    explicit LruCache(std::size_t capacity) : cap_(capacity) {}

    std::optional<V> get(const K& key) {
        auto it = index_.find(key);
        if (it == index_.end()) return std::nullopt;
        order_.splice(order_.begin(), order_, it->second);  // move node to front; iterator stays valid
        return it->second->second;                           // copy of the value
    }

    void put(const K& key, V value) {
        auto it = index_.find(key);
        if (it != index_.end()) {                            // update: move-assign, then refresh
            it->second->second = std::move(value);
            order_.splice(order_.begin(), order_, it->second);
            return;
        }
        order_.emplace_front(key, std::move(value));         // insert: move the value into the node
        index_[key] = order_.begin();
        if (order_.size() > cap_) {
            index_.erase(order_.back().first);               // map entry first, pop_back destroys the key
            order_.pop_back();
        }
    }

    bool contains(const K& key) const { return index_.count(key) > 0; }

    std::size_t size() const { return order_.size(); }

    std::vector<K> keys_mru_first() const {
        std::vector<K> keys;
        keys.reserve(order_.size());
        for (const auto& kv : order_) keys.push_back(kv.first);
        return keys;
    }

private:
    std::size_t cap_;
    std::list<std::pair<K, V>> order_;                                            // front = MRU, back = LRU
    std::unordered_map<K, typename std::list<std::pair<K, V>>::iterator> index_;  // key -> node in order_
};
