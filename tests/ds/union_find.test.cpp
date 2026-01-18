#include "algo/ds/dsu.h"
#include <iostream>
#include "blazingio/blazingio.min.hpp"
#define PROBLEM "https://judge.yosupo.jp/problem/unionfind"
using namespace std;
using namespace cpa::ds;
int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, q;
    cin >> n >> q;
    dsu dsu(n);
    vector<int> res;
    res.reserve(q/4);
    for(int i = 0; i < q; ++i) {
        uint8_t op;
        cin >> op;
        int u, v;
        cin >> u >> v;
        if (op == '0') dsu.unite(u, v);
        else if (op == '1') res.push_back(dsu.find(u) == dsu.find(v) ? 1 : 0);
    }
    for (auto x : res) cout << x << "\n";
}