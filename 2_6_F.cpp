#include <iostream>
#include <vector>

void main_work(std::vector<int>& str1, std::vector<int>& str2, int m, std::vector<std::vector<int>>& dp, int n) {
	for (int i = 0; i < n + 1; ++i) {
		dp[i].resize(m + 1, 0);
	}
	for (int i = 1; i < n + 1; ++i) {
		for (int j = 1; j < m + 1; ++j) {
			if (str1[i] != str2[j]) {
				dp[i][j] = std::max(dp[i - 1][j], dp[i][j - 1]);
			}
			else {
				dp[i][j] = std::max(1 + dp[i - 1][j - 1], std::max(dp[i - 1][j], dp[i][j - 1]));
			}
		}
	}
}

int main()
{
	int n;
	std::cin >> n;
	std::vector<int> str1;
	str1.push_back(0);
	for (int i = 0; i < n; ++i) {
		int x;
		std::cin >> x;
		str1.push_back(x);
	}
	int m;
	std::cin >> m;
	std::vector<int> str2;
	str2.push_back(0);
	for (int i = 0; i < m; ++i) {
		int x;
		std::cin >> x;
		str2.push_back(x);
	}
	std::vector<std::vector<int>> dp(n + 1);
	main_work(str1, str2, m, dp, n);
	std::cout << dp[n][m];
	return 0;
}