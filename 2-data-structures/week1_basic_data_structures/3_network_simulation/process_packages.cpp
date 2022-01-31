#include <iostream>
#include <queue>
#include <vector>

struct Request
{
    Request(int32_t arrival_time, int32_t process_time)
        : arrival_time(arrival_time)
        , process_time(process_time)
    {}

    int32_t arrival_time;
    int32_t process_time;
};

struct Response
{
    Response(bool dropped, int32_t start_time)
        : dropped(dropped)
        , start_time(start_time)
    {}

    bool dropped;
    int32_t start_time;
};

class Buffer
{
public:
    Buffer(int32_t size)
        : size_(size)
        , finish_time_()
    {}

    Response Process(const Request& request)
    {
        // write your code here
    }

private:
    int32_t size_;
    std::queue<int32_t> finish_time_;
};

std::vector<Request> ReadRequests()
{
    std::vector<Request> requests;
    int32_t count;
    std::cin >> count;
    for (int32_t i = 0; i < count; ++i) {
        int32_t arrival_time, process_time;
        std::cin >> arrival_time >> process_time;
        requests.push_back(Request(arrival_time, process_time));
    }
    return requests;
}

std::vector<Response> ProcessRequests(const std::vector<Request>& requests, Buffer* buffer)
{
    std::vector<Response> responses;
    for (int32_t i = 0; i < requests.size(); ++i)
        responses.push_back(buffer->Process(requests[i]));
    return responses;
}

void PrintResponses(const std::vector<Response>& responses)
{
    for (int32_t i = 0; i < responses.size(); ++i)
        std::cout << (responses[i].dropped ? -1 : responses[i].start_time) << std::endl;
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
