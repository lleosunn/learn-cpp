#include "check.h"
#include "tracked.h"
#include "solution.cpp"

void run_tests() {
    {
        LruCache<int, int> c(2);  // LeetCode 146 trace
        c.put(1, 1);
        c.put(2, 2);
        CHECK_EQ("get(1) after put 1,2", c.get(1).value_or(-1), 1);
        c.put(3, 3);  // evicts 2: 1 was just used
        CHECK_TRUE("get(2) evicted", !c.get(2).has_value());
        c.put(4, 4);  // evicts 1
        CHECK_TRUE("get(1) evicted", !c.get(1).has_value());
        CHECK_EQ("get(3)", c.get(3).value_or(-1), 3);
        CHECK_EQ("get(4)", c.get(4).value_or(-1), 4);
        CHECK_EQ("keys MRU first", c.keys_mru_first(), (std::vector<int>{4, 3}));
        CHECK_EQ("size at capacity", c.size(), 2u);
    }
    {
        LruCache<int, int> c(3);
        c.put(1, 1);
        c.put(2, 2);
        c.put(3, 3);
        c.put(1, 10);  // update existing key: new value, moved to front, no growth
        CHECK_EQ("updated value", c.get(1).value_or(-1), 10);
        CHECK_EQ("update keeps size", c.size(), 3u);
        CHECK_EQ("update moves key to front", c.keys_mru_first(), (std::vector<int>{1, 3, 2}));
        c.put(4, 4);  // evicts 2, the LRU
        CHECK_TRUE("LRU after update evicted", !c.contains(2));
        CHECK_EQ("keys after eviction", c.keys_mru_first(), (std::vector<int>{4, 1, 3}));
    }
    {
        LruCache<int, int> c(2);
        c.put(1, 1);
        c.put(2, 2);
        CHECK_TRUE("contains(1)", c.contains(1));
        CHECK_TRUE("!contains(9)", !c.contains(9));
        c.put(3, 3);  // contains() did not refresh 1, so 1 is the one evicted
        CHECK_EQ("contains does not change recency", c.keys_mru_first(), (std::vector<int>{3, 2}));
    }
    {
        LruCache<int, int> c(1);
        c.put(1, 1);
        c.put(2, 2);
        CHECK_TRUE("cap 1: first evicted", !c.contains(1));
        CHECK_EQ("cap 1: get(2)", c.get(2).value_or(-1), 2);
        CHECK_EQ("cap 1: size", c.size(), 1u);
    }
    {
        LruCache<std::string, std::string> c(2);
        c.put("a", "apple");
        c.put("b", "banana");
        CHECK_EQ("string cache get", c.get("a").value_or(""), std::string("apple"));
        CHECK_EQ("string cache miss", c.get("zz").value_or("<miss>"), std::string("<miss>"));
        c.put("c", "cherry");  // evicts b: a was refreshed by get
        CHECK_EQ("string cache keys", c.keys_mru_first(), (std::vector<std::string>{"c", "a"}));
    }
    {
        Tracked::reset();
        {
            LruCache<int, Tracked> c(2);
            c.put(1, Tracked(1));
            c.put(2, Tracked(2));
            c.put(3, Tracked(3));  // evicts Tracked(1): it must be destroyed now
            CHECK_EQ("evicted value destroyed", Tracked::alive, 2);
            CHECK_EQ("put moves, never copies", Tracked::copies, 0);
            c.put(2, Tracked(20));  // the update path must move too
            CHECK_EQ("update does not copy", Tracked::copies, 0);
            CHECK_EQ("update does not leak", Tracked::alive, 2);
        }
        CHECK_EQ("all values destroyed with the cache", Tracked::alive, 0);
    }
}
