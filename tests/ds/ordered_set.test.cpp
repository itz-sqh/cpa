#include <bits/stdc++.h>
#include "algo/ds/treap.h"
#include "blazingio/blazingio.min.hpp"

using namespace std;
using namespace cpa::ds;

#define PROBLEM "https://judge.yosupo.jp/problem/ordered_set"


int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    treap t;
    int n,q; cin >> n >> q;
    for (int i = 0; i < n; i++) {
        int x; cin >> x;
        t.insert(x);
    }
    while (q--) {
        int kind, x; cin >> kind >> x;
        if (kind == 0) {
            t.insert(x);
        }
        else if (kind == 1) {
            t.erase(x);
        }
        else if (kind == 2) {
            auto p = t.find_by_order(x-1);
            cout << (p ? p->key : -1) << "\n";
        }
        else if (kind == 3) {
            cout << t.count_less_equal(x) << "\n";
        }
        else if (kind == 4) {
            if (t.contains(x)) cout << x << "\n";
            else {
                auto p = t.predecessor(x);
                cout << (p ? p->key : -1) << "\n";
            }
        }
        else if (kind == 5) {
            if (t.contains(x)) cout << x << "\n";
            else {
                auto p = t.successor(x);
                cout << (p ? p->key : -1) << "\n";
            }
        }
    }
}