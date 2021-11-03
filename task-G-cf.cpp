#include <iostream>
#include <vector>
#include <queue>
struct node {
    std::vector<int> to;
    bool term;
    int link;
    int len;
    std::vector<int> words;
    node() {
        to = std::vector<int>(2, -1);
        term = false;
        link = -1;
        len = 0;
        words = std::vector<int>(0);
    }
};
int num(char c) {
    if (c == '0') return 0;
    else return 1;
}
struct Bor {
    std::vector<node> t;
    bool cycle = false;
    Bor() {
        t.push_back(node());
    }
    void add(const std::string& s, int index) {
        int v = 0;
        for (int i = 0; i < s.size(); ++i) {
            if (t[v].to[num(s[i])] == -1) {
                t.push_back(node());
                t[v].to[num(s[i])] = int(t.size()) - 1;
            }
            v = t[v].to[num(s[i])];
        }
        t[v].term = true;
        t[v].len = s.size();
        t[v].words.push_back(index);
    }
    void Aho_Corasick() {
        t[0].link = 0;
        for (int c = 0; c < 2; ++c) {
            if (t[0].to[c] != -1) {
                continue;
            }
            t[0].to[c] = 0;
        }
        std::queue<int> q;
        q.push(0);
        while (!q.empty()) {
            int v = q.front();
            q.pop();
            for (int c = 0; c < 2; ++c) {
                int u = t[v].to[c];
                if (t[u].link != -1) {
                    continue;
                }
                t[u].link = (v == 0 ? 0 : t[t[v].link].to[c]);
                for (int d = 0; d < 2; ++d) {
                    if (t[u].to[d] != -1) {
                        continue;
                    }
                    t[u].to[d] = t[t[u].link].to[d];
                }
                q.push(u);
            }
        }
    }
    void find_patterns(int v, int pos, std::vector<std::vector<int>>& ans) {
        while (t[v].link != 0) {
            if (t[v].term) {
                for (auto i : t[v].words) {
                    ans[i].push_back(pos - t[v].len + 1);
                }
            }
            v = t[v].link;
        }
        if (t[v].term) {
            for (auto i : t[v].words) {
                ans[i].push_back(pos - t[v].len + 1);
            }
        }
    }
    bool mark_suns_of_terminated(int v = 0) {
        if (t[v].link == 0) return t[v].term;
        if (t[v].term) {
            return true;
        }
        else {
            t[v].term = mark_suns_of_terminated(t[v].link);
            return t[v].term;
        }
    }
    void is_cycled(std::vector<int>& color, int v = 0) {
        color[v] = 1;
        for (int go : t[v].to) {
            if (go == 0) return;
            if (t[go].term) continue;
            if (color[go] == 1) {
                cycle = true;
                break;
            }
            else if (color[go] != 0) continue;
            else {
                is_cycled(color, go);
            }
        }
        color[v] = 2;
    }
};

int main()
{
    int N;
    std::cin >> N;
    Bor bor = Bor();
    for (int i = 0; i < N; i++) {
        std::string s;
        std::cin >> s;
        bor.add(s, i);
    }
    bor.Aho_Corasick();
    for (int i = bor.t.size() - 1; i >= 0; i--) {
        bor.mark_suns_of_terminated(i);
    }
    std::vector<int> color(bor.t.size(), 0);
    bor.is_cycled(color, 0);
    if (bor.cycle) std::cout << "TAK";
    else std::cout << "NIE";
    return 0;
}
