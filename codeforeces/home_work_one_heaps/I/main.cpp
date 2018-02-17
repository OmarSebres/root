#include <bits/stdc++.h>
using namespace std;

const int INF = numeric_limits<int>::max();

struct node {
    node* child = nullptr;
    node* sibling = nullptr;
    int key;
    int degree;

    node(const node*& n):
        child(n->child),
        sibling(n->sibling),
        key(n->key),
        degree(n->degree) { }

    node(node*&& n):
        child(move(n->child)),
        sibling(move(n->sibling)),
        key(move(n->key)),
        degree(move(n->degree))
    {
        n->child = nullptr;
        n->sibling = nullptr;
    }

    node(int _key):
        child(nullptr),
        sibling(nullptr),
        key(_key),
        degree(0) { }

    static node* merge(node* a, node* b) {
        if (a == nullptr) return b;
        if (b == nullptr) return a;
        if (a->key < b->key) return merge(b, a);
        b->sibling = a->child;
        a->child = b;
        a->degree += 1;
        return a;
    }
};

struct heap {
    node* root;

    heap(): root(nullptr) { }

    heap(int val): root(new node(val)) { }

    heap(node* n): root(n) { }

    heap(const heap*& h): root(h->root) { }

    heap(heap*&& h): root(move(h->root)) {
        h->root = nullptr;
    }

    void insert(int x) {
        heap* h = new heap(x);
        root = merge(h)->root;
    }

    heap* merge(heap* other) {
        heap* h = new heap(merge(root, other->root));
        root = other->root = nullptr;
        if (h->root == nullptr) {
            return h;
        }
        node* prev = nullptr;
        node* head = h->root;
        node* next = head->sibling;
        while (next != nullptr) {
            if (head->degree != next->degree || (next->sibling != nullptr && next->sibling->degree == head->degree)) {
                prev = head;
                head = next;
            } else if (head->key > next->key) {
                head->sibling = next->sibling;
                head = node::merge(head, next);
            } else {
                if (prev == nullptr) {
                    h->root = next;
                } else {
                    prev->sibling = next;
                }
                next = node::merge(head, next);
                head = next;
            }
            next = next->sibling;
        }
        return move(h);
    }

    int pop() {
        if (root == nullptr) {
            return -INF;
        }
        node* head_prev = nullptr;
        node* head = root;
        node* next = head->sibling;
        node* prev = head;
        while (next != nullptr) {
            if (head->key < next->key) {
                head = next;
                head_prev = prev;
            }
            prev = next;
            next = next->sibling;
        }
        if (head == root) {
            root = root->sibling;
        } else {
            head_prev->sibling = head->sibling;
        }
        heap* h = new heap();
        node* without_max = head->child;
        while (without_max != nullptr) {
            node* temp_next = without_max->sibling;
            without_max->sibling = h->root;
            h->root = without_max;
            without_max = temp_next;
        }
        root = merge(move(h))->root;
        return head->key;
    }

private:
    void merge_nodes(node*& a, node*& b) {
        a->sibling = b->child;
        b->child = a;
        b->degree += 1;
    }

    node* merge(node*& n1, node*& n2) {
        if (n1 == nullptr) return n2;
        if (n2 == nullptr) return n1;

        node* head = nullptr;
        if (n1->degree <= n2->degree) {
            head = n1;
            n1 = n1->sibling;
        } else {
            head = n2;
            n2 = n2->sibling;
        }

        node* itr = head;
        while (n1 != nullptr && n2 != nullptr) {
            if (n1->degree <= n2->degree) {
                itr->sibling = n1;
                n1 = n1->sibling;
            } else {
                itr->sibling = n2;
                n2 = n2->sibling;
            }
            itr = itr->sibling;
        }
        if (n1 != nullptr) {
            itr->sibling = n1;
        } else {
            itr->sibling = n2;
        }
        return head;
    }
};

heap* heaps[1000010];

int main() {
    int q;
    scanf("%d", &q);

    int id = 0;
    char cmd[10];

    while (q-- > 0) {
        scanf("%s", cmd);
        if (strcmp(cmd, "new") == 0)  {
            heaps[id++] = new heap();
        } else if (strcmp(cmd, "push") == 0) {
            int i, d;
            scanf("%d%d", &i, &d);
            heaps[i - 1]->insert(d);
        } else if (strcmp(cmd, "pop") == 0) {
            int i;
            scanf("%d", &i);
            printf("%d\n", heaps[i - 1]->pop());
        } else {
            int i, j;
            scanf("%d%d", &i, &j);
            heaps[id++] = heaps[i - 1]->merge(heaps[j - 1]);
        }
    }
}
