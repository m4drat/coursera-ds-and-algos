#include <iostream>
#include <vector>
#include <algorithm>
#include <cassert>
#include <map>

struct Item {
    uint32_t value;
    uint32_t weight;

    Item()
      : value{ 0 }, weight{ 0 } {}

    Item(uint32_t t_Value, uint32_t t_Weight):
      value{ t_Value }, weight{ t_Weight } {}

    float ValuePerUnitOfWeight(uint32_t t_Weight) {
        return ((float)value / (float)weight) * (float)t_Weight;
    }
};

double GetOptimalValue(uint32_t capacity, std::vector<Item>& items) {
    double value = 0.0;

    // 1. Sort the input array of items
    std::sort(items.begin(), items.end(), [](const Item& a, const Item& b) {
        return ((float)a.value / (float)a.weight) > ((float)b.value / (float)b.weight); 
    });

    // Pick the most profitable item
    auto ptr = items.begin();
    while (ptr != items.end()) {
        // If capacity is zero -> knapsack is full
        if (capacity == 0) {
            return value;
        }

        // Try to get as much as possible of the most valuable item
        uint32_t amount = std::min(capacity, ptr->weight);

        value += ptr->ValuePerUnitOfWeight(amount);
        capacity -= amount;

        // ptr->weight -= amount;

        // Pick next most profitable item
        ptr++;
    }

    return value;
}

int main(int argc, char* argv[]) {
    uint32_t n;
    uint32_t capacity;
    std::cin >> n >> capacity;

    std::vector<Item> items(n);

    for (int i = 0; i < n; i++) {
        uint32_t value;
        uint32_t weight;

        std::cin >> value >> weight;

        items[i] = std::move(Item(value, weight));
    }

    double optimalValue = GetOptimalValue(capacity, items);

    std::cout.precision(10);
    std::cout << optimalValue << std::endl;
    return 0;
}
