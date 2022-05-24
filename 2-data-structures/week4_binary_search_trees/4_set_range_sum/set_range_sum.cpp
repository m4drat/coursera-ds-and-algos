#include <cstdio>

// Splay tree implementation

// Vertex of a Splay tree
struct Vertex
{
    int key;
    // Sum of all the keys in the subtree - remember to Update
    // it after each operation that changes the tree.
    long long sum;
    Vertex* left;
    Vertex* right;
    Vertex* parent;

    Vertex(int key, long long sum, Vertex* left, Vertex* right, Vertex* parent)
        : key(key)
        , sum(sum)
        , left(left)
        , right(right)
        , parent(parent)
    {}
};

void Update(Vertex* v)
{
    if (v == NULL)
        return;
    v->sum =
        v->key + (v->left != NULL ? v->left->sum : 0ll) + (v->right != NULL ? v->right->sum : 0ll);
    if (v->left != NULL) {
        v->left->parent = v;
    }
    if (v->right != NULL) {
        v->right->parent = v;
    }
}

void SmallRotation(Vertex* v)
{
    Vertex* parent = v->parent;
    if (parent == NULL) {
        return;
    }
    Vertex* grandparent = v->parent->parent;
    if (parent->left == v) {
        Vertex* m = v->right;
        v->right = parent;
        parent->left = m;
    } else {
        Vertex* m = v->left;
        v->left = parent;
        parent->right = m;
    }
    Update(parent);
    Update(v);
    v->parent = grandparent;
    if (grandparent != NULL) {
        if (grandparent->left == parent) {
            grandparent->left = v;
        } else {
            grandparent->right = v;
        }
    }
}

void BigRotation(Vertex* v)
{
    if (v->parent->left == v && v->parent->parent->left == v->parent) {
        // Zig-zig
        SmallRotation(v->parent);
        SmallRotation(v);
    } else if (v->parent->right == v && v->parent->parent->right == v->parent) {
        // Zig-zig
        SmallRotation(v->parent);
        SmallRotation(v);
    } else {
        // Zig-zag
        SmallRotation(v);
        SmallRotation(v);
    }
}

// Makes Splay of the given vertex and makes
// it the new root.
void Splay(Vertex*& root, Vertex* v)
{
    if (v == NULL)
        return;
    while (v->parent != NULL) {
        if (v->parent->parent == NULL) {
            SmallRotation(v);
            break;
        }
        BigRotation(v);
    }
    root = v;
}

// Searches for the given key in the tree with the given root
// and calls Splay for the deepest visited node after that.
// If found, returns a pointer to the node with the given key.
// Otherwise, returns a pointer to the node with the smallest
// bigger key (next value in the order).
// If the key is bigger than all keys in the tree,
// returns NULL.
Vertex* Find(Vertex*& root, int key)
{
    Vertex* v = root;
    Vertex* last = root;
    Vertex* next = NULL;
    while (v != NULL) {
        if (v->key >= key && (next == NULL || v->key < next->key)) {
            next = v;
        }
        last = v;
        if (v->key == key) {
            break;
        }
        if (v->key < key) {
            v = v->right;
        } else {
            v = v->left;
        }
    }
    Splay(root, last);
    return next;
}

void Split(Vertex* root, int key, Vertex*& left, Vertex*& right)
{
    right = Find(root, key);
    Splay(root, right);
    if (right == NULL) {
        left = root;
        return;
    }
    left = right->left;
    right->left = NULL;
    if (left != NULL) {
        left->parent = NULL;
    }
    Update(left);
    Update(right);
}

Vertex* Merge(Vertex* left, Vertex* right)
{
    if (left == NULL)
        return right;
    if (right == NULL)
        return left;
    Vertex* min_right = right;
    while (min_right->left != NULL) {
        min_right = min_right->left;
    }
    Splay(right, min_right);
    right->left = left;
    Update(right);
    return right;
}

// Code that uses Splay tree to solve the problem

Vertex* root = NULL;

void Insert(int x)
{
    Vertex* left = NULL;
    Vertex* right = NULL;
    Vertex* new_vertex = NULL;
    Split(root, x, left, right);
    if (right == NULL || right->key != x) {
        new_vertex = new Vertex(x, x, NULL, NULL, NULL);
    }
    root = Merge(Merge(left, new_vertex), right);
}

void Erase(int x)
{
    // Implement Erase yourself
}

bool Find(int x)
{
    // Implement Find yourself

    return false;
}

long long sum(int from, int to)
{
    Vertex* left = NULL;
    Vertex* middle = NULL;
    Vertex* right = NULL;
    Split(root, from, left, middle);
    Split(middle, to + 1, middle, right);
    long long ans = 0;
    // Complete the implementation of sum

    return ans;
}

const int MODULO = 1000000001;

int main()
{
    int n;
    scanf("%d", &n);
    int last_sum_result = 0;
    for (int i = 0; i < n; i++) {
        char buffer[10];
        scanf("%s", buffer);
        char type = buffer[0];
        switch (type) {
            case '+': {
                int x;
                scanf("%d", &x);
                Insert((x + last_sum_result) % MODULO);
            } break;
            case '-': {
                int x;
                scanf("%d", &x);
                Erase((x + last_sum_result) % MODULO);
            } break;
            case '?': {
                int x;
                scanf("%d", &x);
                printf(Find((x + last_sum_result) % MODULO) ? "Found\n" : "Not found\n");
            } break;
            case 's': {
                int l, r;
                scanf("%d %d", &l, &r);
                long long res = sum((l + last_sum_result) % MODULO, (r + last_sum_result) % MODULO);
                printf("%lld\n", res);
                last_sum_result = int(res % MODULO);
            }
        }
    }
    return 0;
}
