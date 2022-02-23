#include <algorithm>
#include <iostream>
#include <vector>

struct Node
{
    int32_t key;
    int32_t left;
    int32_t right;

    Node()
        : key(0)
        , left(-1)
        , right(-1)
    {}
    Node(int32_t key_, int32_t left_, int32_t right_)
        : key(key_)
        , left(left_)
        , right(right_)
    {}
};

bool IsBinarySearchTree(const std::vector<Node>& tree)
{
    // Implement correct algorithm here
    return true;
}

int32_t main()
{
    int32_t nodes;
    std::cin >> nodes;
    std::vector<Node> tree;
    for (int32_t i = 0; i < nodes; ++i) {
        int32_t key, left, right;
        std::cin >> key >> left >> right;
        tree.push_back(Node(key, left, right));
    }

    if (IsBinarySearchTree(tree)) {
        std::cout << "CORRECT" << std::endl;
    } else {
        std::cout << "INCORRECT" << std::endl;
    }
    return 0;
}
