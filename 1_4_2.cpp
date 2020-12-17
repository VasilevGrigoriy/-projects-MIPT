#include <iostream>
#include <vector>
#include <cmath>
using namespace std;
void create_tree(vector<long long>& v, vector<long long>& tree, long long current_knot, long long tl, long long tr) {
	if (tl == tr) {
		tree[current_knot] = v[tl];
	}
	else {
		long long tm = (tl + tr) / 2;
		create_tree(v, tree, current_knot * 2, tl, tm);
		create_tree(v, tree, current_knot * 2 + 1, (tm + 1), tr);
	    tree[current_knot] = tree[current_knot * 2] + tree[current_knot * 2 + 1];
	}
}
long long main_work(vector<long long>& tree, long long current_knot, long long tl, long long tr, long long l, long long r) {
	if (l > r)
		return 0;
	if (l == tl && r == tr)
		return tree[current_knot];
	long long tm = (tl + tr) / 2;
	return main_work(tree, current_knot * 2, tl, tm, l, min(r, tm))
			+ main_work(tree, current_knot * 2 + 1, tm + 1, tr, max(l, tm + 1), r);
	
}
void update(vector< long long>& tree, long long current_knot, long long tl, long long tr, long long pos, long long new_val) {
	if (tl == tr) {
		if (tl % 2 != 0)
			tree[current_knot] = new_val;
		else
			tree[current_knot] = -new_val;
	}
	else {
		long long tm = (tl + tr) / 2;
		if (pos <= tm)
			update(tree, current_knot * 2, tl, tm, pos, new_val);
		else
			update(tree, current_knot * 2 + 1, tm + 1, tr, pos, new_val);
		tree[current_knot] = tree[current_knot * 2] + tree[current_knot * 2 + 1];
	}
}
int main()
{
	long long n;
	cin >> n;
	vector<long long> v(n + 1, 0);
	vector<long long> tree(n * 4 + 4);
	for (long long i = 1; i <= n; ++i) {
		long long x;
		cin >> x;
		if (i % 2 == 0) x = -x;
		v[i] = x;
	}
	create_tree(v, tree, 1, 1, n);
	long long m;
	cin >> m;
	for (long long i = 0; i < m; ++i) {
		long long temp,x, y;
		cin >>temp>> x >> y;
		if (temp!=0) {
			long long z = main_work(tree, 1, 1, n, x, y);
				if (x % 2 == 0) cout << -z << endl;
				else cout << z << endl;
		}
		else {
			update(tree, 1, 1, n, x, y);
		}
	}
}
