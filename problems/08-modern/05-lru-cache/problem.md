# Capstone: Generic LRU Cache

`functools.lru_cache` is one decorator in Python. Here you build the data structure underneath it, using everything from this chapter: a class template, `std::optional` returns, move semantics, and two containers that must stay in sync.

## The lesson

An LRU cache holds at most `capacity` entries and, when full, evicts the one used least recently. Both `get` and `put` must be O(1), which rules out scanning for the oldest entry. The classic structure is two containers pointing at each other:

```text
order_  (std::list<pair<K,V>>)         front = most recently used
        +-------+     +-------+     +-------+
        | 4 : d | <-> | 3 : c | <-> | 1 : a |   back = least recently used, evicted first
        +-------+     +-------+     +-------+
            ^             ^             ^
index_  (unordered_map<K, list iterator>)
        4 ----+           |             |
        3 ----------------+             |
        1 ------------------------------+
```

- **`std::list`** is a doubly linked list. Its selling point is not speed (traversal is slow) but **iterator stability**: an iterator to a node stays valid no matter what happens elsewhere in the list, and `splice` moves a node to the front in O(1) without copying or invalidating anything. A `std::vector` cannot offer that; inserting or erasing shifts elements and invalidates iterators. This is the one place `list` beats `vector`.
- **`std::unordered_map<K, iterator>`** gives O(1) lookup from a key to its node. The mapped value *is* the list iterator.

The two operations that matter:

```cpp
// get: look up, move to front, return a copy of the value
auto it = index_.find(key);
if (it == index_.end()) return std::nullopt;
order_.splice(order_.begin(), order_, it->second);  // it->second is still valid afterwards
return it->second->second;                           // map entry -> list iterator -> pair -> value

// put, new key: push to front, record the iterator, evict if over capacity
order_.emplace_front(key, std::move(value));
index_[key] = order_.begin();
if (order_.size() > cap_) {
    index_.erase(order_.back().first);  // map first: pop_back destroys the key you need
    order_.pop_back();
}
```

The containers must agree at all times. The classic bug is removing a node from the list but leaving its iterator in the map: the next `get` of that key dereferences freed memory and AddressSanitizer reports `heap-use-after-free`. Every removal is two lines, always together.

**Move the values.** `put` takes `V value` by value so callers can pass temporaries; inside, `std::move(value)` hands the payload to the list node instead of copying it. The `Tracked` test counts copies, and the correct solution makes zero during `put`. `get` returns `std::optional<V>` by copy, because the cache keeps its own entry.

**Class templates.** `template <typename K, typename V> class LruCache` is one definition, instantiated separately for `LruCache<int, int>`, `LruCache<std::string, std::string>`, and so on. Inside the class, `K` and `V` are ordinary types. The one new piece of syntax: a nested type that depends on a template parameter needs the `typename` keyword in front, as in `typename std::list<std::pair<K, V>>::iterator`. It is already written in the skeleton.

## Coming from Python

- `OrderedDict` with `move_to_end(key, last=False)` and `popitem()` is this same structure behind a dict interface. Here you hold both halves yourself.
- `functools.lru_cache(maxsize=2)` wraps a function; this class is only the cache. Wrapping a function on top is a lambda away.
- Python dicts iterate in insertion order; `std::unordered_map` iteration order is unspecified. That is exactly why recency lives in the list, not in the map.
- `cache.get(k)` returning `None` is `std::optional<V>`.

## Your task

Implement the class template. The private members are declared for you.

```cpp
template <typename K, typename V>
class LruCache {
public:
    explicit LruCache(std::size_t capacity);  // capacity >= 1
    std::optional<V> get(const K& key);       // value if present; marks it most recently used
    void put(const K& key, V value);          // insert or update; marks MRU; evicts LRU when over capacity
    bool contains(const K& key) const;        // no recency change
    std::size_t size() const;
    std::vector<K> keys_mru_first() const;    // keys from most to least recently used

private:
    std::size_t cap_;
    std::list<std::pair<K, V>> order_;                                           // front = MRU
    std::unordered_map<K, typename std::list<std::pair<K, V>>::iterator> index_;
};
```

| sequence (capacity 2) | result |
|---|---|
| `put(1,1) put(2,2) get(1)` | `1`; order is now `[1, 2]` |
| `put(3,3)` | evicts `2`; `get(2)` is `nullopt` |
| `put(4,4)` | evicts `1`; `get(3)` is `3`, `get(4)` is `4` |
| `keys_mru_first()` | `{4, 3}` |
