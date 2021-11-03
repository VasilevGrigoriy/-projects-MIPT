#include <iostream>
#include <string>
#include <cmath>
#include <vector>
#include <algorithm>

#pragma GCC optimize("Ofast,no-stack-protector")
#pragma GCC target("sse,sse2,sse3,ssse3,sse4,popcnt,abm,mmx,avx,avx2,tune=native")
#pragma GCC optimize("unroll-loops")
#pragma GCC optimize("fast-math")
#pragma GCC optimize("section-anchors")
#pragma GCC optimize("profile-values,profile-reorder-functions,tracer")
#pragma GCC optimize("vpt")
#pragma GCC optimize("rename-registers")
#pragma GCC optimize("move-loop-invariants")
#pragma GCC optimize("unswitch-loops")
#pragma GCC optimize("function-sections")
#pragma GCC optimize("data-sections")
#pragma GCC optimize("branch-target-load-optimize")
#pragma GCC optimize("branch-target-load-optimize2")
#pragma GCC optimize("btr-bb-exclusive")


long long p = 13;
long long m = 2000017177;

long long h(std::string s) {
    std::vector<long long> pref;
    pref.push_back(int(s[0]));
    for (int i = 1; i <= s.size() - 1; ++i) {
        pref.push_back(((pref[i - 1] * p) % m + int(s[i])) % m);
    }
    return pref[s.size() - 1];
}
int main()
{
    int N, M;
    std::cin >> N >> M;
    std::vector<int> s(N);
    std::vector<long long> pref;
    std::vector<int> s_rev(N);
    std::vector<long long> pref_rev;
    for (int i = 0; i < N; i++) {
        int x;
        std::cin >> x;
        s[i] = x;
        s_rev[N - 1 - i] = x;
    }
    pref.push_back(int(s[0]));
    for (int i = 1; i <= s.size() - 1; ++i) {
        pref.push_back(((pref[i - 1] * p) % m + int(s[i])) % m);
    }
    pref_rev.push_back(int(s_rev[0]));
    for (int i = 1; i <= s_rev.size() - 1; ++i) {
        pref_rev.push_back(((pref_rev[i - 1] * p) % m + int(s_rev[i])) % m);
    }
    //long long hash = ((pref[i + t.size() - 1] - (pref[i - 1] * pp) % m) % m + m) % m;
    std::vector<int> ans;
    long long pp = p;
    ans.push_back(s.size());
    for (int i = 1; i <= s.size() / 2; ++i) {
        long long hash_1 = ((pref[2 * i - 1] - (pref[i - 1] * pp) % m) % m + m) % m; //from common
        long long hash_2 = ((pref_rev[s_rev.size() - 1] - (pref[s_rev.size() - i - 1] * pp) % m) % m + m) % m; //from reversed
        if (hash_1 == hash_2) ans.push_back(s.size() - i);
        pp = (pp * p) % m;
    }
    for (int i = ans.size() - 1; i >= 0; i--) {
        std::cout << ans[i] << ' ';
    }
    return 0;
}