#include <algorithm>
#include <iostream>
#include <limits>
#include <vector>

const int32_t cNullElem = -1;

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

bool InOrderImpl(const std::vector<Node>& tTree, int32_t tRootIdx, int32_t& tPrevIdx)
{
    if (tRootIdx == cNullElem)
        return true;

    if (!InOrderImpl(tTree, tTree[tRootIdx].left, tPrevIdx))
        return false;

    if (tPrevIdx != cNullElem && tTree[tRootIdx].key <= tTree[tPrevIdx].key)
        return false;

    tPrevIdx = tRootIdx;
    return InOrderImpl(tTree, tTree[tRootIdx].right, tPrevIdx);
}

bool IsBinarySearchTree(const std::vector<Node>& tTree)
{
    if (tTree.empty())
        return true;

    int32_t tPrevIdx = cNullElem;
    return InOrderImpl(tTree, 0, tPrevIdx);
}

int32_t main()
{
    int32_t nodes;
    std::cin >> nodes;
    std::vector<Node> tree;
    tree.reserve(nodes);

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
