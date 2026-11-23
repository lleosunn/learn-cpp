#include <vector>

// Return the sum of item.value over all items.
// Tracked is already defined. This version gets the right number...
// but run the tests and look at Tracked::copies.
int sum_values(std::vector<Tracked> items) {
    int s = 0;
    for (Tracked t : items) s += t.value;
    return s;
}
