#include <iostream>
#include <vector>
#include <string>

int main()
{
    std::vector<long long> stack;
    std::string s = "hi";

    while (s != "exit")
    {
        getline(std::cin, s);
        if (s.substr(0, 4) == "push")
        {
            std::string s1 = s.substr(5, s.size() - 5);
            long long next_element = atoi(s1.c_str());
            stack.push_back(next_element);
            std::cout << "ok" << std::endl;
        }
        else if (s == "pop")
        {
            if (stack.size() == 0)
            {
                std::cout << "error" << std::endl;
            }
            else
            {
                std::cout << stack[stack.size() - 1] << std::endl;
                stack.pop_back();
            }
        }
        else if (s == "back")
        {
            if (stack.size() == 0)
            {
                std::cout << "error" << std::endl;
            }
            else
            {
                std::cout << stack[stack.size() - 1] << std::endl;
            }
        }
        else if (s == "size")
        {
            std::cout << stack.size() << std::endl;
        }
        else if (s == "clear")
        {
            stack.clear();
            std::cout << "ok" << std::endl;
        }
    }
    std::cout << "bye" << std::endl;

    return 0;
}