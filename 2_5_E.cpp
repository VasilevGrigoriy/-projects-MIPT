#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <list>
long long m = 100001;
long long p = 997;
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
void work_put(std::string& name, std::string& value, std::vector<std::list<std::pair<std::string, std::string>>>& v) {
    long long hash = hashing(name, p, m);
    if (v[hash].size() == 0) {
        v[hash].push_back({ name,value });
    }
    else {
        bool flag = 0;
        auto i = v[hash].end();
        i--;
        for (auto it = i;; --it) {
            if (it->first == name) {
                flag = true;
                it->second = value;
                break;
            }
            if (it == v[hash].begin()) {
                break;
            }
        }
        if (!flag) {
            v[hash].push_back({ name,value });
        }
    }
}

void work_delete(std::string& name, std::vector<std::list<std::pair<std::string, std::string>>>& v) {
    long long hash = hashing(name, p, m);
    if (v[hash].size() == 0) {
        return;
    }
    else {
        bool flag = 0;
        auto i = v[hash].end();
        i--;
        for (auto it = i;; --it) {
            if (it->first == name) {
                v[hash].erase(it);
                return;
            }
            if (it == v[hash].begin()) {
                return;
            }
        }
    }
}

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(0);
    std::cout.tie(0);
    
    std::vector<std::list<std::pair<std::string, std::string>>> v(m);
    
    std::ifstream fin;
    fin.open("map.in");
    std::ofstream fout;
    fout.open("map.out");

    std::string what_to_do;
    while (fin >> what_to_do) {
        if (what_to_do == "put") {
            std::string name;
            std::string value;
            fin >> name >> value;
            long long hash = hashing(name, p, m);
            work_put(name, value, v);
        }
        else if (what_to_do == "get") {
            std::string name;
            fin >> name;
            long long hash = hashing(name, p, m);
            if (v[hash].size() == 0) {
                fout << "none" << '\n';
            }
            else {
                bool flag = 0;
                auto i = v[hash].end();
                i--;
                for (auto it = i;; --it) {
                    if (it->first == name) {
                        flag = 1;
                        fout << it->second << '\n';
                        break;
                    }
                    if (it == v[hash].begin()) {
                        break;
                    }
                }
                if (!flag) {
                    fout << "none" << '\n';
                }
            }
        }
        else {
            std::string name;
            fin >> name;
            work_delete(name, v);
        }
    }
    fin.close();
    fout.close();
    return 0;
}