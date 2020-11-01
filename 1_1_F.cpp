#include <fstream>
#include <iostream>
#include <algorithm>
#include <vector>

long long mergesort_counting_inverses(std::vector<long long>& arr, long long begin_element, long long end_element)
{
    if (end_element == begin_element) return 0;
    long long middle_element = (begin_element + end_element) / 2;
    long long first_count = mergesort_counting_inverses(arr, begin_element, middle_element) 
        + mergesort_counting_inverses(arr, middle_element + 1, end_element);//Длинные строчки лучше разбивать
    long long second_count = 0;
    int i = 0;
    int j = 0;
    int pozishion = begin_element;
    std::vector<long long> ar_levo(middle_element - begin_element + 1);//Аррр лево!
    for (int u = 0; u < middle_element - begin_element + 1; ++u)
    {
        ar_levo[u] = arr[begin_element + u];
    }
    std::vector<long long> ar_pravo(end_element - middle_element);//Аррр право!
    for (int u = 0; u < end_element - middle_element; ++u)
    {
        ar_pravo[u] = arr[middle_element + 1 + u];
    }
    while (i < middle_element - begin_element + 1 && j < end_element - middle_element)
    {
        if (ar_levo[i] < ar_pravo[j])
        {
            arr[pozishion] = ar_levo[i];
            pozishion++;
            second_count += j;
            i++;
        }
        else
        {
            arr[pozishion] = ar_pravo[j];
            pozishion++;
            j++;
        }
    }
    while (i < middle_element - begin_element + 1)
    {
        arr[pozishion] = ar_levo[i];
        pozishion++;
        i++;
        second_count += end_element - middle_element;
    }
    while (j < end_element - middle_element)
    {
        arr[pozishion] = ar_pravo[j];
        pozishion++;
        j++;
    }
    return first_count + second_count;
}

int main()
{
    long long n;
    std::ifstream fin;
    fin.open("inverse.in");
    fin >> n;
    std::vector<long long> massive(n);
    for (int i = 0; i < n; ++i)
    {
        long long x;
        fin >> x;
        massive[i] = x;
    }
    fin.close();
    long long count = 0;
    count = mergesort_counting_inverses(massive, 0, n - 1);
    std::ofstream fout;
    fout.open("inverse.out");
    fout << count << '\n';
    fout.close();
    return 0;
}
