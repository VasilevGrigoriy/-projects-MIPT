﻿#include <iostream>
#include <vector>
#include <cmath>
using namespace std;
long long hashing(long long name, long long p, long long m) {
    return ((name % p % (m - 1)) * (p % (m - 1))) % (m - 1);
}
long long main_theme(vector<pair<long long, long long>>& v, long long hash_x, long long x, long long hash_y, long long y) {
    long long temp_x = hash_x;
    long long temp_y = hash_y;
    while (v[temp_x].first != pow(10, 10)) {
        if (v[temp_x].first == x) break;
        temp_x++;
        if (temp_x >= 400000) temp_x = 0;
    }
    v[temp_x].first = x;
    while (v[temp_y].first != pow(10, 10)) {
        if (v[temp_y].first == y) break;
        temp_y++;
        if (temp_y >= 400000) temp_y = 0;
    }
    v[temp_y].first = y;
    if (v[temp_x].second == 0) v[temp_x].second = x;
    if (v[temp_y].second == 0) v[temp_y].second = y;
    swap(v[temp_y].second, v[temp_x].second);
    return abs(v[temp_y].second - v[temp_x].second);
}
int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(NULL);
    std::cout.tie(NULL);
    long long m = 400000;
    vector<pair<long long, long long>> v(m, { pow(10,10),0 });
    long long n;
    cin >> n;
    long long p = 10479967;
    for (long long i = 0; i < n; ++i) {
        long long x, y;
        cin >> x >> y;
        long long hash_x = hashing(x, p, m);
        long long hash_y = hashing(y, p, m);
        cout << main_theme(v, hash_x, x, hash_y, y) << '\n';
    }
    return 0;
}