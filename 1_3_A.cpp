#include <string>
#include <iostream>
#include <algorithm>

struct AVL {
	long long key;
	long long height;
	AVL* left;
	AVL* right;
	AVL(long long k) {
		key = k;
		left = 0;
		right = 0;
		height = 1;
	}
};
long long height_1(AVL* knot) {
	if (knot == 0) return 0;
	else return knot->height;
}
long long balance_factor(AVL* knot) {
	return height_1(knot->left) - height_1(knot->right);
}
void height(AVL* knot) {
	knot->height = std::max(height_1(knot->right), height_1(knot->left)) + 1;
}
AVL* rotateright(AVL* knot_current) {
	AVL* knot_left = knot_current->left;
	knot_current->left = knot_left->right;
	knot_left->right = knot_current;
	height(knot_current);
	height(knot_left);
	return knot_left;
}
AVL* rotateleft(AVL* knot_current) {
	AVL* knot_right = knot_current->right;
	knot_current->right = knot_right->left;
	knot_right->left = knot_current;
	height(knot_current);
	height(knot_right);
	return knot_right;
}
AVL* balancing(AVL* knot) {
	height(knot);
	if (balance_factor(knot) == 2) {
		if (balance_factor(knot->left) < 0) {
			knot->left = rotateleft(knot->left);
		}
		return rotateright(knot);
	}
	if (balance_factor(knot) == -2) {
		if (balance_factor(knot->right) > 0) {
			knot->right = rotateright(knot->right);
		}
		return rotateleft(knot);
	}
	return knot;
}
AVL* insert(AVL* knot, long long k) {
	if (knot == 0) return new AVL(k);
	else {

		if (k < knot->key) {
			knot->left = insert(knot->left, k);
		}
		else {
			knot->right = insert(knot->right, k);
		}
	}
	return balancing(knot);
}
AVL* findmin(AVL* knot) {
	if (knot->left) return findmin(knot->left);
	else return knot;
}
AVL* findmax(AVL* knot) {
	if (knot->right) return findmax(knot->right);
	else return knot;
}
AVL* deletemin(AVL* knot) {
	if (knot->left == 0)
		return knot->right;
	knot->left = deletemin(knot->left);
	return balancing(knot);
}
AVL* delete_key(AVL* knot, long long k) {
	if (knot == 0) return 0;
	if (k < knot->key) {
		knot->left = delete_key(knot->left, k);
	}
	else if (k > knot->key) {
		knot->right = delete_key(knot->right, k);
	}
	else {
		AVL* knot_left = knot->left;
		AVL* knot_right = knot->right;
		delete knot;
		if (knot_right == 0) return knot_left;
		AVL* knot_min = findmin(knot_right);
		knot_min->right = deletemin(knot_right);
		knot_min->left = knot_left;
		return balancing(knot_min);
	}
	return balancing(knot);
}
bool exists(AVL* knot, long long k) {
	if (knot == 0) return false;
	else {

		if (knot->key == k) return true;
		else if (knot->key > k) {
			return exists(knot->left, k);
		}
		else if (knot->key < k) {
			return exists(knot->right, k);
		}
	}
}
long long next(AVL* knot, long long k, AVL* tmp) {
	if (knot == 0) {
		if (tmp == 0) return 1000000001;
		else return tmp->key;
	}
	else {
		if (k > knot->key) {
			return next(knot->right, k, tmp);
		}
		if (k < knot->key) {
			tmp = knot;
			return next(knot->left, k, tmp);
		}
		if (k == knot->key) {
			if (knot->right) return findmin(knot->right)->key;
			else {
				if (tmp != 0) return tmp->key;
				else return 1000000001;
			}
		}
	}
	return 0;
}


long long prev(AVL* knot, long long k, AVL* tmp) {
	if (knot == 0) {
		if (tmp == 0) return 1000000001;
		else return tmp->key;
	}
	else {
		if (k < knot->key) {
			return prev(knot->left, k, tmp);
		}
		if (k > knot->key) {
			tmp = knot;
			return prev(knot->right, k, tmp);
		}
		if (k == knot->key) {
			if (knot->left) return findmax(knot->left)->key;
			else {
				if (tmp != 0) return tmp->key;
				else return 1000000001;
			}
		}
	}
	return 0;
}
int main()
{
	std::ios::sync_with_stdio(0);
	std::cin.tie(0);
	std::cout.tie(0);
	std::string s = "";
	bool b = false;
	AVL* root = 0;
	while (std::cin >> s) {
		if (s[0] == 'i') {
			long long x = 0;
			std::cin >> x;
			if (!b) {
				root = new AVL(x);
				b = true;
			}
			else if (!exists(root, x)) root = insert(root, x);
		}
		if (s[0] == 'd') {
			long long x = 0;
			std::cin >> x;
			if (b) root = delete_key(root, x);
		}
		if (s[0] == 'e') {
			long long x = 0;
			std::cin >> x;
			if (exists(root, x) && b) std::cout << "true" << std::endl;
			else std::cout << "false" << std::endl;
		}
		if (s[0] == 'p') {
			long long x = 0;
			std::cin >> x;
			if (prev(root, x, 0) == 1000000001 || !b) std::cout << "none" << std::endl;
			else std::cout << prev(root, x, 0) << std::endl;
		}
		if (s[0] == 'n') {
			long long x = 0;
			std::cin >> x;
			if (next(root, x, 0) == 1000000001 || !b) std::cout << "none" << std::endl;
			else std::cout << next(root, x, 0) << std::endl;
		}
	}
	return 0;
}
