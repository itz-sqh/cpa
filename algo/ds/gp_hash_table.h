#ifndef CPA_DS_GP_HASH_TABLE_H
#define CPA_DS_GP_HASH_TABLE_H
#include <chrono>
#include <ext/pb_ds/assoc_container.hpp>
// source: https://codeforces.com/blog/entry/62393?locale=en
struct hash_ll {
    static uint64_t splitmix64(uint64_t x) {
        // http://xorshift.di.unimi.it/splitmix64.c
        x += 0x9e3779b97f4a7c15;
        x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9;
        x = (x ^ (x >> 27)) * 0x94d049bb133111eb;
        return x ^ (x >> 31);
    }

    size_t operator()(uint64_t x) const {
        static const uint64_t FIXED_RANDOM = std::chrono::steady_clock::now().time_since_epoch().count();
        return splitmix64(x + FIXED_RANDOM);
    }
};

struct hash_int {
    static uint32_t splitmix32(uint32_t x) {
        x += 0x9e3779b9;
        x = (x ^ (x >> 16)) * 0x85ebca6b;
        x = (x ^ (x >> 13)) * 0xc2b2ae35;
        return x ^ (x >> 16);
    }
    size_t operator()(uint32_t x) const {
        static const uint32_t FIXED_RANDOM = std::chrono::steady_clock::now().time_since_epoch().count();
        return splitmix32(x + FIXED_RANDOM);
    }
};
// test here: https://judge.yosupo.jp/problem/associative_array
typedef __gnu_pbds::gp_hash_table<long long, long long, hash_ll> hash_table;
#endif //CPA_DS_GP_HASH_TABLE_H
