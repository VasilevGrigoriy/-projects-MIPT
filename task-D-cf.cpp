#include <iostream>
#include <map>
#include <vector>
#include <set>
#include <string>
#include <queue>

enum{
  WHITE = 0,
  GREY = 1,
  BLACK = 2,
  ALPH_SIZE = 26,
  FIRST = 97
};



struct node {
  std::vector<int> to;
  int how_many_words;
  bool term;
  bool is_ready;
  node() {
    to = std::vector<int>(ALPH_SIZE,-1);
    term = false;
    how_many_words = 0;
    is_ready = false;
  }
};

struct Bor {
  std::vector<node> nodes;
  Bor() {
    nodes.push_back(node());
  }
  void add(const std::string& s) {
    int v = 0;
    for (int i = 0; i < s.size(); ++i) {
      if (nodes[v].to[s[i] - FIRST] == -1) {
        nodes.push_back(node());
        nodes[v].to[s[i] - FIRST] = int(nodes.size()) - 1;
      }
      v = nodes[v].to[s[i] - FIRST];
    }
    nodes[v].term = true;
    nodes[v].how_many_words++;
  }
  void is_cycled(std::vector<std::string>& sorted_strings, std::string& curr, int v = 0){
    for (int i = 0;i<=25;i++) {
      if(nodes[v].to[char(i)] != -1) {
        char go = char(i);
        curr+=char(go + FIRST);
        if (nodes[nodes[v].to[go]].term) {
          for (int i = 0; i < nodes[nodes[v].to[go]].how_many_words; i++) {
            sorted_strings.push_back(curr);
          }
        }
        is_cycled( sorted_strings, curr, nodes[v].to[go]);
        curr.pop_back();
      }
    }
  }
};

void make_for_pars(Bor& bor, std::string& s) {
  int i = 0;
  for (int i = 0; i < s.size(); ++i) {
    if (s[i] == '.') continue;
    else {
      std::string s_temp = "";
      while (i < s.size() && s[i] != '.') {
        s_temp += s[i];
        i++;
      }
      bor.add(s_temp);
    }
  }
}

int main() {
  std::string s;
  std::cin >> s;
  std::vector<std::string> sorted_strings;
  std::string ss = "";
  Bor bor = Bor();
  make_for_pars(bor, s);
  bor.is_cycled(sorted_strings, ss, 0);
  int j =0;
  for(int i=0;i<s.size();i++){
    if(s[i] != '.') {
      std::cout << sorted_strings[j];
      j++;
      while(i < s.size() && s[i]!='.') i++;
      i--;
      continue;
    }
    std::cout<<s[i];
  }
  return 0;
}
