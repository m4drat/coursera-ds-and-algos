#include <algorithm>
#include <cassert>
#include <iostream>
#include <map>
#include <vector>

struct Item
{
    double value;
    double weight;

    Item()
        : value{ 0 }
        , weight{ 0 }
    {}

    Item(double t_Value, double t_Weight)
        : value{ t_Value }
        , weight{ t_Weight }
    {}

    double ValuePerUnitOfWeight(double t_Weight)
    {
        return (value / weight) * t_Weight;
    }
};

double GetOptimalValue(uint32_t capacity, std::vector<Item>& items)
{
    double value = 0.0;

    // 1. Sort the input array of items
    std::sort(items.begin(), items.end(), [](const Item& a, const Item& b) {
        return (a.value / a.weight) > (b.value / b.weight);
    });

    // Pick the most profitable item
    auto ptr = items.begin();
    while (ptr != items.end()) {
        // If capacity is zero -> knapsack is full
        if (capacity == 0) {
            return value;
        }

        // Try to get as much as possible of the most valuable item
        uint32_t amount = std::min(static_cast<double>(capacity), ptr->weight);

        value += ptr->ValuePerUnitOfWeight(amount);
        capacity -= amount;

        // ptr->weight -= amount;

        // Pick next most profitable item
        ptr++;
    }

    return value;
}

int main(int argc, char* argv[])
{
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
