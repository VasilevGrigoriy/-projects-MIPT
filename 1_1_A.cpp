#include <iostream>
#include <string>

void push(long long*& arr, long long& size, long long value)
{
    long long* newArray = new long long[size + 1];
    for (int i = 0; i < size; ++i)
    {
        newArray[i] = arr[i];
    }
    newArray[size] = value;
    ++size;
    delete[] arr;
    arr = newArray;
}
void pop(long long*& arr, long long& size)
{
    long long* newArray = new long long[size - 1];
    for (int i = 0; i < size - 1; ++i)
    {
        newArray[i] = arr[i];
    }
    --size;
    delete[] arr;
    arr = newArray;
}
void clear(long long*& arr, long long& size)
{
    long long* newArray = new long long[0];
    size = 0;
    delete[] arr;
    arr = newArray;
}
int main()
{
    long long size = 0;
    long long* arr = new long long[size];
    std::string s = "hi";
    while (s != "exit")
    {
        getline(std::cin, s);
        if (s.substr(0, 4) == "push")
        {
            std::string s1 = s.substr(5, s.size() - 5);
            long long next_element = atoi(s1.c_str());
            push(arr, size, next_element);
            std::cout << "ok" << std::endl;
        }
        else if (s == "pop")
        {
            if (size == 0)
            {
                std::cout << "error" << std::endl;
            }
            else
            {
                std::cout << arr[size - 1] << std::endl;
                pop(arr, size);
            }
        }
        else if (s == "back")
        {
            if (size == 0)
            {
                std::cout << "error" << std::endl;
            }
            else
            {
                std::cout << arr[size - 1] << std::endl;
            }
        }
        else if (s == "size")
        {
            std::cout << size << std::endl;
        }
        else if (s == "clear")
        {
            clear(arr, size);
            std::cout << "ok" << std::endl;
        }

    }
    delete[] arr;
    arr = nullptr;
    std::cout << "bye" << std::endl;

    return 0;
}