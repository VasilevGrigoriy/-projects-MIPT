#include <iostream>
#include <queue>
#include<string>
#include<stack>


int main()
{
	std::ios::sync_with_stdio(false);
	int n;
	std::string s;
	getline(std::cin, s);
	n = atoi(s.c_str());
	std::deque <int> levaya_polovina_queue;
	std::deque<int> pravaya_polovina_queue;
	int size_queue = 0;
	while (n != 0)
	{
		--n;
		std::string snext;
		getline(std::cin, snext);
		if (snext.substr(0, 1) == "+")
		{
			std::string next_element_in_string = snext.substr(2, snext.size() - 2);
			int next_element = atoi(next_element_in_string.c_str());
			levaya_polovina_queue.push_back(next_element);
			++size_queue;
			if (size_queue % 2 == 0)
			{
				if (levaya_polovina_queue.size() > pravaya_polovina_queue.size())
				{
					pravaya_polovina_queue.push_back(levaya_polovina_queue.front());
					levaya_polovina_queue.pop_front();
				}
			}
			if (size_queue % 2 != 0 && pravaya_polovina_queue.size() != levaya_polovina_queue.size() + 1)
			{
				pravaya_polovina_queue.push_back(levaya_polovina_queue.front());
				levaya_polovina_queue.pop_front();
			}
		}
		else if (snext.substr(0, 1) == "-")
		{
			std::cout << pravaya_polovina_queue.front() << std::endl;
			pravaya_polovina_queue.pop_front();
			size_queue--;
			if (size_queue % 2 == 0)
			{
				if (levaya_polovina_queue.size() > pravaya_polovina_queue.size())
				{
					pravaya_polovina_queue.push_back(levaya_polovina_queue.front());
					levaya_polovina_queue.pop_front();
				}
			}
			if (size_queue % 2 != 0 && pravaya_polovina_queue.size() != levaya_polovina_queue.size() + 1)
			{
				pravaya_polovina_queue.push_back(levaya_polovina_queue.front());
				levaya_polovina_queue.pop_front();
			}
		}
		else
		{
			std::string next_element_in_string = snext.substr(2, snext.size() - 2);
			int next_element = atoi(next_element_in_string.c_str());
			if (size_queue % 2 == 0)
			{
				pravaya_polovina_queue.push_back(next_element);
			}
			else
			{
				levaya_polovina_queue.push_front(next_element);
			}
			++size_queue;
		}

	}
	return 0;
}