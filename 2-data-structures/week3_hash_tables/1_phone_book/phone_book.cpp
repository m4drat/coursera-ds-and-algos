#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

struct Query
{
    std::string type, name;
    int32_t number;
};

template<class T>
class DirectAddressingHashMap
{
private:
    std::vector<T> mElements;

public:
    DirectAddressingHashMap(uint32_t tTotalEntries)
        : mElements{}
    {
        mElements.resize(tTotalEntries);
    }

    T& operator[](std::size_t idx)
    {
        return mElements[idx];
    }
};

void ProcessQueries()
{
    int32_t n;
    std::cin >> n;
    Query query;

    DirectAddressingHashMap<std::unique_ptr<std::string>> hashMap{ 10'000'000 };
    std::vector<std::string> results;

    for (int32_t i = 0; i < n; ++i) {
        std::cin >> query.type;
        if (query.type == "add") {
            std::cin >> query.number >> query.name;
            hashMap[query.number] = std::make_unique<std::string>(query.name);
        } else if (query.type == "del") {
            std::cin >> query.number;
            hashMap[query.number] = nullptr;
        } else if (query.type == "find") {
            std::cin >> query.number;
            if (hashMap[query.number]) {
                results.emplace_back(*hashMap[query.number]);
            } else {
                results.emplace_back("not found");
            }
        }
    }

    for (const auto& str : results) {
        std::cout << str << '\n';
    }
}

int main()
{
    ProcessQueries();
    return 0;
}
