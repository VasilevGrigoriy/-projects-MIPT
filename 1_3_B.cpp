#include <algorithm>
#include <iostream>
#include <string>

struct splay {
    long long key;
    splay* left;
    splay* right;
    splay* parent;
    long long sum;
    splay(long long h) {
        key = h;
        sum = h;
        left = 0;
        right = 0;
        parent = 0;
    }
};
long long sum(splay* knot) {
    if (knot == 0) return 0;
    else return knot->sum;
}
splay* parent(splay* knot) {
    if (knot == 0) return 0;
    else return knot->parent;
}
splay* grand_parent(splay* knot) {
    if (knot != 0 && parent(knot) != 0) return parent(parent(knot));
    else return 0;
}
void returning_parent(splay* knot_child, splay* knot_parent) {
    if (knot_child != 0) {
        knot_child->parent = knot_parent;
    }
}
void get_parent(splay* knot) {
    if (knot == 0) return;
    returning_parent(knot->left, knot);
    returning_parent(knot->right, knot);
}
void get_sum(splay* knot) {
    if (knot == 0) return;
    else knot->sum = sum(knot->left) + sum(knot->right) + knot->key;
}
void zig(splay* knot) {
    if (knot == 0) return;
    splay* par = parent(knot);
    splay* grandpar = grand_parent(knot);
    splay* right_tree = knot->right;
    if (par != 0) {
        par->left = right_tree;
        get_parent(par);
        get_sum(par);
        knot->right = par;
        get_parent(knot);
        get_sum(knot);
        if (grandpar != 0) {
            knot->parent = grandpar;
            if (grandpar->left == par) grandpar->left = knot;
            else grandpar->right = knot;
        }
        else knot->parent = 0;
    }
}
void zag(splay* knot) {
    if (knot == 0) return;
    splay* par = parent(knot);
    splay* grandpar = grand_parent(knot);
    splay* left_tree = knot->left;
    if (par != 0) {
        par->right = left_tree;
        get_parent(par);
        get_sum(par);
        knot->left = par;
        get_parent(knot);
        get_sum(knot);
        if (grandpar != 0) {
            knot->parent = grandpar;
            if (grandpar->left == par) grandpar->left = knot;
            else grandpar->right = knot;
        }
        else knot->parent = 0;
    }
}
splay* SPLAY(splay* knot) {
    while (knot->parent != 0) {
        splay* par = parent(knot);
        splay* grandpar = grand_parent(knot);
        if (grandpar == 0) {
            if (knot == par->left) {
                zig(knot);
            }
            else {
                zag(knot);
            }
        }
        else {
            if (knot == par->left && par == grandpar->left) {
                zig(knot);
                zig(knot);
            }
            else if (knot == par->right && par == grandpar->right) {
                zag(knot);
                zag(knot);
            }
            else if (knot == par->left && par == grandpar->right) {
                zig(knot);
                zag(knot);
            }
            else {
                zag(knot);
                zig(knot);
            }
        }
    }
    return knot;
}
splay* finding_element(splay* knot, long long element) {
    if (element > knot->key && knot->right == 0) return 0;
    if (element < knot->key && knot->left == 0) return 0;
    if (element > knot->key && knot->right != 0) return finding_element(knot->right, element);
    if (element < knot->key && knot->left != 0) return finding_element(knot->left, element);
    if (element == knot->key) return knot;
}
splay* findmin(splay* knot) {
    if (knot->left != 0) return findmin(knot->left);
    else return knot;
}
splay* findmax(splay* knot) {
    if (knot->right != 0) return findmax(knot->right);
    else return knot;
}

splay* first_next(splay* knot, long long element, splay* temp) {
    if (knot == 0) return temp;
    else {
        if (element > knot->key) {
            return first_next(knot->right, element, temp);
        }
        else if (element < knot->key) {
            temp = knot;
            return first_next(knot->left, element, temp);
        }
        else {
            if (knot->right != 0) return findmin(knot->right);
            else return temp;
        }
    }
    return 0;
}
splay* SPLIT(splay* knot, long long element) {
    if (knot == 0) return 0;
    splay* find_element = finding_element(knot, element);
    if (find_element == 0) find_element = first_next(knot, element, find_element);
    if (find_element == 0) find_element = findmax(knot);
    knot = SPLAY(find_element);
    return knot;
}
splay* ADD(splay* knot, long long element) {
    knot = SPLIT(knot, element);
    if (knot == 0) return knot;
    else {
        if (knot->key == element) return knot;
        else if (knot->key < element) {
            splay* root = new splay(element);
            root->left = knot;
            get_parent(root);
            get_sum(root);
            return root;
        }
        else {
            splay* root = new splay(element);
            splay* poor_right = knot;
            splay* poor_left = knot->left;
            poor_right->left = 0;
            root->right = poor_right;
            root->left = poor_left;
            get_sum(poor_right);
            get_sum(poor_left);
            get_parent(root);
            get_sum(root);
            return root;
        }
    }
}
splay* MERGE(splay* knot1, splay* knot2) {
    if (knot1 == 0) return knot2;
    if (knot2 == 0) return knot1;
    splay* poor_left = findmax(knot1);
    poor_left = SPLAY(poor_left);
    splay* poor_right = findmin(knot2);
    poor_right = SPLAY(poor_right);
    poor_right->left = poor_left;
    get_parent(poor_right);
    get_sum(poor_right);
    return poor_right;
}
void SUM(splay*& root, long long from, long long to, long long& sum, long long& p) {
    if (from > to || root == nullptr) sum = 0;
    else {
        root = SPLIT(root, from);
        if (root->key < from) sum = 0;
        else
        {
            splay* poor_from = root->left;
            splay* poor_to = nullptr;
            root->left = nullptr;
            if (poor_from != nullptr) poor_from->parent = nullptr;
            root = SPLIT(root, to);
            if (root == nullptr) sum = 0;
            else {
                if (root->key <= to) {
                    poor_to = root->right;
                    root->right = nullptr;
                    if (poor_to != nullptr) poor_to->parent = nullptr;
                    get_sum(poor_to);
                    get_parent(root);
                    get_sum(root);
                    if (root != nullptr) sum = root->sum;
                    else sum = 0;
                }
                else {
                    poor_to = root;
                    root = poor_to->left;
                    poor_to->left = nullptr;
                    if (root != nullptr) root->parent = nullptr;
                    get_sum(poor_to);
                    get_parent(poor_to);
                    get_parent(root);
                    get_sum(root);
                    if (root != nullptr) sum = root->sum;
                    else sum = 0;
                }


            }
            root = MERGE(poor_from, root);
            root = MERGE(root, poor_to);
        }
    }
    p = sum;
    std::cout << sum << '\n';
}
int main()
{
    splay* root = nullptr;
    std::ios::sync_with_stdio(false);
    std::cout.tie(0);
    std::cin.tie(0);
    std::string s = "+";
    long long p = 0;
    long long element = 0;
    long long sum = 0;
    int n = 0;
    std::cin >> n;
    for (int i = 0; i < n; ++i) {
        std::cin >> s;
        if (s[0] == '+') {
            std::cin >> element;
            if (p != 0) element = (element + p) % 1000000000;
            if (root == nullptr) root = new splay(element);
            else root = ADD(root, element);
            p = 0;
        }
        if (s[0] == '?') {
            long long from;
            long long to;
            std::cin >> from >> to;
            SUM(root, from, to, sum, p);
        }
    }
    return 0;
}