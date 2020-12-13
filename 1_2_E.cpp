#include<iostream>
#include<algorithm>
#include<vector>
void siftUP(std::vector < long long>& massive, long long i) {
    //поставить элемент на сове место c "движением вверх"
    while (i > 1 && massive[i] >= massive[i / 2]) {
        std::swap(massive[i], massive[i / 2]);
        i /= 2;
    }
}
void siftDOWN(std::vector < long long>& massive, long long i) {
    //также поставить элемент на свое место, только "с движение вниз"
    while (2 * i <= massive.size() - 1) {
        int j = -1;
        if (massive[2 * i] >= massive[i])j = 2 * i;
        if (2 * i + 1 <= massive.size() - 1 && massive[2 * i + 1] >= massive[i] && (j == -1 || massive[2 * i + 1] >= massive[2 * i])) j = 2 * i + 1;
        if (j == -1) break;
        else {
            std::swap(massive[i], massive[j]);
            i = j;
        }
    }
}
void insert(std::vector <long long>& massive, long long x, long long i) {
    massive.push_back(x);
    siftUP(massive, massive.size() - 1); 
    //храню максимум в первой ячейке
}

void extractMAX(std::vector <long long>& massive) {
    //удаляю максимум
    std::swap(massive[1], massive[massive.size() - 1]);
    massive.pop_back();
    siftDOWN(massive, 1);
}
int main() {
    long long n;
    int k;
    std::cin >> n >> k;
    std::vector <long long> massive;
    for (long long i = 0; i < k; ++i) {
        long long x;
        std::cin >> x;
        insert(massive, x, i);
    }
    if (n > k) {
        //обрабатываю оставшиеся элементы
        for (long long i = k; i < n; ++i) {
            long long x;
            std::cin >> x;
            if (x < massive[1]) {
                extractMAX(massive);
                insert(massive, x, i);
            }
        }
    }
    sort(massive.begin(), massive.end());
    for (int i = 0; i < massive.size(); ++i) {
        std::cout << massive[i] << ' ';
    }
    return 0;
}
