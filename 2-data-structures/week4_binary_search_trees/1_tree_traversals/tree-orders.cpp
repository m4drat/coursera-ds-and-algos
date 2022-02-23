#include <algorithm>
#include <iostream>
#include <vector>
#if defined(__unix__) || defined(__APPLE__)
#include <sys/resource.h>
#endif

class TreeOrders
{
private:
    int32_t n;
    std::vector<int32_t> key;
    std::vector<int32_t> left;
    std::vector<int32_t> right;

    const int32_t cNullElem = -1;

    void InOrderImpl(int32_t root, std::vector<int32_t>& data)
    {
        if (root == cNullElem)
            return;

        InOrderImpl(left[root], data);
        data.push_back(key[root]);
        InOrderImpl(right[root], data);
    }

    void PreOrderImpl(int32_t root, std::vector<int32_t>& data)
    {
        if (root == cNullElem)
            return;

        data.push_back(key[root]);
        PreOrderImpl(left[root], data);
        PreOrderImpl(right[root], data);
    }

    void PostOrderImpl(int32_t root, std::vector<int32_t>& data)
    {
        if (root == cNullElem)
            return;

        PostOrderImpl(left[root], data);
        PostOrderImpl(right[root], data);
        data.push_back(key[root]);
    }

public:
    void read()
    {
        std::cin >> n;
        key.resize(n);
        left.resize(n);
        right.resize(n);
        for (int32_t i = 0; i < n; i++) {
            std::cin >> key[i] >> left[i] >> right[i];
        }
    }

    std::vector<int32_t> InOrder()
    {
        std::vector<int32_t> result;
        result.reserve(n);
        InOrderImpl(0, result);
        return result;
    }

    std::vector<int32_t> PreOrder()
    {
        std::vector<int32_t> result;
        result.reserve(n);
        PreOrderImpl(0, result);
        return result;
    }

    std::vector<int32_t> PostOrder()
    {
        std::vector<int32_t> result;
        result.reserve(n);
        PostOrderImpl(0, result);
        return result;
    }
};

void print(const std::vector<int32_t>& a)
{
    for (size_t i = 0; i < a.size(); i++) {
        if (i > 0) {
            std::cout << ' ';
        }
        std::cout << a[i];
    }
    std::cout << '\n';
}

int32_t main_with_large_stack_space()
{
    std::ios_base::sync_with_stdio(0);
    TreeOrders t;
    t.read();
    print(t.InOrder());
    print(t.PreOrder());
    print(t.PostOrder());
    return 0;
}

int32_t main(int32_t argc, char** argv)
{
#if defined(__unix__) || defined(__APPLE__)
    // Allow larger stack space
    const rlim_t kStackSize = 16 * 1024 * 1024; // min stack size = 16 MB
    struct rlimit rl;
    int32_t result;

    result = getrlimit(RLIMIT_STACK, &rl);
    if (result == 0) {
        if (rl.rlim_cur < kStackSize) {
            rl.rlim_cur = kStackSize;
            result = setrlimit(RLIMIT_STACK, &rl);
            if (result != 0) {
                std::cerr << "setrlimit returned result = " << result << std::endl;
            }
        }
    }
#endif

    return main_with_large_stack_space();
}
