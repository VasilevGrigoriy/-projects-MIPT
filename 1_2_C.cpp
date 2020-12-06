#include <iostream>
#include <vector>
#include <stdio.h>
#include <cstdlib> 
#include <ctime> 
#include <algorithm>

long long partition(std::vector<long long>& arr, long long first, long long end)
{
    long long target_index = (first + end) / 2;
    long long target_element = arr[target_index];
    long long i = first;
    long long j = end;
    while (i!= target_index && j!= target_index)
    {
        if (arr[i] > target_element && arr[j] <= target_element)
        {
            std::swap(arr[i], arr[j]);
            ++i;
            --j;
        }
        else if (arr[i] <= target_element && arr[j] <= target_element)
        {
            ++i;
        }
        else if (arr[i] <= target_element && arr[j] > target_element)
        {
            ++i;
            --j;
        }
        else if (arr[i] > target_element && arr[j] > target_element)
        {
            --j;
        }
    }
    if (i == target_index && j != target_index)
    {
        long long temp = j + 1;
        while (j != target_index)
        {
            if (arr[j] > target_element)
            {
                --temp;
                std::swap(arr[temp], arr[j]);
            }
            --j;
        }
        std::swap(arr[temp - 1], arr[target_index]);
        return (temp-1);
    }
    else if (i != target_index && j == target_index)
    {
        long long temp = i - 1;
        while (i != target_index)
        {
            if (arr[i] <= target_element)
            {
                ++temp;
                std::swap(arr[temp], arr[i]);
            }
            ++i;
        }
        std::swap(arr[temp + 1], arr[target_index]);
        return (temp + 1);
    }
    else
    {
        return target_index;
    }
}

int main()
{
    int n1, k1;
    std::scanf("%d%d", &n1,&k1);
    long long n = n1, k=k1;
    long long permament_k = k;
    std::vector<long long> massive;
    for (long long i = 0; i < n; ++i)
    {
        int x;
        std::scanf("%d",&x);
        long long z=x;
        massive.push_back(z);
    }
   
    long long first = 0;
    long long end = n - 1;
    while (end - first > 1)// Почему бы не выделить и этот цикл в функцию, чтобы сделать main лаконичее?
    {
        long long middle_element = partition(massive, first, end);
        if (permament_k == middle_element)
        {
            std::cout << massive[middle_element];
            return 0;
        }
        else if (k + 1 <= middle_element - first)
        {
            end = middle_element-1;
        }
        else
        {
            k = k - middle_element + first - 1;
            first = middle_element + 1;
            
        }
    }
    if (massive[first] > massive[end])
    {
        std::swap(massive[first], massive[end]);
    }
    int result = massive[k + first];
    std::printf("%d",result);
    
    return 0;
}


