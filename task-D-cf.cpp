#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

int main()
{
    std::string s;
    std::cin >> s;
    std::vector<std::string> for_pars;
    std::vector<std::string> ans;
    int i = 0;
    for (int i = 0; i < s.size(); ++i) {
        if (s[i] == '.') ans.push_back(".");
        else {
            ans.push_back("_");
            std::string s_temp = "";
            while (i < s.size() && s[i] != '.') {
                s_temp += s[i];
                i++;
            }
            if (i < s.size() && s[i] == '.') ans.push_back(".");
            if (i == s.size() && s[i - 1] == '.') ans.push_back(".");
            for_pars.push_back(s_temp);
        }

    }
    int j = 0;
    std::sort(for_pars.begin(), for_pars.end());
    for (int i = 0; i < ans.size(); i++) {
        if (ans[i] == "_") {
            ans[i] = for_pars[j];
            j++;
        }
    }
    std::string res = "";
    for (int i = 0; i < ans.size(); i++) {
        res += ans[i];
    }
    std::cout << res;
}

