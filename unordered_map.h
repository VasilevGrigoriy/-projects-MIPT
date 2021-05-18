#include<iostream>
#include<stdio.h>
#include<vector>
#include<deque>
#include<algorithm>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>
#include <iterator>
#include <cassert>

template<typename T, typename Allocator = std::allocator<T>>
class List {

    int sz = 0;

    void push_back_without_argument() {
        //  std::cerr << "pub" << '\n';
        if (sz == 0) {
            Node* new_node = AlTr::allocate(alloc, 1);
            AlTr::construct(alloc, new_node);
            new_node->prev = fake_node;
            new_node->next = fake_node;
            fake_node->prev = new_node;
            fake_node->next = new_node;
        }
        else {
            Node* new_node = AlTr::allocate(alloc, 1);
            AlTr::construct(alloc, new_node);
            fake_node->prev->next = new_node;
            new_node->prev = fake_node->prev;
            fake_node->prev = new_node;
            new_node->next = fake_node;
        }
        sz++;
    }
public:
    struct Node {
        Node* prev = nullptr;
        Node* next = nullptr;
        T val;
        Node() {
        }
        Node(const T& val) :val(val) {}
        template<typename... Args>
        Node(Args&&... args) : val(std::forward<Args>(args)...) {

        }
    };
    Node* fake_node = nullptr;
    typename std::allocator_traits<Allocator>::template rebind_alloc<Node> alloc;
    using AlTr = std::allocator_traits<typename std::allocator_traits<Allocator>::template rebind_alloc<Node>>;
    typename std::allocator_traits<Allocator>::template rebind_alloc<T> copy = alloc;
    explicit List(const Allocator& allocat = Allocator()) : alloc(allocat) {
        // std::cerr << "common_create\n";
        fake_node = AlTr::allocate(alloc, 1);
        fake_node->prev = fake_node;
        fake_node->next = fake_node;
    }
    List(int count, const T& value, const Allocator& allocat = Allocator()) : alloc(allocat) {
        //  std::cerr << "many_create\n";
        fake_node = AlTr::allocate(alloc, 1);
        while (this->size() != count) {
            this->push_back(value);
        }
    }
    List(int count, const Allocator& allocat = Allocator()) : alloc(allocat) {
        //  std::cerr << "many_create\n";
        fake_node = AlTr::allocate(alloc, 1);
        while (this->size() != count) {
            this->push_back_without_argument();
        }
    }
    //----------------------------------------------------------
    void push_back(const T& val) {
        //  std::cerr << "pub" << '\n';
        if (sz == 0) {
            Node* new_node = AlTr::allocate(alloc, 1);
            AlTr::construct(alloc, new_node, val);
            new_node->prev = fake_node;
            new_node->next = fake_node;
            fake_node->prev = new_node;
            fake_node->next = new_node;
        }
        else {
            Node* new_node = AlTr::allocate(alloc, 1);
            AlTr::construct(alloc, new_node, val);
            fake_node->prev->next = new_node;
            new_node->prev = fake_node->prev;
            fake_node->prev = new_node;
            new_node->next = fake_node;
        }
        sz++;
    }
    void push_front(const T& val) {
        //   std::cerr << "puf" << '\n';
        if (sz == 0) {
            Node* new_node = AlTr::allocate(alloc, 1);
            AlTr::construct(alloc, new_node, val);
            new_node->prev = fake_node;
            new_node->next = fake_node;
            fake_node->prev = new_node;
            fake_node->next = new_node;
        }
        else {
            Node* new_node = AlTr::allocate(alloc, 1);
            AlTr::construct(alloc, new_node, val);
            fake_node->next->prev = new_node;
            new_node->next = fake_node->next;
            fake_node->next = new_node;
            new_node->prev = fake_node;
        }
        sz++;
    }
    template<typename E>
    void push_front(E&& val) {
        //   std::cerr << "puf" << '\n';
        if (sz == 0) {
            Node* new_node = AlTr::allocate(alloc, 1);
            AlTr::construct(alloc, new_node, std::forward<E>(val));
            new_node->prev = fake_node;
            new_node->next = fake_node;
            fake_node->prev = new_node;
            fake_node->next = new_node;
        }
        else {
            Node* new_node = AlTr::allocate(alloc, 1);
            AlTr::construct(alloc, new_node, std::forward<E>(val));
            fake_node->next->prev = new_node;
            new_node->next = fake_node->next;
            fake_node->next = new_node;
            new_node->prev = fake_node;
        }
        sz++;
    }
    void pop_back() {
        //  std::cerr << "pob" << '\n';
        if (sz == 1) {
            AlTr::destroy(alloc, fake_node->prev);
            AlTr::deallocate(alloc, fake_node->prev, 1);
            fake_node->prev = fake_node;
            fake_node->next = fake_node;
        }
        else {
            Node* copy = fake_node->prev->prev;
            fake_node->prev->prev->next = fake_node;
            AlTr::destroy(alloc, fake_node->prev);
            AlTr::deallocate(alloc, fake_node->prev, 1);
            fake_node->prev = copy;
        }
        sz--;
    }
    void pop_front() {
        //    std::cerr << "pof" << '\n';
        if (sz == 1) {
            AlTr::destroy(alloc, fake_node->prev);
            AlTr::deallocate(alloc, fake_node->prev, 1);
            fake_node->prev = fake_node;
            fake_node->next = fake_node;
        }
        else {
            Node* copy = fake_node->next->next;
            fake_node->next->next->prev = fake_node;
            AlTr::destroy(alloc, fake_node->next);
            AlTr::deallocate(alloc, fake_node->next, 1);
            fake_node->next = copy;
        }
        sz--;
    }
    //-----------------------------------------------------------
    int size() const {
        return this->sz;
    }
    Allocator get_allocator() const {
        return copy;
    }
    //-----------------------------------------------------------
    List(const List& l) {
        //      std::cerr << "create_from_list\n";
        alloc = AlTr::select_on_container_copy_construction(l.alloc);
        fake_node = AlTr::allocate(alloc, 1);
        fake_node->prev = fake_node;
        fake_node->next = fake_node;
        if (l.size() == 0) {

        }
        else {
            Node* temp = l.fake_node->next;
            for (int i = 0; i < l.size(); i++) {
                this->push_back(temp->val);
                temp = temp->next;
            }
        }
    }
    List& operator=(const List& l) {
        //   std::cerr << "operator_=\n";
        if (AlTr::propagate_on_container_copy_assignment::value && alloc != l.alloc) {
            alloc = l.alloc;
        }
        while (sz != 0) {
            this->pop_back();
        }
        Node* temp = l.fake_node->next;
        for (int i = 0; i < l.size(); i++) {
            this->push_back(temp->val);
            temp = temp->next;
        }
        return *this;
    }
    template<bool is_const >
    class common_iterator {
    public:
        std::conditional_t<is_const, const Node*, Node*> ref = nullptr;

        using iterator = common_iterator<false>;
        using const_iterator = common_iterator<true>;
        using reverse_iterator = std::reverse_iterator<iterator>;
        using const_reverse_iterator = std::reverse_iterator<const_iterator>;

        using iterator_category = std::bidirectional_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using pointer = std::conditional_t<is_const, const T*, T*>;
        using value_type = std::conditional_t<is_const, const T, T>;;
        using reference = std::conditional_t<is_const, const T&, T&>;
        common_iterator() {

        }
        common_iterator(Node* n) :ref(n) {
        }
        common_iterator(const common_iterator<false>& it) {
            ref = it.ref;
        }
        common_iterator& operator=(common_iterator it) {
            ref = it.ref;
            return *this;
        }

        //----------------------------------------------

        reference operator*() const {
            //    std::cerr << "*" << '\n';
            return ref->val;
        }
        pointer operator->() const {
            return &(ref->val);
        }

        //----------------------------------------------------

        common_iterator& operator++() {
            //   std::cerr << "++" << '\n';
            ref = ref->next;
            return *this;
        }
        common_iterator& operator--() {
            //  std::cerr << "--" << '\n';
            ref = ref->prev;
            return *this;
        }
        common_iterator operator++(int) {

            common_iterator copy = *this;
            copy.ref = copy.ref->next;
            ref = ref->next;
            return copy;
        }
        common_iterator operator--(int) {
            common_iterator copy = *this;
            copy.ref = copy.ref->prev;
            ref = ref->next;
            return copy;
        }

        //-----------------------------------------------------

        bool operator!=(common_iterator<false>& it) const {
            //     std::cerr << "!=\n";
            if (ref == it.ref) return false;
            else return true;
        }
        bool operator!=(common_iterator<true> it) const {
            if (ref == it.ref) return false;
            else return true;
        }
        bool operator==(common_iterator<false> it) const {
            //    std::cerr << "==\n";
            if (ref == it.ref) return true;
            else return false;
        }
        bool operator==(common_iterator<true> it) const {
            if (ref == it.ref) return true;
            else return false;
        }

        ~common_iterator() = default;
    };

    using iterator = common_iterator<false>;
    using const_iterator = common_iterator<true>;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;



    //----------------------------------------------------------------------------

    iterator begin() {
        // std::cerr << "bgn\n";
        return common_iterator<false>(this->fake_node->next);
    }
    const_iterator begin() const {
        return common_iterator<true>(this->fake_node->next);
    }
    iterator end() {
        //  std::cerr << "end\n";
        return common_iterator<false>(this->fake_node);
    }
    const_iterator end() const {
        return common_iterator<true>(this->fake_node);
    }
    const_iterator cbegin() const {
        return common_iterator<true>(this->fake_node->next);
    }
    const_iterator cend() const {
        return common_iterator<true>(this->fake_node);
    }


    reverse_iterator rbegin() {
        iterator it(this->fake_node);
        return reverse_iterator(it);
    }
    const_reverse_iterator rbegin() const {
        const_iterator it(this->fake_node);
        return const_reverse_iterator(it);
    }
    reverse_iterator rend() {
        iterator it(this->fake_node->next);
        return reverse_iterator(it);
    }
    const_reverse_iterator rend() const {
        const_iterator it(this->fake_node->next);
        return const_reverse_iterator(it);
    }
    const_reverse_iterator crbegin() const {
        const_iterator it(this->fake_node);
        return const_reverse_iterator(it);
    }
    const_reverse_iterator crend() const {
        const_iterator it(this->fake_node->next);
        return const_reverse_iterator(it);
    }

    //---------------------------------------------

    void insert(iterator& it, const T& val = T()) {
        //   std::cerr << "in" << '\n';
        Node* new_node = AlTr::allocate(alloc, 1);
        AlTr::construct(alloc, new_node, val);
        new_node->prev = it.ref->prev;
        it.ref->prev->next = new_node;
        new_node->next = it.ref;
        it.ref->prev = new_node;
        sz++;
    }
    void erase(iterator it) {
        //  std::cerr << "er" << '\n';
        it.ref->prev->next = it.ref->next;
        it.ref->next->prev = it.ref->prev;
        AlTr::destroy(alloc, it.ref);
        AlTr::deallocate(alloc, it.ref, 1);
        sz--;
    }
    void push_node(Node* new_node) {
        if (sz == 0) {
            new_node->prev = fake_node;
            new_node->next = fake_node;
            fake_node->prev = new_node;
            fake_node->next = new_node;
        }
        else {
            fake_node->next->prev = new_node;
            new_node->next = fake_node->next;
            fake_node->next = new_node;
            new_node->prev = fake_node;
        }
        sz++;
    }
    template<typename... Args>
    iterator emplace(Args&&... args) {
        Node* new_node = AlTr::allocate(alloc, 1);
        std::allocator_traits<Allocator>::construct(copy, &new_node->val, std::forward<Args>(args)...);
        push_node(new_node);
        return begin();
    }
    void delete_ptrs() {
        fake_node->prev = nullptr;
        fake_node->next = nullptr;
        sz--;
    }
    ~List() {
        //   std::cerr << "~\n";
        while (this->size() != 0) {
            this->pop_front();
        }
        AlTr::deallocate(alloc, fake_node, 1);
    }
};



template<typename Key, typename Value, typename Hash = std::hash<Key>, typename Equal = std::equal_to<Key>, typename Alloc = std::allocator<std::pair<const Key, Value>>>
class UnorderedMap {
private:
    using NodeType = std::pair<const Key, Value>;
    using bucket = List<NodeType, Alloc>;
    using Chain = std::pair<bucket, std::pair<int, int>>;
    using vector_alloc = typename std::allocator_traits<Alloc>::template rebind_alloc<Chain>;
    // -1 = неопределенно

   // typename std::allocator_traits<Alloc>::template rebind_alloc<node> alloc;
   // using AlTr = std::allocator_traits<typename std::allocator_traits<Alloc>::template rebind_alloc<node>>;
    int mx_load_factor = 10;
    int map_size = 250000;
    int ld_factor = 0;
    int used_buckets = 0;
    int sz = 0;

    int begin_lst = 0;
    int end_lst = 0;

    void change(const UnorderedMap& m) {
        hash = m.hash;
        is_equal = m.is_equal;
        map = m.map;
        map_size = m.map_size;
        begin_lst = m.begin_lst;
        end_lst = m.end_lst;
        sz = m.sz;
        ld_factor = m.ld_factor;
        used_buckets = m.used_buckets;
    }
    void muv(UnorderedMap& m) {
        hash = std::move(m.hash);
        is_equal = std::move(m.is_equal);
        map = std::move(m.map);
        map_size = std::move(m.map_size);
        begin_lst = std::move(m.begin_lst);
        end_lst = std::move(m.end_lst);
        sz = std::move(m.sz);
        ld_factor = std::move(m.ld_factor);
        used_buckets = std::move(m.used_buckets);
    }
    Value& main_function(const Key& key, int flag = 0) {//0 - метод at, 1 - метод []
        this->rehash(3 * map_size);
        auto hs = hash(key) % (map_size - 1) + 1;
        if (map[hs].first.size() > 0) {
            for (auto i = map[hs].first.begin(); i != map[hs].first.end(); ++i) {
                if (is_equal((*i).first, key)) {
                    return (*i).second;
                }
            }
            if (flag == 1) {
                sz++;
                map[hs].first.push_front(std::make_pair(key, Value()));
                auto temp = map[hs].first.begin();
                ld_factor = int(sz / used_buckets);
                return (*temp).second;
            }
            else {
                throw std::out_of_range("std::out_of_range");
            }
        }
        else {
            if (flag == 1) {
                if (sz == 0) {
                    begin_lst = hs;
                    map[hs].first.push_front(std::make_pair(key, Value()));
                    used_buckets++;

                }
                else if (sz == 1) {
                    map[hs].first.push_front(std::make_pair(key, Value()));
                    map[hs].second.first = begin_lst;
                    map[begin_lst].second.second = hs;
                    end_lst = hs;
                    used_buckets++;
                    sz++;
                    ld_factor = int(sz / used_buckets);
                }
                else {
                    map[hs].first.push_front(std::make_pair(key, Value()));
                    map[end_lst].second.second = hs;
                    map[hs].second.first = end_lst;
                    end_lst = hs;
                    sz++;
                    used_buckets++;
                    ld_factor = int(sz / used_buckets);
                }
                auto temp = map[hs].first.begin();
                return (*temp).second;
            }
            else {
                throw std::out_of_range("std::out_of_range");
            }
        }
    }
    void main_rehash(int how_long) {
        how_long = std::max(how_long, map_size);
        if (how_long == map_size) return;
        std::vector<Chain, vector_alloc> new_map(how_long, map.get_allocator());
        int count = 0;
        int last_hash = 0;
        for (auto it = begin_lst; it != 0; it = map[it].second.second) {
            while (map[it].first.size() != 0) {
                auto it1 = map[it].first.begin();
                auto hs = hash(it1.ref->val.first) % (how_long - 1) + 1;
                if (count == 0) {
                    begin_lst = hs;
                    last_hash = hs;
                }
                else if (count == sz - 1) {
                    end_lst = hs;
                    last_hash = hs;
                }
                new_map[hs].second.first = last_hash;
                new_map[last_hash].second.second = hs;
                count++;
                new_map[hs].first.push_node(it1.ref);
                map[hs].first.pop_front();
            }
        }
        map = std::move(new_map);
        map_size = how_long;
        ld_factor = int(sz / map_size);
    }
    void rehash() {
        if (ld_factor >= mx_load_factor) {
            main_rehash(3 * map_size);
        }
    }
public:
    Hash hash;
    Equal is_equal;


    bucket for_emplace;
    std::vector<Chain, vector_alloc> map;

    //----------------------------------------------------------------------
    UnorderedMap(const Hash& hash = Hash(), const Equal& is_equal = Equal(), const Alloc& alloc = Alloc()) :
        hash(hash), is_equal(is_equal), map(std::vector<Chain, vector_alloc>(map_size, alloc)) {
    }
    UnorderedMap(const UnorderedMap& m) {
        change(m);
    }
    UnorderedMap(UnorderedMap&& m) :mx_load_factor(std::move(m.mx_load_factor)), map_size(std::move(m.map_size)),
        ld_factor(std::move(m.ld_factor)), used_buckets(std::move(m.used_buckets)),
        sz(std::move(m.sz)), begin_lst(std::move(m.begin_lst)), end_lst(std::move(m.end_lst)),
        hash(std::move(m.hash)), is_equal(std::move(m.is_equal)), map(std::move(m.map))
    {
        m.mx_load_factor = 0;
        m.begin_lst = 0;
        m.end_lst = 0;
        m.sz = 0;
        m.ld_factor = 0;
        m.used_buckets = 0;
    }
    //------------------------------------------------------------------------------
    UnorderedMap& operator=(const UnorderedMap& m) {
        change(m);
        return *this;
    }
    UnorderedMap& operator=(UnorderedMap&& m) {
        muv(m);

        m.begin_lst = 0;
        m.end_lst = 0;
        m.sz = 0;
        m.ld_factor = 0;
        m.used_buckets = 0;
        return *this;
    }
    //----------------------------------------------------------------------------------
    Value& operator[](const Key& key) {
        return main_function(key, 1);
    }
    Value& operator[](Key&& key) {
        this->rehash();
        auto hs = hash(key) % (map_size - 1) + 1;
        if (map[hs].first.size() > 0) {
            for (auto i = map[hs].first.begin(); i != map[hs].first.end(); ++i) {
                if (is_equal((*i).first, key)) {
                    return (*i).second;
                }
            }
            sz++;
            map[hs].first.push_front(std::make_pair(std::move(key), Value()));
            auto temp = map[hs].first.begin();
            ld_factor = int(sz / used_buckets);
            return (*temp).second;
        }
        else {
            if (sz == 0) {
                begin_lst = hs;
                map[hs].first.push_front(std::make_pair(std::move(key), Value()));
                used_buckets++;
                sz++;
                ld_factor = int(sz / used_buckets);
            }
            else if (sz == 1) {
                map[hs].first.push_front(std::make_pair(std::move(key), Value()));
                map[hs].second.first = begin_lst;
                map[begin_lst].second.second = hs;
                end_lst = hs;
                used_buckets++;
                sz++;
                ld_factor = int(sz / used_buckets);
            }
            else {
                map[hs].first.push_front(std::make_pair(std::move(key), Value()));
                map[end_lst].second.second = hs;
                map[hs].second.first = end_lst;
                end_lst = hs;
                sz++;
                used_buckets++;
                ld_factor = int(sz / used_buckets);
            }
            auto temp = map[hs].first.begin();
            return (*temp).second;
        }
    }
    Value& at(const Key& key) {
        auto hs = hash(key) % (map_size - 1) + 1;
        if (map[hs].first.size() > 0) {
            for (auto i = map[hs].first.begin(); i != map[hs].first.end(); ++i) {
                if (is_equal((*i).first, key)) return (*i).second;
            }
            throw std::out_of_range("std::out_of_range");
        }
        else {
            throw std::out_of_range("std::out_of_range");
        }
    }
    const Value& at(const Key& key) const {
        auto hs = hash(key) % (map_size - 1) + 1;
        if (map[hs].first.size() > 0) {
            for (auto i = map[hs].first.begin(); i != map[hs].first.end(); ++i) {
                if (is_equal((*i).first, key)) return (*i).second;
            }
            throw std::out_of_range("std::out_of_range");
        }
        else {
            throw std::out_of_range("std::out_of_range");
        }
    }
    int size() const {
        return sz;
    }
    //----------------------------------------------------------------------------------
    template<bool is_const>
    class common_iterator {
    public:
        using Chain = std::pair<bucket, std::pair<int, int>>;
        using vector_alloc = typename std::allocator_traits<Alloc>::template rebind_alloc<Chain>;
        //----------------------------------------------------------------------
        std::conditional_t<is_const, typename bucket::const_iterator, typename bucket::iterator> current;
        using Map = std::conditional_t<is_const, const std::vector<Chain, vector_alloc>&, std::vector<Chain, vector_alloc>&>;
        std::conditional_t<is_const, const std::vector<Chain, vector_alloc>*, std::vector<Chain, vector_alloc>*> maps;//почему-то без этого итератор не видит поля map, иначе не получится итерироваться по map
        int cached;//хеш высчитанный
        //------------------------------------------------------------------

        using iterator_category = std::forward_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using pointer = std::conditional_t<is_const, const NodeType*, NodeType*>;
        using value_type = std::conditional_t<is_const, const NodeType, NodeType>;
        using reference = std::conditional_t<is_const, const NodeType&, NodeType&>;
        common_iterator(int cached, int flag, Map maaps) :maps(&maaps), cached(cached) {
            if (flag == 0) {
                current = (*maps)[cached].first.begin();
            }
            else current = (*maps)[cached].first.end();
        }
        common_iterator(int cached, typename bucket::iterator it, Map mapps) {
            current = it;
            maps = &mapps;
            this->cached = cached;
        }
        //-----------------------------------------------------------------
        reference operator*() const {
            return (*current);
        }
        pointer operator->() {
            return current.operator->();
        }
        //------------------------------------------------------------------
        common_iterator& operator++() {
            ++current;
            if ((*maps)[cached].first.end() == current) {
                if ((*maps)[cached].second.second == 0) {
                    return *this;
                }
                cached = (*maps)[cached].second.second;
                current = (*maps)[cached].first.begin();
            }
            return *this;
        }
        common_iterator operator++(int) {
            auto copy = *this;
            ++* this;
            return copy;
        }
        //----------------------------------------------------------------------------
        bool operator==(const common_iterator<false>& it) {
            return current == it.current;
        }
        bool operator!=(const common_iterator<false>& it) {
            return current != it.current;
        }
        bool operator==(const common_iterator<true>& it) {
            return current == it.current;
        }
        bool operator!=(const common_iterator<true>& it) {
            return current != it.current;
        }
    };

    using iterator = common_iterator<false>;
    using const_iterator = common_iterator<true>;

    //----------------------------------------------------------------------------------------

    //----------------------------------------------------------------------------------------
    iterator begin() {
        iterator it(begin_lst, 0, map);
        return it;
    }
    const_iterator begin() const {
        const_iterator it(begin_lst, 0, map);
        return it;
    }
    iterator end() {
        iterator it(end_lst, 1, map);
        return it;
    }
    const_iterator end() const {
        const_iterator it(end_lst, 1, map);
        return it;
    }
    const_iterator cbegin() const {
        const_iterator it(begin_lst, 0, map);
        return it;
    }
    const_iterator cend() const {
        const_iterator it(end_lst, 1, map);
        return it;
    }
    //-----------------------------------------------------------------------------------------------
    void reserve(int how_long) {
        mx_load_factor = how_long;
        this->rehash();
    }
    int max_load_factor() {
        return mx_load_factor;
    }
    void max_load_factor(int ml) {
        mx_load_factor = ml;
    }
    int load_factor() {
        return ld_factor;
    }
    //-------------------------------------------------------------------------------------------
    iterator find(const Key& key) {
        auto hs = hash(key) % (map_size - 1) + 1;
        if (map[hs].first.size() > 0) {
            for (auto i = map[hs].first.begin(); i != map[hs].first.end(); ++i) {
                if (is_equal((*i).first, key)) {
                    iterator temp(hs, i, map);
                    return temp;
                }
            }
            return this->end();
        }
        else return this->end();
    }
    const_iterator find(const Key& key) const {
        auto hs = hash(key) % (map_size - 1) + 1;
        if (map[hs].first.size() > 0) {
            for (auto i = map[hs].first.begin(); i != map[hs].first.end(); ++i) {
                if (is_equal((*i).first, key)) {
                    const_iterator temp(hs, i, map);
                    return temp;
                }
            }
            return this->cend();
        }
        else return this->cend();
    }
    //---------------------------------------------------------------------------------------------------
    std::pair<iterator, bool> insert(const NodeType& elem) {
        this->rehash();
        auto hs = hash(elem.first) % (map_size - 1) + 1;
        if (map[hs].first.size() > 0) {
            for (auto i = map[hs].first.begin(); i != map[hs].first.end(); ++i) {
                if (is_equal((*i).first, elem.first)) {
                    iterator temp(hs, i, map);
                    return { temp, 0 };
                }
            }
            sz++;
            map[hs].first.push_front(elem);
            auto temp = map[hs].first.begin();
            iterator temp_1(hs, temp, map);
            ld_factor = int(sz / used_buckets);
            return { temp_1, 1 };
        }
        else {
            if (sz == 0) {
                begin_lst = hs;
                map[hs].first.push_front(elem);
                used_buckets++;
                sz++;
                ld_factor = int(sz / used_buckets);
            }
            else if (sz == 1) {
                map[hs].first.push_front(elem);
                map[hs].second.first = begin_lst;
                map[begin_lst].second.second = hs;
                end_lst = hs;
                used_buckets++;
                sz++;
                ld_factor = int(sz / used_buckets);
            }
            else {
                map[hs].first.push_front(elem);
                map[end_lst].second.second = hs;
                map[hs].second.first = end_lst;
                end_lst = hs;
                sz++;
                used_buckets++;
                ld_factor = int(sz / used_buckets);
            }
            auto temp = map[hs].first.begin();
            iterator temp_1(hs, temp, map);
            return { temp_1, 1 };
        }
    }
    template<typename E>
    std::pair<iterator, bool> insert(E&& elem, bool flag = 0) {
        this->rehash();
        auto hs = hash(elem.first) % (map_size - 1) + 1;
        if (map[hs].first.size() > 0) {
            if (!flag) {
                for (auto i = map[hs].first.begin(); i != map[hs].first.end(); ++i) {
                    if (is_equal((*i).first, elem.first)) {
                        iterator temp(hs, i, map);
                        return { temp, 0 };
                    }
                }
            }
            sz++;
            map[hs].first.push_front(std::forward<E>(elem));
            auto temp = map[hs].first.begin();
            iterator temp_1(hs, temp, map);
            ld_factor = int(sz / used_buckets);
            return { temp_1, 1 };
        }
        else {
            if (sz == 0) {
                begin_lst = hs;
                map[hs].first.push_front(std::forward<E>(elem));
                used_buckets++;
                sz++;
                ld_factor = int(sz / used_buckets);
            }
            else if (sz == 1) {
                map[hs].first.push_front(std::forward<E>(elem));
                map[hs].second.first = begin_lst;
                map[begin_lst].second.second = hs;
                end_lst = hs;
                used_buckets++;
                sz++;
                ld_factor = int(sz / used_buckets);
            }
            else {
                map[hs].first.push_front(std::forward<E>(elem));
                map[end_lst].second.second = hs;
                map[hs].second.first = end_lst;
                end_lst = hs;
                sz++;
                used_buckets++;
                ld_factor = int(sz / used_buckets);
            }
            auto temp = map[hs].first.begin();
            iterator temp_1(hs, temp, map);
            return { temp_1, 1 };
        }
    }
    template<typename InputIterator>
    void insert(InputIterator it_begin, InputIterator it_end) {
        for (auto temp = it_begin; temp != it_end; ++temp) {
            insert(*temp);
        }
        this->rehash();
    }
    //--------------------------------------------------------------------------------
    template<typename... Args>
    std::pair<iterator, bool> emplace(Args&&... args) {
        auto iter_to_elem = for_emplace.emplace(std::forward<Args>(args)...);
        auto hs = hash(iter_to_elem.ref->val.first) % (map_size - 1) + 1;
        if (map[hs].first.size() > 0) {
            for (auto i = map[hs].first.begin(); i != map[hs].first.end(); ++i) {
                if (is_equal((*i).first, iter_to_elem.ref->val.first)) {
                    for_emplace.delete_ptrs();
                    iterator temp(hs, i, map);
                    return { temp, 0 };
                }
            }
            sz++;
            map[hs].first.push_node(iter_to_elem.ref);
            auto temp = map[hs].first.begin();
            iterator temp_1(hs, temp, map);
            ld_factor = int(sz / used_buckets);
            for_emplace.delete_ptrs();
            return { temp_1, 1 };
        }
        else {
            if (sz == 0) {
                begin_lst = hs;
                map[hs].first.push_node(iter_to_elem.ref);
                used_buckets++;
                sz++;
                ld_factor = int(sz / used_buckets);
            }
            else if (sz == 1) {
                map[hs].first.push_node(iter_to_elem.ref);
                map[hs].second.first = begin_lst;
                map[begin_lst].second.second = hs;
                end_lst = hs;
                used_buckets++;
                sz++;
                ld_factor = int(sz / used_buckets);
            }
            else {
                map[hs].first.push_node(iter_to_elem.ref);
                map[end_lst].second.second = hs;
                map[hs].second.first = end_lst;
                end_lst = hs;
                sz++;
                used_buckets++;
                ld_factor = int(sz / used_buckets);
            }
            auto temp = map[hs].first.begin();
            iterator temp_1(hs, temp, map);
            for_emplace.delete_ptrs();
            return { temp_1, 1 };
        }
    }
    //---------------------------------------------------------------------------------
    void erase(iterator it) {
        if (map[it.cached].first.size() == 1) {
            auto prev = map[it.cached].second.first;
            auto next = map[it.cached].second.second;
            map[it.cached].second.first = 0;
            map[it.cached].second.second = 0;
            map[it.cached].first.erase(it.current);
            sz--;
            if (prev == 0 && next == 0) {

            }
            else if (prev == 0) {
                begin_lst = next;
                map[next].second.first = 0;
            }
            else if (next == 0) {
                end_lst = prev;
                map[prev].second.second = 0;
            }
            else {
                map[next].second.first = prev;
                map[prev].second.second = next;
            }
            used_buckets--;
            if (used_buckets == 0) {
                ld_factor = 0;
                return;
            }
            ld_factor = int(sz / used_buckets);
        }
        else {
            map[it.cached].first.erase(it.current);
            sz--;
            ld_factor = int(sz / used_buckets);
        }
    }
    void erase(iterator it_begin, iterator it_end) {
        for (auto temp = it_begin; temp != it_end;) {
            auto copy_temp = temp;//после erase итератор инвалидируется
            ++copy_temp;
            erase(temp);
            temp = copy_temp;
        }
    }
};