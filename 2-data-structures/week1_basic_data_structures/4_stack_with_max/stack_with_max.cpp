#include <algorithm>
#include <cassert>
#include <iostream>
#include <string>
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

    bool IsEmpty()
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
        if (stack.IsEmpty())
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

    T Max() const
    {
        return maxStack.TopFront().data;
    }
};

int main()
{
    int num_queries = 0;
    std::cin >> num_queries;

    std::string query;
    std::string value;

    StackWithMax<int32_t> stack;

    for (int i = 0; i < num_queries; ++i) {
        std::cin >> query;
        if (query == "push") {
            std::cin >> value;
            stack.Push(std::stoi(value));
        } else if (query == "pop") {
            stack.Pop();
        } else if (query == "max") {
            std::cout << stack.Max() << "\n";
        } else {
            assert(0);
        }
    }
    return 0;
}