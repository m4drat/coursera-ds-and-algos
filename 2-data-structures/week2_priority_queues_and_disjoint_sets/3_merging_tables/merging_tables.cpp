#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <vector>

struct DisjointSetsElement
{
    int32_t mSize, parent, rank;

    DisjointSetsElement(int32_t mSize = 0, int32_t parent = -1, int32_t rank = 0)
        : mSize(mSize)
        , parent(parent)
        , rank(rank)
    {}
};

struct DisjointSets
{
    int32_t mSize;
    int32_t mMaxTableSize;
    std::vector<DisjointSetsElement> mSets;

    DisjointSets(int32_t mSize)
        : mSize(mSize)
        , mMaxTableSize(0)
        , mSets(mSize)
    {
        for (int32_t i = 0; i < mSize; i++)
            mSets[i].parent = i;
    }

    int32_t getParent(int32_t table)
    {
        // find parent and compress path
    }

    void merge(int32_t destination, int32_t source)
    {
        int32_t realDestination = getParent(destination);
        int32_t realSource = getParent(source);
        if (realDestination != realSource) {
            // merge two components
            // use union by rank heuristic
            // update mMaxTableSize
        }
    }
};

int32_t main()
{
    int32_t n, m;
    std::cin >> n >> m;

    DisjointSets tables(n);
    for (auto& table : tables.mSets) {
        std::cin >> table.mSize;
        tables.mMaxTableSize = std::max(tables.mMaxTableSize, table.mSize);
    }

    for (int32_t i = 0; i < m; i++) {
        int32_t destination, source;
        std::cin >> destination >> source;
        --destination;
        --source;

        tables.merge(destination, source);
        std::cout << tables.mMaxTableSize << std::endl;
    }

    return 0;
}
