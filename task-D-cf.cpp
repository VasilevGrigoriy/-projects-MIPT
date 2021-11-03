#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
std::vector<std::string> make_for_pars(std::string& s){
    std::vector<std::string> for_pars;
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
}
std::string make_res(std::vector<std::string>& for_pars, std::string& s){
    int i = 0;
    int j = 0;
    std::vector<std::string> ans;
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
}
int main()
{
    std::string s;
    std::cin >> s;
    std::vector<std::string> for_pars = make_for_pars(s);
    std::string res = make_res(for_pars, s);
    std::cout << res;
}

