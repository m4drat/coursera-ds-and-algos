#include <algorithm>
#include <cassert>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

// #include <concepts>
// template <class T>
// concept comparable = requires (T& a, T& b) { a == b; };

template<class T>
class ListNode
{
public:
    std::unique_ptr<ListNode<T>> next;
    T value;

    explicit ListNode(std::unique_ptr<ListNode<T>> tNext)
        : next{ std::move(tNext) }
    {}

    explicit ListNode(const T& tValue)
        : ListNode{ nullptr, tValue }
    {}

    explicit ListNode(std::unique_ptr<ListNode<T>> tNext, const T& tValue)
        : next{ std::move(tNext) }
        , value{ tValue }
    {}

    explicit ListNode()
        : ListNode{ nullptr }
    {}
};

template<class T>
class HashTable
{
private:
    uint32_t mBucketCount{ 1 };

    // We use vector of unique_ptrs because object T can be really big and we don't want to allocate
    // memory for it until we really going to use it
    std::vector<std::unique_ptr<ListNode<T>>> mElements{};

    std::size_t HashElement(const T& tElement) const
    {
        static const std::size_t multiplier = 263;
        static const std::size_t prime = 1000000007;
        std::size_t hash = 0;
        for (int32_t i = static_cast<int32_t>(tElement.size()) - 1; i >= 0; --i)
            hash = (hash * multiplier + tElement[i]) % prime;
        return hash % mBucketCount;
    }

public:
    explicit HashTable(uint32_t tBucketCount)
        : mBucketCount{ tBucketCount }
    {
        mElements.resize(tBucketCount);
    }

    void Insert(const T& tElement)
    {
        std::size_t hashtableIndex = HashElement(tElement);
        auto& head = mElements[hashtableIndex];

        if (!head) {
            head = std::make_unique<ListNode<T>>(tElement);
            return;
        }

        // Check that we already have this element in a hashmap
        ListNode<T>* headPtr = head.get();
        for (; headPtr != nullptr && tElement != headPtr->value; headPtr = headPtr->next.get()) {
        }

        if (headPtr != nullptr) {
            return;
        }

        std::unique_ptr<ListNode<T>> oldHead = std::move(head);
        std::unique_ptr<ListNode<T>> newHead =
            std::make_unique<ListNode<T>>(std::move(oldHead), tElement);
        head = std::move(newHead);
    }

    void Remove(const T& tElement)
    {
        std::size_t hashtableIndex = HashElement(tElement);
        auto& head = mElements[hashtableIndex];

        // Current bucket does not contain any elements
        if (!head) {
            return;
        }

        // Current bucket has just one element
        if (head->value == tElement && head->next == nullptr) {
            head.reset(nullptr);
            return;
        }

        // The first element in the bucket is the target and we have >= 1 of elements in the bucket
        if (head->value == tElement) {
            head = std::move(head->next);
            return;
        }

        // Current bucket has multiple elements and we are not deleting the first element
        ListNode<T>* next = head.get();
        ListNode<T>* prev = head.get();
        for (; next != nullptr && tElement != next->value; next = next->next.get()) {
            prev = next;
        }

        if (next != nullptr) {
            prev->next = std::move(next->next);
        }
    }

    bool Exists(const T& tElement) const
    {
        std::size_t hashtableIndex = HashElement(tElement);
        if (!mElements[hashtableIndex]) {
            return false;
        }

        ListNode<T>* head = mElements[hashtableIndex].get();
        for (; head != nullptr && tElement != head->value; head = head->next.get()) {
        }

        if (head != nullptr)
            return true;

        return false;
    }

    void PrintBucketList(uint32_t tBucketIndex)
    {
        if (!mElements[tBucketIndex]) {
            std::cout << '\n';
            return;
        }

        for (ListNode<T>* head = mElements[tBucketIndex].get(); head != nullptr;
             head = head->next.get()) {
            std::cout << head->value << ' ';
        }
        std::cout << '\n';
    }
};

struct Query
{
    std::string type, s;
    std::size_t ind;
};

class QueryProcessor
{
    uint32_t mBucketCount;
    // store all strings in one std::vector
    HashTable<std::string> mElems;

public:
    explicit QueryProcessor(uint32_t tBucketCount)
        : mBucketCount(tBucketCount)
        , mElems{ tBucketCount }
    {}

    Query ReadQuery() const
    {
        Query query;
        std::cin >> query.type;
        if (query.type != "check")
            std::cin >> query.s;
        else
            std::cin >> query.ind;
        return query;
    }

    void WriteSearchResult(bool was_found) const
    {
        std::cout << (was_found ? "yes\n" : "no\n");
    }

    void ProcessQuery(const Query& query)
    {
        if (query.type == "add") {
            mElems.Insert(query.s);
        } else if (query.type == "del") {
            mElems.Remove(query.s);
        } else if (query.type == "find") {
            WriteSearchResult(mElems.Exists(query.s));
        } else if (query.type == "check") {
            mElems.PrintBucketList(query.ind);
        } else {
            assert(false && "ProcessQuery failed!");
        }
    }

    void ProcessQueries()
    {
        int32_t n;
        std::cin >> n;
        for (int32_t i = 0; i < n; ++i)
            ProcessQuery(ReadQuery());
    }
};

int32_t main()
{
    std::ios_base::sync_with_stdio(false);
    int32_t bucket_count;
    std::cin >> bucket_count;
    QueryProcessor proc(bucket_count);
    proc.ProcessQueries();
    return 0;
}
