#include <iostream>
#include <vector>
#include <string>
#include <cmath>
long long first_iter_cls = 0;
std::vector<long long> make_p(std::string& s) {
	std::vector<int> cnt(256, 0);
	std::vector<long long> p(s.size());
	for (int i = 0; i < s.size(); ++i) {
		++cnt[s[i]];
	}
	for (int i = 1; i <= 255; ++i) {
		cnt[i] += cnt[i - 1];
	}
	for (int i = s.size() - 1; i >= 0; --i) {
		p[--cnt[s[i]]] = i;
	}
	return p;
}
std::vector<long long> make_c(std::vector <long long>& p, std::string& s) {
	std::vector<long long> c(s.size());
	c[p[0]] = first_iter_cls;
	for (long long i = 1; i < s.size(); ++i) {
		if (s[p[i]] != s[p[i - 1]]) {
			++first_iter_cls;
		}
		c[p[i]] = first_iter_cls;
	}
	return c;
}
std::vector<std::pair<long long, long long>> make_suff_mass(std::vector<long long>& p, std::vector<long long>& c, std::string& s, long long cls) {
	std::vector<std::pair<long long, long long>> p_last;
	for (int k = 0; 1 << k < s.size(); ++k) {
		std::vector<std::pair<long long, long long>> p_extra(s.size());
		for (int i = 0; i < s.size(); ++i) {
			p_extra[i] = { i, (i + int(pow(2, k))) % s.size() };//???????
		}
		std::vector<std::pair<long long, long long>> p_extra_copy = p_extra;
		std::vector<int> cnt(cls + 1, 0);
		for (int i = 0; i < s.size(); ++i) {
			++cnt[c[p_extra[i].second]];
		}
		for (int i = 1; i <= cls; ++i) {
			cnt[i] += cnt[i - 1];
		}
		for (int i = s.size() - 1; i >= 0; --i) {
			p_extra_copy[--cnt[c[p_extra[i].second]]] = p_extra[i];
		}
		p_extra = p_extra_copy;
		std::vector<std::pair<long long, long long>> p_extra_copy2 = p_extra;
		std::vector<int> cnt2(cls + 1, 0);
		for (int i = 0; i < s.size(); ++i) {
			++cnt2[c[p_extra[i].first]];
		}
		for (int i = 1; i <= cls; ++i) {
			cnt2[i] += cnt2[i - 1];
		}
		for (int i = s.size() - 1; i >= 0; --i) {
			p_extra_copy2[--cnt2[c[p_extra[i].first]]] = p_extra[i];
		}
		p_extra = p_extra_copy2;
		std::vector<long long> c_new(s.size(), 0);
		long long cls_n = 0;
		c_new[p_extra[0].first] = cls_n;
		for (int i = 1; i < s.size(); ++i) {
			if (c[p_extra[i].first] != c[p_extra[i - 1].first] || c[p_extra[i].second] != c[p_extra[i - 1].second]) {
				++cls_n;
			}
			c_new[p_extra[i].first] = cls_n;
		}
		c = c_new;
		cls = cls_n;
		p_last = p_extra;
	}
	return p_last;
}

int main()
{
	std::string s;
	std::cin >> s;
	s += "$";
	std::vector<long long> p = make_p(s);
	std::vector<long long> c = make_c(p, s);
	std::vector<std::pair<long long, long long>> suff_mass = make_suff_mass(p, c, s, first_iter_cls);
	for (int i = 1; i < suff_mass.size(); i++) {
		std::cout << suff_mass[i].first + 1 << ' ';
	}
	return 0;
}