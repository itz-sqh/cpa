#ifndef CPA_DS_TREAP_H
#define CPA_DS_TREAP_H
#include <utility>

namespace cpa::ds {
    struct treap {
        typedef int Key;
        struct node {
            Key key;
            int prior;
            long long subtree_sum = 0;
            size_t _subtree_size = 1;
            node *left = nullptr, *right = nullptr;
            node() = default;
            node(Key key) : key(key), prior(rand()), subtree_sum(key) {}
        };
        typedef node* ptr;

        // root of treap
        ptr root = nullptr;

        // Merge two treaps A and B where max(A) <= min(B)
        // Returns merged treap
        static ptr merge(ptr A, ptr B) {
            if (!A || !B) return A ? A : B;
            push(A), push(B);
            if (A->prior > B->prior) {
                ptr r = merge(A->right, B);
                A->right = r;
                update(A);
                return A;
            }
            ptr l = merge(A,B->left);
            B->left = l;
            update(B);
            return B;
        }

        // Split treap A into (L, R) where L contains keys <= key and R contains keys > key
        // Returns pair {L, R}
        static std::pair<ptr,ptr> split(ptr A, Key key) {
            if (!A) return {nullptr, nullptr};
            push(A);
            if (A->key <= key) {
                auto [l, r] = split(A->right, key);
                A->right = l;
                update(A);
                return {A,r};
            }
            auto [l,r] = split(A->left, key);
            A->left = r;
            update(A);
            return {l,A};
        }
        // Split treap A into (L, R) where |L| = k (when |A| >= k) or L = A (when |A| < k)
        // Returns pair {L, R}
        static std::pair<ptr,ptr> split_by_order(ptr A, size_t k) {
            if (!A) return {nullptr, nullptr};
            push(A);
            size_t left_size = subtree_size(A->left);
            if (left_size >= k) {
                auto [l, r] = split_by_order(A->left, k);
                A->left = r;
                update(A);
                return {l, A};
            }
            auto [l, r] = split_by_order(A->right, k - left_size - 1);
            A->right = l;
            update(A);
            return {A, r};
        }

        // Update subtree size and sum for node A
        static void update(ptr A) {
            if (A) {
                A->_subtree_size = subtree_size(A->left) + subtree_size(A->right) + 1;
                A->subtree_sum = sum(A->left) + sum(A->right) + A->key;
            }
        }
        // Safely get sum of subtree rooted at A
        static long long sum(ptr A) {
            return A ? A->subtree_sum : 0;
        }
        static void push(ptr A) {
            // in case you need lazy, push to children of A
        }
        // Safely get size of subtree rooted at A
        static size_t subtree_size(ptr A) {
            return A ? A->_subtree_size : 0;
        }
        static ptr find(ptr A, Key key) {
            while (A) {
                push(A);
                if (A->key == key) return A;
                A = (key < A->key) ? A->left : A->right;
            }
            return nullptr;
        }
        // Find first node with key >= target in subtree A
        // Returns pointer to node or nullptr if not found
        static ptr lower_bound(ptr A, Key key) {
            ptr res = nullptr;
            while (A) {
                push(A);
                if (A->key >= key) {
                    res = A;
                    A = A->left;
                }
                else {
                    A = A->right;
                }
            }
            return res;
        }
        // Find first node with key > target in subtree A
        // Returns pointer to node or nullptr if not found
        static ptr upper_bound(ptr A, Key key) {
            ptr res = nullptr;
            while (A) {
                push(A);
                if (A->key > key) {
                    res = A;
                    A = A->left;
                }
                else {
                    A = A->right;
                }
            }
            return res;
        }
        // Count nodes with key < target in subtree A
        // Returns count
        static size_t count_less(ptr A, Key key) {
            size_t count = 0;
            while (A) {
                push(A);
                if (A->key < key) {
                    count += subtree_size(A->left) + 1;
                    A = A->right;
                }
                else {
                    A = A->left;
                }
            }
            return count;
        }
        size_t count_less(Key key) const {
            return count_less(root, key);
        }
        size_t count_less_equal(Key key) const {
            return count_less(key) + (find(key) != nullptr);
        }
        size_t count_greater(Key key) const {
            return size() - count_less_equal(key);
        }
        size_t count_greater_equal(Key key) const {
            return size() - count_less(key);
        }
        // Insert key into tree (does nothing if key already exists)
        void insert(Key key) {
            auto [lm,r] = split(root, key);
            auto [l, m] = split(lm, key-1);
            if (m) {
                root = merge(merge(l,m),r);
                return;
            }
            ptr new_node = new node(key);
            root = merge(merge(l,new_node),r);
        }
        // Remove key from tree (does nothing if key doesn't exist)
        void erase(Key key) {
            auto [lm, r] = split(root, key);
            auto [l,m] = split(lm,key-1);
            if (m) {
                push(m);
                ptr tmp = m;
                m = merge(m->left, m->right);
                delete tmp;
            }
            root = merge(merge(l,m),r);
        }
        ptr find(Key key) const {
            return find(root, key);
        }
        bool contains(Key key) const {
            return find(key) != nullptr;
        }
        size_t size() const {
            return subtree_size(root);
        }
        bool empty() const {
            return root == nullptr;
        }
        // Find k-th smallest element (0-indexed)
        // Returns pointer to node or nullptr if k >= size()
        ptr find_by_order(size_t k) const {
            if (k >= size()) return nullptr;
            ptr cur = root;
            while (cur) {
                push(cur);
                size_t left = subtree_size(cur->left);
                if (left == k) {
                    return cur;
                }
                else if (left > k) {
                    cur = cur->left;
                }
                else {
                    k -= left + 1;
                    cur = cur->right;
                }
            }
            return nullptr;
        }
        // Get number of nodes with key < target
        // Returns count (0-indexed order of first occurrence if key exists)
        size_t order_of_key(Key key) {
            auto [l,r] = split(root, key - 1);
            size_t res = subtree_size(l);
            root = merge(l,r);
            return res;
        }
        // Count nodes with keys in range [l_key, r_key]
        // Returns count
        size_t count_range(Key l_key, Key r_key) {
            auto [l,mr] = split(root, l_key-1);
            auto [m,r] = split(mr, r_key);
            size_t res = subtree_size(m);
            root = merge(merge(l,m),r);
            return res;
        }
        // Get sum of keys in range [l_key, r_key]
        // Returns sum
        long long range_query(Key l_key, Key r_key) {
            auto [lm,r] = split(root, r_key);
            auto [l, m] = split(lm, l_key-1);
            push(m);
            long long res = sum(m);
            root = merge(merge(l,m),r);
            return res;
        }
        // Find largest node with key < target
        // Returns pointer to node or nullptr if not found
        ptr predecessor(Key key) {
            auto [l, r] = split(root, key - 1);
            if (!l) {
                root = merge(l, r);
                return nullptr;
            }
            ptr cur = l;
            push(cur);
            while (cur->right) {
                push(cur);
                cur = cur->right;
            }
            push(cur);
            ptr res = cur;
            root = merge(l, r);
            return res;
        }
        // Find smallest node with key > target
        // Returns pointer to node or nullptr if not found
        ptr successor(Key key) {
            auto [l, r] = split(root, key);
            if (!r) {
                root = merge(l, r);
                return nullptr;
            }
            ptr cur = r;
            push(cur);
            while (cur->left) {
                push(cur);
                cur = cur->left;
            }
            push(cur);
            ptr res = cur;
            root = merge(l, r);
            return res;
        }
    };
}

#endif //CPA_DS_TREAP_H
