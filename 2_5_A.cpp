#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <cmath>
const long long m = 400;
const long long p = 997;
long long hashing(std::string& s, long long p, long long m) {
    long long hash = 0;
    int i = 0;
    long long temp = 1;
    while (i != s.size()) {
        int elem = s[i];
        hash += elem * temp;
        temp *= p;
        temp %= m;
        hash %= m;
        i++;
    }
    return hash;
}

void main_work(int what_increm, std::string name, std::vector<std::list<std::pair<std::string, int>>>& v) {
    int hash = hashing(name, p, m);
    if (v[hash].size() == 0) {
        v[hash].push_back({ name,what_increm });
        std::cout << what_increm << '\n';
    }
    else {
        bool flag = 0;
        auto i = v[hash].end();
        i--;
        for (auto it = i;; --it) {
            if (it->first == name) {
                flag = true;
                it->second += what_increm;
                std::cout << it->second << '\n';
                break;
            }
            if (it == v[hash].begin()) {
                break;
            }
        }
        if (!flag) {
            v[hash].push_back({ name,what_increm });
            std::cout << what_increm << '\n';
        }
    }
}

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(NULL);
    std::cout.tie(NULL);
    std::vector<std::list<std::pair<std::string, int>>> v(m);
    std::string name = "";
    while (std::cin >> name) {
        int what_increm = 0;
        std::cin >> what_increm;
        main_work(what_increm, name, v);
    }
    return 0;
}
