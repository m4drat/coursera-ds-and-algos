#include <cassert>
#include <iostream>
#include <vector>

template<class T>
class ListNode
{
public:
    ListNode* next;
    T data;

    ListNode(ListNode* tNext, T tData)
        : next{ tNext }
        , data{ tData }
    {}

    ListNode()
        : next{ nullptr }
        , data{}
    {}
};

template<class T>
class SinglyLinkedList
{
    ListNode<T>* root;
    uint32_t listSize;

public:
    void PushFront(T tData)
    {
        // Create new root node, with pointer pointing to the current root node.
        ListNode<T>* node = new ListNode<T>(root, tData);
        // Update root node with a newly created node
        root = node;

        // Increase list size
        listSize++;
    }

    void PopFront()
    {
        // If list is empty - do nothing
        if (!listSize)
            return;

        // Save current root node
        ListNode<T>* node = root;

        // Update root node with the next pointer
        root = node->next;

        // Delete old root node
        delete node;

        // Decrease list size
        listSize--;
    }

    ListNode<T> TopFront() const
    {
        // If list is empty - return empty node
        if (!listSize)
            return ListNode<T>();

        return *root;
    }

    bool Empty()
    {
        // If next pointer of root node is empty - than the list is empty
        return !root->next;
    }

    uint32_t Size()
    {
        return listSize;
    }

    SinglyLinkedList()
        : root{ new ListNode<T>() }
        , listSize(0)
    {}

    ~SinglyLinkedList()
    {
        // For each node starting from the root - delete them one by one
        for (ListNode<T>* node = root; node != nullptr;) {
            ListNode<T>* next = node->next;
            delete node;
            node = next;
        }
    }
};

template<class T>
class StackWithMax
{
    SinglyLinkedList<T> stack;
    SinglyLinkedList<T> maxStack;

public:
    void Push(T value)
    {
        // If newly pushed value is bigger than current maximum
        // Add this new value to the stack with max values
        if (value >= Max()) {
            maxStack.PushFront(value);
        }

        // Push value to the default stack
        stack.PushFront(value);
    }

    void Pop()
    {
        // maxStack.size() should always be less than or equal to
        // stack.size() (we can't have more elements in maxStack
        // than in default stack)
        assert(stack.Size() >= maxStack.Size());

        // If stack is empty - do nothing
        if (stack.Empty())
            return;

        // Get top element from the default stack
        ListNode<T> extacted = stack.TopFront();
        // Delete element from the default stack
        stack.PopFront();

        // If exracted element equal to the element from maxStack
        // - update maxStack by removing one element from it
        if (extacted.data == maxStack.TopFront().data) {
            maxStack.PopFront();
        }
    }

    T Top()
    {
        return stack.TopFront().data;
    }

    T Max() const
    {
        return maxStack.TopFront().data;
    }

    bool Empty()
    {
        return stack.Empty();
    }
};

template<class T>
class MaxQueue
{
private:
    StackWithMax<T> enqueueStack;
    StackWithMax<T> dequeuStack;

    void TransferStackAToB(StackWithMax<T>& stackA, StackWithMax<T>& stackB)
    {
        while (!stackA.Empty()) {
            T topValue = stackA.Top();
            stackA.Pop();

            stackB.Push(topValue);
        }
    }

public:
    void Enqueue(T value)
    {
        enqueueStack.Push(value);
    }

    void Dequeue()
    {
        if (dequeuStack.Empty()) {
            TransferStackAToB(enqueueStack, dequeuStack);
        }

        dequeuStack.Pop();
    }

    // // Warning slow operation!
    // T Back() {
    //     if (dequeuStack.Empty()) {
    //         TransferStackAToB(enqueueStack, dequeuStack);
    //     }

    //     return dequeuStack.Top();
    // }

    // // Warning slow operation!
    // T Front() {
    //     if (enqueueStack.Empty()) {
    //         TransferStackAToB(dequeuStack, enqueueStack);
    //     }

    //     return dequeuStack.Top();
    // }

    T Max()
    {
        return std::max(enqueueStack.Max(), dequeuStack.Max());
    }
};

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
    assert(array.size() >= m);

    MaxQueue<int32_t> maxQueue;

    // Initial values for the maxQueue
    for (uint32_t i = 0; i < m; ++i) {
        maxQueue.Enqueue(array[i]);
    }
    std::cout << maxQueue.Max() << ' ';

    for (uint32_t i = m; i < array.size(); ++i) {
        maxQueue.Dequeue();
        maxQueue.Enqueue(array[i]);

        std::cout << maxQueue.Max() << ' ';
    }

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

    MaxSlidingWindow(A, m);

    return 0;
}
