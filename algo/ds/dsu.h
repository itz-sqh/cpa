#ifndef CPA_DS_DSU_H
#define CPA_DS_DSU_H
#include <numeric>
#include <vector>
namespace cpa::ds {
    struct disjoint_set_union {
        // member data
        std::vector<int> parent, rank;

        disjoint_set_union(int n) : parent(n), rank(n, 0) {
            std::iota(parent.begin(), parent.end(), 0);
        }
        int find(int u) {
            return u == parent[u] ? u : parent[u] = find(parent[u]);
        }
        void unite(int u, int v) {
            u = find(u), v = find(v);
            if (u == v) return;
            if (rank[u] < rank[v])
                std::swap(u, v);
            parent[v] = u;
            if (rank[u] == rank[v])
                rank[u]++;
        }
    };
    typedef disjoint_set_union dsu;
}
#endif //CPA_DS_DSU_H
