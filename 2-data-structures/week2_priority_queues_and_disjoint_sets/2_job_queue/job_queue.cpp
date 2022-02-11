#include <algorithm>
#include <concepts>
#include <iostream>
#include <vector>

template<class T>
concept comparable = requires(T& a, T& b)
{
    a < b;
};

class ThreadNode
{
private:
    uint64_t mFinishTime;
    uint32_t mThreadId;

public:
    ThreadNode(uint64_t tFinishTime, uint32_t tThreadId)
        : mFinishTime{ tFinishTime }
        , mThreadId{ tThreadId }
    {}

    ThreadNode(uint32_t tThreadId)
        : mFinishTime{ 0 }
        , mThreadId{ tThreadId }
    {}

    bool operator<(const ThreadNode& n2) const
    {
        // If time is equal, compare by thread id
        if (this->mFinishTime == n2.mFinishTime)
            return this->mThreadId < n2.mThreadId;
        return this->mFinishTime < n2.mFinishTime;
    }

    bool operator>(const ThreadNode& n2) const
    {
        return n2.operator<(*this);
    }

    void SetPriority(uint64_t tNewPriority)
    {
        mFinishTime = tNewPriority;
    }

    uint64_t GetPriority()
    {
        return mFinishTime;
    }

    uint32_t GetThreadId()
    {
        return mThreadId;
    }
};

template<comparable T>
class MinHeap
{
private:
    std::vector<T> mData;

    int32_t GetParent(int32_t nodeIdx)
    {
        return nodeIdx / 2;
    }

    int32_t GetLeftChild(int32_t nodeIdx)
    {
        return 2 * nodeIdx + 1;
    }

    int32_t GetRightChild(int32_t nodeIdx)
    {
        return 2 * nodeIdx + 2;
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
                std::swap(mData[curMinIndex], mData[nodeIdx]);
                nodeIdx = curMinIndex;
            }
        }
    }

    void SiftUp(int32_t nodeIdx)
    {
        while (nodeIdx > 0 && mData[GetParent(nodeIdx)] < mData[nodeIdx]) {
            std::swap(mData[GetParent(nodeIdx)], mData[nodeIdx]);
            nodeIdx = GetParent(nodeIdx);
        }
    }

    void BuildHeap()
    {
        for (int32_t i = mData.size() / 2; i >= 0; i--) {
            SiftDownIterative(i);
        }
    }

public:
    MinHeap(std::vector<T>& tData)
        : mData(tData)
    {
        BuildHeap();
    }

    T GetMin()
    {
        return mData.at(0);
    }

    void ChangePriority(uint32_t tNodeIdx, uint64_t newPriority)
    {
        uint64_t oldPriority = mData.at(tNodeIdx).GetPriority();
        mData.at(tNodeIdx).SetPriority(newPriority);

        if (newPriority < oldPriority) {
            SiftUp(tNodeIdx);
            return;
        }

        SiftDownIterative(tNodeIdx);
    }
};

class JobQueue
{
private:
    int32_t mTotalThreads;
    int32_t mTotalJobs;

    std::vector<ThreadNode> mThreads;
    std::vector<int32_t> mJobs;

    std::vector<int32_t> mAssignedWorkers;
    std::vector<uint64_t> mStartTimes;

    void WriteResponse() const
    {
        for (int32_t i = 0; i < mTotalJobs; ++i) {
            std::cout << mAssignedWorkers[i] << " " << mStartTimes[i] << "\n";
        }
    }

    void ReadData()
    {
        std::cin >> mTotalThreads >> mTotalJobs;

        mThreads.reserve(mTotalThreads);
        for (uint32_t threadId = 0; threadId < mTotalThreads; threadId++)
            mThreads.emplace_back(threadId);

        mJobs.resize(mTotalJobs);
        for (int32_t i = 0; i < mTotalJobs; ++i)
            std::cin >> mJobs[i];
    }

    void AssignJobs()
    {
        mAssignedWorkers.resize(mJobs.size());
        mStartTimes.resize(mJobs.size());

        MinHeap<ThreadNode> minHeap{ mThreads };

        for (uint32_t i = 0; i < mJobs.size(); i++) {
            ThreadNode thread = minHeap.GetMin();

            mAssignedWorkers[i] = thread.GetThreadId();
            mStartTimes[i] = thread.GetPriority();

            minHeap.ChangePriority(0, mJobs[i] + thread.GetPriority());
        }
    }

public:
    void Solve()
    {
        ReadData();
        AssignJobs();
        WriteResponse();
    }
};

int32_t main()
{
    std::ios_base::sync_with_stdio(false);
    JobQueue job_queue;
    job_queue.Solve();
    return 0;
}
