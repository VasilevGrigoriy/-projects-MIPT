#include <iostream>
#include <vector>
#include <algorithm>


int main()
{
	int n, k;
	std::cin >> n >> k;
	std::vector<int> massive(k);
	for (int i = 0; i < k; ++i)
	{
		int x;
		std::cin >> x;
		massive[i] = x;
	}
	sort(massive.begin(), massive.end());
	for (int i = 0; i < n - k; ++i)
	{
		int next_element;
		std::cin >> next_element;
		if (next_element < massive[k - 1])
		{
			massive[k - 1] = next_element;
			for (int j = k - 1; j > 0; --j)
			{
				if (massive[j] < massive[j - 1])
				{
					std::swap(massive[j], massive[j - 1]);
				}
				else
				{
					break;
				}
			}
		}
	}
	for (int i = 0; i < k; ++i)
	{
		std::cout << massive[i] << ' ';
	}
}