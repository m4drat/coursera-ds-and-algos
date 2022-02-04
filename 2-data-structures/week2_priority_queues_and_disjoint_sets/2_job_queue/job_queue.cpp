#include <algorithm>
#include <iostream>
#include <vector>

class JobQueue
{
private:
    int32_t mNumWorkers;
    std::vector<int32_t> mJobs;

    std::vector<int32_t> mAssignedWorkers;
    std::vector<uint64_t> mStartTimes;

    void WriteResponse() const
    {
        for (int32_t i = 0; i < mJobs.size(); ++i) {
            std::cout << mAssignedWorkers[i] << " " << mStartTimes[i] << "\n";
        }
    }

    void ReadData()
    {
        int32_t m;
        std::cin >> mNumWorkers >> m;
        mJobs.resize(m);
        for (int32_t i = 0; i < m; ++i)
            std::cin >> mJobs[i];
    }

    void AssignJobs()
    {
        // TODO: replace this code with a faster algorithm.
        mAssignedWorkers.resize(mJobs.size());
        mStartTimes.resize(mJobs.size());
        std::vector<uint64_t> mNextFreeTime(mNumWorkers, 0);
        for (int32_t i = 0; i < mJobs.size(); ++i) {
            int32_t duration = mJobs[i];
            int32_t next_worker = 0;
            for (int32_t j = 0; j < mNumWorkers; ++j) {
                if (mNextFreeTime[j] < mNextFreeTime[next_worker])
                    next_worker = j;
            }
            mAssignedWorkers[i] = next_worker;
            mStartTimes[i] = mNextFreeTime[next_worker];
            mNextFreeTime[next_worker] += duration;
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
