#include <algorithm>
#include <iostream>
#include <queue>
#include <vector>
#if defined(__unix__) || defined(__APPLE__)
#include <sys/resource.h>
#endif

class Node;

class Node
{
public:
    int32_t key;
    Node* parent;
    std::vector<Node*> children;

    Node()
    {
        this->parent = NULL;
    }

    void setParent(Node* theParent)
    {
        parent = theParent;
        parent->children.push_back(this);
    }
};

uint32_t TreeHeightIterative(Node* root)
{
    std::queue<Node*> queue;
    queue.push(root);

    int32_t maxHeight = 0;
    while (!queue.empty()) {
        uint32_t currentLevelNodesCount = queue.size();

        for (; currentLevelNodesCount--;) {
            Node* currNode = queue.front();
            queue.pop();

            for (Node* node : currNode->children)
                queue.push(node);
        }

        maxHeight++;
    }

    return maxHeight;
}

uint32_t TreeHeightRecursive(Node* root)
{
    if (root->children.empty()) {
        return 1;
    }

    uint32_t curMax = 0;
    for (Node* node : root->children) {
        curMax = std::max(TreeHeightRecursive(node) + 1, curMax);
    }

    return curMax;
}

int32_t MainWithLargeStack()
{
    std::ios_base::sync_with_stdio(0);
    int32_t n;
    std::cin >> n;

    std::vector<Node> nodes;
    nodes.resize(n);
    uint32_t rootIdx = 0;

    for (int32_t child_index = 0; child_index < n; child_index++) {
        int32_t parent_index;
        std::cin >> parent_index;
        if (parent_index >= 0)
            nodes[child_index].setParent(&nodes[parent_index]);
        else
            rootIdx = child_index;
        nodes[child_index].key = child_index;
    }

    std::cout << TreeHeightIterative(&nodes[rootIdx]) << std::endl;

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
    return MainWithLargeStack();
}
