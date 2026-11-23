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

    // Value for key if present, marking it most recently used.
    std::optional<V> get(const K& key) {
        // TODO: find in index_, splice the node to the front, return a copy of the value
        return std::nullopt;
    }

    // Insert or update. Marks key most recently used. Evicts the least recently
    // used entry when the size would exceed the capacity.
    void put(const K& key, V value) {
        // TODO: existing key -> assign (std::move!) and splice to the front
        //       new key      -> emplace_front, record the iterator, evict if over cap_
    }

    // True if key is cached. Does not change recency.
    bool contains(const K& key) const {
        // TODO
        return false;
    }

    std::size_t size() const {
        // TODO
        return 0;
    }

    // Keys from most to least recently used.
    std::vector<K> keys_mru_first() const {
        // TODO: walk order_ from front to back
        return {};
    }

private:
    std::size_t cap_;
    std::list<std::pair<K, V>> order_;                                            // front = MRU, back = LRU
    std::unordered_map<K, typename std::list<std::pair<K, V>>::iterator> index_;  // key -> node in order_
};
