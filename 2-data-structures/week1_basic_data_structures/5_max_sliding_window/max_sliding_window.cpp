#include <iostream>
#include <vector>

void MaxSlidingWindowNaive(std::vector<int32_t> const& A, int32_t w)
{
    for (size_t i = 0; i < A.size() - w + 1; ++i) {
        int32_t window_max = A.at(i);
        for (size_t j = i + 1; j < i + w; ++j)
            window_max = std::max(window_max, A.at(j));

        std::cout << window_max << " ";
    }

    return;
}

void MaxSlidingWindow(std::vector<int32_t> const& array, int32_t m)
{

    return;
}

int32_t main()
{
    int32_t n = 0;
    std::cin >> n;

    std::vector<int32_t> A(n);
    for (size_t i = 0; i < n; ++i)
        std::cin >> A.at(i);

    int32_t m = 0;
    std::cin >> m;

    MaxSlidingWindowNaive(A, m);

    return 0;
}
