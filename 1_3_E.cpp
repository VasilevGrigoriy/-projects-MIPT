#include <set>
#include <iostream>
#include <vector>
#include <string>
#include <map>
void ADD(std::vector<std::set<long long>>& massiv, long long number, long long massive, std::map<long long, std::set<long long>>& massives_of_numbers) {
	massiv[massive].insert(number);
	massives_of_numbers[number].insert(massive);
}
void DELETE(std::vector<std::set<long long>>& massiv, long long number, long long massive, std::map<long long, std::set<long long>>& massives_of_numbers) {
	massiv[massive].erase(number);
	massives_of_numbers[number].erase(massive);
}
void CLEAR(std::vector<std::set<long long>>& massiv, long long massive, std::map<long long, std::set<long long>>& massives_of_numbers) {
	for (auto it : massiv[massive]) {
		massives_of_numbers[it].erase(massive);
	}
	massiv[massive].clear();
}
void LISTSET(std::vector<std::set<long long>>& massiv, long long massive) {
	std::ios::sync_with_stdio(0);
	std::cout.tie(0);
	if (massiv[massive].size() == 0) {
		std::cout << -1 << '\n';
		return;
	}
	for (auto it : massiv[massive]) {
		std::cout << it << ' ';
	}
	std::cout << '\n';
}
void LISTSETSOF(std::vector<std::set<long long>>& massiv, long long number, std::map<long long, std::set<long long>>& massives_of_numbers) {
	std::ios::sync_with_stdio(0);
	std::cout.tie(0);
	bool marker = 0;
	for (auto it : massives_of_numbers[number]) {
		marker = true;
		std::cout << it << ' ';
	}
	if (!marker) std::cout << -1;
	std::cout << '\n';
}
int main()
{
	std::ios::sync_with_stdio(0);
	std::cin.tie(0);
	std::cout.tie(0);
	long long n, m, k;
	std::cin >> n >> m >> k;
	std::vector<std::set<long long>> massives(m + 1);
	std::map<long long, std::set<long long>> massives_of_numbers;
	std::string s;
	getline(std::cin, s);
	for (long long i = 0; i < k; ++i) {
		std::cin >> s;
		if (s[0] == 'A') {
			long long number, massive;
			std::cin >> number >> massive;
			ADD(massives, number, massive, massives_of_numbers);
		}
		else if (s[0] == 'D') {
			long long number, massive;
			std::cin >> number >> massive;
			DELETE(massives, number, massive, massives_of_numbers);
		}
		else if (s[0] == 'C') {
			long long massive;
			std::cin >> massive;
			CLEAR(massives, massive, massives_of_numbers);
		}
		else if (s == "LISTSET") {
			long long massive;
			std::cin >> massive;
			LISTSET(massives, massive);
		}
		else {
			long long number;
			std::cin >> number;
			LISTSETSOF(massives, number, massives_of_numbers);
		}
	}
	return 0;
}
