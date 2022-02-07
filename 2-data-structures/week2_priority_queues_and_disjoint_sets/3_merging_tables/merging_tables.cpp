#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <vector>

class Table
{
public:
    int32_t mSize, mParent, mRank;
    Table(int32_t tSize = 0, int32_t tParent = -1, int32_t tRank = 0)
        : mSize(tSize)
        , mParent(tParent)
        , mRank(tRank)
    {}
};

class DisjointSets
{
public:
    int32_t mMaxTableSize;
    std::vector<Table> mTables;

    DisjointSets(int32_t tTotalTables)
        : mMaxTableSize(0)
        , mTables(tTotalTables)
    {
        for (int32_t i = 0; i < tTotalTables; i++)
            mTables[i].mParent = i;
    }

    int32_t GetParent(int32_t tTable)
    {
        if (tTable == mTables[tTable].mParent) {
            return tTable;
        }

        mTables[tTable].mParent = GetParent(mTables[tTable].mParent);

        return mTables[tTable].mParent;
    }

    void Merge(int32_t tDestination, int32_t tSource)
    {
        int32_t realDestination = GetParent(tDestination);
        int32_t realSource = GetParent(tSource);

        // If src == dst -> two tables are already merged
        // We don't need to update mMaxTableSize
        if (realDestination == realSource)
            return;

        // If rank (height) of the real destination is higher,
        // attach mTables[realDestination] to mTables[realSource]
        // This implements union-by-rank heuristic, that helps
        // to keep the tree shallow
        if (mTables[realDestination].mRank > mTables[realSource].mRank) {
            mTables[realSource].mParent = realDestination;

            mTables[realDestination].mSize += mTables[realSource].mSize;
            mTables[realSource].mSize = 0;
        } else {
            mTables[realDestination].mParent = realSource;

            mTables[realSource].mSize += mTables[realDestination].mSize;
            mTables[realDestination].mSize = 0;

            if (mTables[realDestination].mRank == mTables[realSource].mRank) {
                mTables[realSource].mRank++;
            }
        }

        // Update mMaxTableSize
        mMaxTableSize =
            std::max(mTables[realDestination].mSize + mTables[realSource].mSize, mMaxTableSize);
    }
};

int32_t main()
{
    int32_t totalTables, totalMergeRequests;
    std::cin >> totalTables >> totalMergeRequests;

    DisjointSets tables(totalTables);
    for (auto& table : tables.mTables) {
        std::cin >> table.mSize;
        tables.mMaxTableSize = std::max(tables.mMaxTableSize, table.mSize);
    }

    for (int32_t i = 0; i < totalMergeRequests; i++) {
        int32_t destination, source;
        std::cin >> destination >> source;

        tables.Merge(destination - 1, source - 1);
        std::cout << tables.mMaxTableSize << std::endl;
    }

    return 0;
}
