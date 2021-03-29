#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cmath>

long long main_work(long long s, long long n, std::vector<long long>& weights, std::vector<std::vector<long long>>& dp) {
	for (int i = 0; i < n + 1; ++i) {
		dp[i].resize(s + 1, 0);
	}
	for (int i = 0; i < n; ++i) {
		for (int w = 0; w < s + 1; ++w) {
			dp[i + 1][w] = std::max(dp[i + 1][w], dp[i][w]);
			if (w + weights[i] > s) continue;
			dp[i + 1][w + weights[i]] = std::max(dp[i + 1][w + weights[i]], dp[i][w] + weights[i]);
		}
	}
	long long res = 0;
	for (int i = 0; i <= n; ++i) {
		res = std::max(dp[i][s], res);
	}
	return res;
}

int main()
{
	std::ios::sync_with_stdio(false);
	std::cin.tie(0);
	std::cout.tie(0);
	long long s, n;
	std::cin >> s >> n;
	std::vector<long long> weights;
	for (int i = 0; i < n; ++i) {
		long long x;
		std::cin >> x;
		weights.push_back(x);
	}
	std::vector<std::vector<long long>> dp(n + 1);

	std::cout << main_work(s, n, weights, dp);
	return 0;
}