#include <string>
#include <iostream>
#include <algorithm>

struct AVL {
	long long key;
	long long height;
	long long elements_down;
	AVL* left;
	AVL* right;
	AVL(long long k) {
		key = k;
		left = 0;
		right = 0;
		height = 1;
		elements_down = 1;
	}
};
long long height_1(AVL* knot) {
	if (knot == 0) return 0;
	else return knot->height;
}
long long elements_downs(AVL* knot) {
	if (knot == 0) return 0;
	else return knot->elements_down;
}
long long balance_factor(AVL* knot) {
	return height_1(knot->left) - height_1(knot->right);
}
void height_and_elements_downs(AVL* knot) {
	knot->height = std::max(height_1(knot->right), height_1(knot->left)) + 1;
	knot->elements_down = elements_downs(knot->left) + elements_downs(knot->right) + 1;
}

AVL* rotateright(AVL* knot_current) {
	AVL* knot_left = knot_current->left;
	knot_current->left = knot_left->right;
	knot_left->right = knot_current;
	height_and_elements_downs(knot_current);
	height_and_elements_downs(knot_left);
	return knot_left;
}
AVL* rotateleft(AVL* knot_current) {
	AVL* knot_right = knot_current->right;
	knot_current->right = knot_right->left;
	knot_right->left = knot_current;
	height_and_elements_downs(knot_current);
	height_and_elements_downs(knot_right);

	return knot_right;
}
AVL* balancing(AVL* knot) {
	height_and_elements_downs(knot);
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
long long MAX_K_STATISTIC(AVL* knot, long long k) {
	long long elements_down_right;
	if (knot->right == 0) {
		elements_down_right = 0;
	}
	else {
		elements_down_right = knot->right->elements_down;
	}
	if (elements_down_right + 1 == k) return knot->key;
	else if (elements_down_right + 1 > k) MAX_K_STATISTIC(knot->right, k);
	else if (elements_down_right + 1 < k) MAX_K_STATISTIC(knot->left, k - elements_down_right - 1);
}
int main()
{
	std::ios::sync_with_stdio(0);
	std::cin.tie(0);
	std::cout.tie(0);
	std::string s = "";
	long long n;
	std::cin >> n;
	bool b = false;
	AVL* root = 0;
	while (n != 0) {
		--n;
		std::cin >> s;
		if (s[0] == '+') {
			long long element;
			std::cin >> element;
			if (!b) {
				root = new AVL(element);
				b = true;
			}
			else if (!exists(root, element)) root = insert(root, element);
		}
		else if (s[0] == '1') {
			long long element;
			std::cin >> element;
			if (!b) {
				root = new AVL(element);
				b = true;
			}
			else if (!exists(root, element)) root = insert(root, element);
		}
		else if (s[0] == '-') {
			long long element;
			std::cin >> element;
			if (b) root = delete_key(root, element);
		}
		else if (s[0] == '0') {
			long long statistic_from_max;
			std::cin >> statistic_from_max;
			std::cout << MAX_K_STATISTIC(root, statistic_from_max) << '\n';
		}
	}
	return 0;
}
