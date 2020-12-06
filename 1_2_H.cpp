#include <iostream>
#include <vector>
#include <algorithm>

void Sort(std::vector<std::pair<long long,long long>>& arr, int i)
{
    if (i == 9)
    {
        return;
    }
    std::vector<long long> cnt(256, 0);// Для каждой итерации создавать новый массив плохая идея, так как он в конце не нужен
    // А в памяти продолжает висеть пока все рекурсии не закончатся
    for (int i = 0; i < arr.size(); ++i)
    {
        ++cnt[arr[i].first % 256];
    }
    for (int i = 1; i < 256; ++i)
    {
        cnt[i] += cnt[i - 1];
    }
    std::vector<std::pair<long long, long long>> ans(arr.size());//Аналогично
    for (int i = arr.size() - 1; i >= 0; --i)
    {
        ans[cnt[arr[i].first % 256]-1].first = arr[i].first/256;
        ans[cnt[arr[i].first % 256]-1].second = arr[i].second;
        --cnt[arr[i].first%256];
    }
    arr = ans;
    Sort(arr, i + 1);
}

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(NULL);
    std::cout.tie(NULL);
    long long n;
    std::cin >> n;
    std::vector<std::pair<long long,long long>> numbers;

    for (int i = 0; i < n; ++i)
    {
        long long z;
        std::cin >> z;
        numbers.push_back({ z,z });
    }
    Sort(numbers, 1);//Для таких вещей обычно делают либо ещё одну "декорирующую" функцию с нужными парамемтрами, либо параметры по умолчанию
    for (long long i = 0; i < n; ++i)
    {
        std::cout << numbers[i].second << ' ';
    }
    
    return 0;
}

