#include <algorithm>
#include <iostream>
#include <vector>

class HeapBuilder
{
private:
    std::vector<int32_t> mData;
    std::vector<std::pair<int32_t, int32_t>> mSwaps;

    void WriteResponse() const
    {
        std::cout << mSwaps.size() << "\n";
        for (int32_t i = 0; i < mSwaps.size(); ++i) {
            std::cout << mSwaps[i].first << " " << mSwaps[i].second << "\n";
        }
    }

    void ReadData()
    {
        int32_t n;
        std::cin >> n;
        mData.resize(n);

        for (int32_t i = 0; i < n; ++i)
            std::cin >> mData[i];
    }

    int32_t GetLeftChild(int32_t nodeIdx)
    {
        return 2 * nodeIdx + 1;
    }

    int32_t GetRightChild(int32_t nodeIdx)
    {
        return 2 * nodeIdx + 2;
    }

    void SiftDownRecursive(int32_t nodeIdx)
    {
        uint32_t curMinIndex = nodeIdx;

        int32_t leftChildIdx = GetLeftChild(nodeIdx);
        if (leftChildIdx < mData.size() && mData[leftChildIdx] < mData[curMinIndex]) {
            curMinIndex = leftChildIdx;
        }

        int32_t rightChildIdx = GetRightChild(nodeIdx);
        if (rightChildIdx < mData.size() && mData[rightChildIdx] < mData[curMinIndex]) {
            curMinIndex = rightChildIdx;
        }

        if (nodeIdx != curMinIndex) {
            mSwaps.push_back(std::make_pair(curMinIndex, nodeIdx));
            std::swap(mData[curMinIndex], mData[nodeIdx]);
            SiftDownRecursive(curMinIndex);
        }
    }

    void SiftDownIterative(int32_t nodeIdx)
    {
        uint32_t curMinIndex = 0;

        // While current node idx is less than heap size - continue
        while (nodeIdx < mData.size()) {
            curMinIndex = nodeIdx;

            int32_t leftChildIdx = GetLeftChild(nodeIdx);
            if (leftChildIdx < mData.size() && mData[leftChildIdx] < mData[curMinIndex]) {
                curMinIndex = leftChildIdx;
            }

            int32_t rightChildIdx = GetRightChild(nodeIdx);
            if (rightChildIdx < mData.size() && mData[rightChildIdx] < mData[curMinIndex]) {
                curMinIndex = rightChildIdx;
            }

            // If after checks, we can't find any child, whose value is less
            // than the current node value (nodeIdx) - break
            if (curMinIndex == nodeIdx) {
                break;
            }

            if (nodeIdx != curMinIndex) {
                mSwaps.push_back(std::make_pair(curMinIndex, nodeIdx));
                std::swap(mData[curMinIndex], mData[nodeIdx]);
                nodeIdx = curMinIndex;
            }
        }
    }

    void BuildHeap()
    {
        for (int32_t i = mData.size() / 2; i >= 0; i--) {
            SiftDownIterative(i);
        }
    }

public:
    HeapBuilder()
        : mData{}
        , mSwaps{}
    {}

    void Solve()
    {
        ReadData();
        BuildHeap();
        WriteResponse();
    }
};

int32_t main()
{
    std::ios_base::sync_with_stdio(false);
    HeapBuilder heap_builder{};
    heap_builder.Solve();
    return 0;
}
