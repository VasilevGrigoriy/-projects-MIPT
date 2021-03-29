#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
bool comp(std::pair<int, int> a, std::pair<int, int> b) {
    if (a.first<b.first || a.first > b.first) return a.first < b.first;
    else {
        return a.second > b.second;
    }
}
std::pair<int, int> get(std::vector<std::pair<int, int>>& v, int current_knot, int tl, int tr, int l, int r) {
    if (l > r)
        return { 0,-1 };
    if (l == tl && r == tr)
        return v[current_knot];
    int tm = (tl + tr) / 2;
    std::pair<int, int> v1 = get(v, current_knot * 2, tl, tm, l, std::min(r, tm));
    std::pair<int, int> v2 = get(v, current_knot * 2 + 1, tm + 1, tr, std::max(l, tm + 1), r);
    if (v1.first > v2.first) return v1;
    else return v2;
}
void update(std::vector<std::pair<int, int>>& v, int current_knot, int tl, int tr, int pos, int new_val1, int new_val2) {
    if (tl == tr) {
        v[current_knot].first = new_val1;
        v[current_knot].second = new_val2;
    }
    else {
        int tm = (tl + tr) / 2;
        if (pos <= tm)
            update(v, current_knot * 2, tl, tm, pos, new_val1, new_val2);
        else
            update(v, current_knot * 2 + 1, tm + 1, tr, pos, new_val1, new_val2);
        if (v[current_knot * 2].first > v[current_knot * 2 + 1].first) {
            v[current_knot] = v[current_knot * 2];
        }
        else {
            v[current_knot] = v[current_knot * 2 + 1];
        }
    }
}
void main_work(std::vector<std::pair<int, int>>& v, int n) {
    std::sort(v.begin(), v.end());
    std::vector<std::pair<int, int>> tree(4 * n + 4);
    std::vector<std::pair<int, int>> dp(n);
    for (int i = 0; i < n; i++) {
        std::pair<int, int> mx = get(tree, 1, 0, n - 1, 0, v[i].second - 1);
        dp[i].first = std::max(1, 1 + mx.first);
        dp[i].second = mx.second;
        update(tree, 1, 0, n - 1, v[i].second, dp[i].first, i);
    }
    int mx = 0;
    int temp = 0;
    std::pair <int, int> m;
    for (int i = 0; i < n; i++) {
        if (mx <= dp[i].first) {
            mx = dp[i].first;
            m = dp[i];
            temp = i;
        }
    }
    std::cout << mx << '\n';
    while (temp != -1) {
        std::cout << n - v[temp].second << ' ';
        temp = dp[temp].second;
    }
}
int main()
{
    int n;
    std::cin >> n;
    std::vector<std::pair<int, int>> v(n);
    for (int i = 0; i < n; i++) {
        int x;
        std::cin >> x;
        v[n - 1 - i] = { x,n - 1 - i };
    }
    main_work(v, n);
    return 0;
}