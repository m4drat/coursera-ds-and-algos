#include <cassert>
#include <iostream>
#include <queue>
#include <vector>

struct Request
{
    Request(int32_t arrivalTime, int32_t processTime)
        : arrivalTime(arrivalTime)
        , processTime(processTime)
    {}

    int32_t arrivalTime;
    int32_t processTime;
};

struct Response
{
    Response(bool dropped, int32_t startTime)
        : dropped(dropped)
        , startTime(startTime)
    {}

    bool dropped;
    int32_t startTime;
};

class Buffer
{
public:
    Buffer(int32_t size)
        : mFinishTime{}
        , mBufferSize{ size }
    {}

    /**
     * @brief Simulates packets processing
     * @param request - request to process
     * @return Response - result of request processing
     */
    Response Process(const Request& request)
    {
        // If queue is not empty - pop every packet that is already processed
        // Which request.arrivalTime > mFinishTime.front()
        for (int32_t finishTime = mFinishTime.front();
             finishTime <= request.arrivalTime && !mFinishTime.empty();
             finishTime = mFinishTime.front()) {
            mFinishTime.pop();
        }

        // If we don't have enough space in the buffer - the packet is dropped
        if (mFinishTime.size() >= mBufferSize)
            return Response{ true, request.arrivalTime };

        // If mFinishTime is empty - we start to process the packet immediately
        if (mFinishTime.empty()) {
            mFinishTime.push(request.arrivalTime + request.processTime);
            return Response{ false, request.arrivalTime };
        }

        // Lastly - we have to process current packet right after
        // we process everything from the buffer
        auto curPacketProcessingStartTime = mFinishTime.back();
        mFinishTime.push(curPacketProcessingStartTime + request.processTime);
        return Response{ false, curPacketProcessingStartTime };
    }

private:
    std::queue<int32_t> mFinishTime;
    int32_t mBufferSize;
};

std::vector<Request> ReadRequests()
{
    int32_t count;
    std::cin >> count;

    std::vector<Request> requests;
    requests.reserve(count);

    for (int32_t i = 0; i < count; ++i) {
        int32_t arrivalTime, processTime;
        std::cin >> arrivalTime >> processTime;
        requests.push_back(Request(arrivalTime, processTime));
    }
    return requests;
}

std::vector<Response> ProcessRequests(const std::vector<Request>& requests, Buffer* buffer)
{
    std::vector<Response> responses;
    responses.reserve(requests.size());

    for (int32_t i = 0; i < requests.size(); ++i)
        responses.push_back(buffer->Process(requests[i]));
    return responses;
}

void PrintResponses(const std::vector<Response>& responses)
{
    for (int32_t i = 0; i < responses.size(); ++i)
        std::cout << (responses[i].dropped ? -1 : responses[i].startTime) << std::endl;
}

int32_t main()
{
    int32_t size;
    std::cin >> size;
    std::vector<Request> requests = ReadRequests();

    Buffer buffer(size);
    std::vector<Response> responses = ProcessRequests(requests, &buffer);

    PrintResponses(responses);
    return 0;
}
