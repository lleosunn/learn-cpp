#include "check.h"
#include "tracked.h"
#include "solution.cpp"

std::vector<int> g_seen;  // written by the plain-function handler below
void record_payload(int p) { g_seen.push_back(p); }

void run_tests() {
    {
        EventBus bus;
        std::vector<int> log;
        bus.subscribe("tick", [&log](int p) { log.push_back(p); });
        bus.subscribe("tick", [&log](int p) { log.push_back(p * 10); });
        CHECK_EQ("emit returns handler count", bus.emit("tick", 4), 2u);
        CHECK_EQ("handlers run in subscription order", log, (std::vector<int>{4, 40}));
        CHECK_EQ("second emit reuses handlers", bus.emit("tick", 5), 2u);
        CHECK_EQ("log after two emits", log, (std::vector<int>{4, 40, 5, 50}));
    }
    {
        EventBus bus;
        CHECK_EQ("emit with no subscribers", bus.emit("nope", 1), 0u);
        bus.clear("nope");  // clearing an unknown event is harmless
        CHECK_EQ("emit after clearing unknown event", bus.emit("nope", 1), 0u);
    }
    {
        EventBus bus;
        int a = 0, b = 0;
        bus.subscribe("a", [&a](int p) { a += p; });
        bus.subscribe("b", [&b](int p) { b += p; });
        bus.emit("a", 3);
        bus.emit("a", 4);
        bus.emit("b", 100);
        CHECK_EQ("event a isolated", a, 7);
        CHECK_EQ("event b isolated", b, 100);
    }
    {
        EventBus bus;
        int hits = 0;
        bus.subscribe("tick", [&hits](int) { ++hits; });
        bus.emit("tick", 1);
        bus.clear("tick");
        CHECK_EQ("emit after clear returns 0", bus.emit("tick", 1), 0u);
        CHECK_EQ("cleared handler not called", hits, 1);
        bus.subscribe("tick", [&hits](int) { hits += 10; });
        bus.emit("tick", 1);
        CHECK_EQ("resubscribe after clear", hits, 11);
    }
    {
        EventBus bus;
        g_seen.clear();
        bus.subscribe("fn", record_payload);  // a plain function is a valid Handler
        bus.emit("fn", 42);
        CHECK_EQ("function pointer handler", g_seen, (std::vector<int>{42}));
    }
    {
        EventBus bus;
        int sum = 0;
        bus.subscribe("x", [&sum](int p) { sum += p; });
        const EventBus& view = bus;  // emit must be callable on a const object
        CHECK_EQ("emit through const ref", view.emit("x", 2), 1u);
        CHECK_EQ("const emit still calls handlers", sum, 2);
    }
}
