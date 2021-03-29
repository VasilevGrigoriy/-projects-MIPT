#include <iostream>
#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <string>

long long main_work(int n, int m, std::vector<std::vector<long long>>& dp, std::vector<long long>& v1, std::vector<long long>& v2) {
    long long prev_max = 0;
    for (int i = 1; i <= n; i++) {
        prev_max = 0;
        for (int j = 1; j <= m; j++) {
            if (v1[i - 1] == v2[j - 1] && dp[i - 1][j] < prev_max + 1) {
                dp[i][j] = prev_max + 1;
            }
            else if (v1[i - 1] > v2[j - 1] && dp[i - 1][j] > prev_max) {
                dp[i][j] = dp[i - 1][j];
                prev_max = dp[i - 1][j];
            }
            else {
                dp[i][j] = dp[i - 1][j];
                continue;
            }
        }
    }
    long long ans = 0;
    for (int i = 1; i <= m; i++) {
        ans = std::max(ans, dp[n][i]);
    }
    return ans;
}

int main()
{
    int n, m;
    std::cin >> n >> m;
    std::vector<long long> v1;
    std::vector<long long> v2;
    for (int i = 0; i < n; i++) {
        long long x;
        std::cin >> x;
        v1.push_back(x);
    }
    for (int i = 0; i < m; i++) {
        long long x;
        std::cin >> x;
        v2.push_back(x);
    }
    std::vector<std::vector<long long>> dp(n + 1, std::vector<long long>(m + 1, 0));
    std::cout << main_work(n, m, dp, v1, v2);
    return 0;
}