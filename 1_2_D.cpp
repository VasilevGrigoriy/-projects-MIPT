#include <string>
#include <iostream>
#include <vector>

void siftUp(std::vector<std::pair<long long, long long>>& arr, long long i)
{
    while (i > 1 && arr[i].first < arr[i / 2].first)
    {
        std::swap(arr[i], arr[i / 2]);
        i /= 2;
    }
}

void siftDown(std::vector<std::pair<long long, long long>>& arr, long long i)
{
    while (i * 2 <= arr.size() - 1)
    {
        long long j = -1;
        if (arr[i * 2].first < arr[i].first)
        {
            j = i * 2;
        }
        if (i * 2 + 1 <= arr.size() - 1 && arr[i * 2 + 1].first < arr[i].first && (j == -1 || arr[i * 2].first > arr[i * 2 + 1].first))
        {
            j = i * 2 + 1;
        }
        if (j == -1) break;
        else
        {
            std::swap(arr[i], arr[j]);
            i = j;
        }
    }
}
void decreaseKey(std::vector<std::pair<long long, long long>>& two_tree, std::string next_move) {
    next_move.erase(0, 12);
    long long a = 0;
    while (next_move[a] != ' ')
    {
        ++a;
    }
    long long what_time = atoi(next_move.substr(0, a).c_str());
    next_move.erase(0, a + 1);
    long long how_low = atoi(next_move.c_str());
    for (long long i = 1; i <= two_tree.size(); ++i)
    {
        if (two_tree[i].second == what_time)
        {
            two_tree[i].first -= how_low;
            siftUp(two_tree, i);
            break;
        }
    }
}
void Insert(std::vector<std::pair<long long, long long>>& two_tree, std::string next_move, int i) {
    two_tree.push_back({ atoi(next_move.substr(7,next_move.size() - 7).c_str()),i });
    siftUp(two_tree, two_tree.size() - 1);
}
void ExtractMin(std::vector<std::pair<long long, long long>>& two_tree) {
    two_tree[1] = two_tree[two_tree.size() - 1];
    two_tree.pop_back();
    siftDown(two_tree, 1);
}

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(NULL);
    std::cout.tie(NULL);
    long long q;
    std::vector<std::pair<long long, long long>> two_tree;
    two_tree.push_back({ 0,0 });
    std::cin >> q;
    std::string s;
    getline(std::cin, s);
    for (long long i = 1; i <= q; ++i)
    {
        std::string next_move;
        getline(std::cin, next_move);
        if (next_move[0] == 'i')
        {
            Insert(two_tree, next_move, i);
        }
        else if (next_move[0] == 'g')
        {
            std::cout << two_tree[1].first << '\n';
        }
        else if (next_move[0] == 'e')
        {
            ExtractMin(two_tree);
        }
        else if (next_move[0] == 'd')
        {
            decreaseKey(two_tree, next_move);
        }
    }
    return 0;
}



