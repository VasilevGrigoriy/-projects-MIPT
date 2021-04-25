#include <iostream>
#include <vector>
#include <cmath>


template <typename T>
class Deque {
private:
    std::vector<T*> deq;
    static const int buc = 11;
    int sz = 0;
    int capacity = 0;
    int begin_i = 0;
    int begin_j = 0;
    int end_i = 0;
    int end_j = 0;
    /*
    *
    -------------->j
    |
    |    система координат
    |
    |
    \/i
    *
   */
public:

    //----------------------------------------

    T& operator[](const int);
    const T& operator[](const int) const;
    T& at(const int);
    const T& at(const int) const;
    T* is_ref(int);
    size_t size() const;
    //----------------------------------------
    Deque& operator=(Deque);
    void swap(Deque&);// Так как это не move конструктор, по хорошему его следует спрятать в private как внутренний инструмент
    //----------------------------------------
    void memory_up();// Аналогично
    bool is_const();
    bool is_const() const;
    //---------------------------------------
    void push_back(const T&);
    void push_front(const T&);
    void pop_back();
    void pop_front();
    //--------------------------------------КОНСТРУКТОРЫ и ДЕСТРУКТОР----------------------------------------
    Deque() {
        capacity = 3;
        begin_i = 1;
        end_i = 1;
        deq.resize(3);
        for (int i = 0; i < 3; i++) {
            deq[i] = reinterpret_cast<T*>(new int8_t[sizeof(T) * buc]);
        }
    };
    Deque(const int n, const T& temp = T()) {
        // std::cerr << n << ' ' << temp << ' '; 
        int broke = 0;
        try {// Безопасное создание относительно возможности не существования конструктора у Т
            double copy = n;
            double cop_1 = buc;
            int live_buc = ceil(copy / cop_1);
            sz = n;
            capacity = 3 * live_buc;
            begin_i = live_buc;
            begin_j = 0;
            end_i = begin_i + live_buc - 1;
            if (n % buc == 0) end_j = buc - 1;
            else end_j = n % buc - 1;
            deq.resize(capacity);
            for (int i = 0; i < capacity; i++) {
                deq[i] = reinterpret_cast<T*>(new int8_t[sizeof(T) * buc]);
            }
            int current_i = begin_i;
            int current_j = begin_j;
            for (broke = 0; broke < sz; broke++) {
                new(deq[current_i] + current_j) T(temp);
                current_j++;
                if (current_j == buc) {
                    current_j = 0;
                    current_i++;
                }
            }
        }
        catch (...) {
            int current_i = begin_i;
            int current_j = begin_j;
            for (int i = 0; i < broke; i++) {
                (deq[current_i] + current_j)->~T();
                current_j++;
                if (current_j == buc) {
                    current_j = 0;
                    current_i++;
                }
            }
            for (int i = capacity - 1; i >= 0; i--) {
                delete[] reinterpret_cast<int8_t*>(deq[i]);
            }
            this->memory_up();
            throw;
        }
    }

    Deque(const Deque& d, int capacity_ = -1) {
        sz = d.sz;
        int temp = d.end_i - d.begin_i + 1;
        capacity = d.capacity;
        begin_i = d.begin_i;
        begin_j = d.begin_j;
        end_i = d.end_i;
        end_j = d.end_j;
        if (capacity_ != -1) {
            capacity = capacity_;
            begin_i = temp;
            end_i = 2 * temp - 1;
        }
        deq.resize(capacity);
        for (int i = 0; i < capacity; ++i) {
            deq[i] = reinterpret_cast<T*>(new int8_t[sizeof(T) * buc]);
        }
        begin_i = temp;
        end_i = 2 * temp - 1;
        int current_i_1 = begin_i;
        int current_j_1 = begin_j;
        for (int broke = 0; broke < sz; broke++) {
            //Конструктор копирования тоже следует экранировать от исключений в данном месте. -5%   
            new(deq[current_i_1] + current_j_1) T(d[broke]);
            current_j_1++;
            if (current_j_1 == buc) {
                current_j_1 = 0;
                current_i_1++;
            }
        }
    }
    ~Deque() {
        int current_i = begin_i;
        int current_j = begin_j;
        for (int i = 0; i < sz; i++) {
            (deq[current_i] + current_j)->~T();
            current_j++;
            if (current_j == buc) {
                current_j = 0;
                current_i++;
            }
        }
        for (int i = capacity - 1; i >= 0; i--) {
            delete[] reinterpret_cast<int8_t*>(deq[i]);
        }
        sz = 0;
        capacity = 0;
        begin_i = 0;
        begin_j = 0;
        end_i = 0;
        end_j = 0;
    }
    //---------------------------------------iterator-----------------------------
    template<bool is_const >
    class common_iterator {
    public:
        std::conditional_t<is_const, const T*, T*> ref;
        std::conditional_t<is_const, T* const*, T**> dq;

        int i = 0;
        int j = 0;

        using iterator = common_iterator<false>;
        using const_iterator = common_iterator<true>;
        using reverse_iterator = std::reverse_iterator<iterator>;
        using const_reverse_iterator = std::reverse_iterator<const_iterator>;

        using iterator_category = std::random_access_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using pointer = std::conditional_t<is_const, const T*, T*>;
        using value_type = T;// Здесь тоже требуется std::conditional, у тебя тип так же меняется (и он требуется для генерирования некоторого кода в reverse)
        using reference = std::conditional_t<is_const, const T&, T&>;

        common_iterator(int i_1, int j_1, Deque& d) :i(i_1), j(j_1) {
            ref = d.deq[i] + j;
            dq = &(d.deq[i]);
        }

        common_iterator(int i_1, int j_1, const Deque& d) :i(i_1), j(j_1) {
            ref = d.deq[i] + j;
            dq = &(d.deq[i]);
        }

        common_iterator(const common_iterator<false>& it) {
            ref = it.ref;
            dq = it.dq;
            i = it.i;
            j = it.j;
        }

        common_iterator(const common_iterator<true>& it) {// По факту не должно существовать возможности сконструировать не константный итератор из константного. Поэтому
        // Эффективнее создать оператор приведения не константного итератора к константному operator common_iterator<True>, а копировать просто от такого же типа
            ref = it.ref;
            dq = it.dq;
            i = it.i;
            j = it.j;
        }

        common_iterator& operator=(common_iterator& it) {
            i = it.i;
            j = it.j;
            ref = it.ref;
            dq = it.dq;
            return *this;
        }

        //----------------------------------------------

        //reference
        std::conditional_t<is_const, const T&, T&> operator*() const {
            std::cerr << "*" << '\n';
            return *ref;
        }
        //pointer
        std::conditional_t<is_const, const T*, T*> operator->() const {
            return ref;
        }

        //----------------------------------------------------

        common_iterator& operator++() {
            std::cerr << "++" << '\n';
            if (j + 1 >= buc) {
                j = 0;
                i++;
                dq++;
                ref = *dq + j;
            }
            else {
                ++ref;
                j++;
            }
            return *this;
        }
        common_iterator& operator--() {
            std::cerr << "--" << '\n';
            if (j - 1 < 0) {
                j = buc - 1;
                i--;
                dq--;
                ref = *dq + j;
            }
            else {
                j--;
                ref--;
            }
            return *this;
        }
        common_iterator operator++(int) {
            common_iterator copy = *this;
            if (j + 1 >= buc) {
                j = 0;
                i++;
                dq++;
                ref = *dq + j;
            }
            else {
                ref++;
                j++;
            }
            return copy;
        }
        common_iterator operator--(int) {
            common_iterator copy = *this;
            if (j - 1 < 0) {
                j = buc - 1;
                i--;
                dq--;
                ref = *dq + j;
            }
            else {
                j--;
                ref--;
            }
            return copy;
        }

        //----------------------------------------------------

        bool operator>(common_iterator<false> it2) const {
            if ((this->i > it2.i) || (this->i == it2.i && this->j > it2.j)) {
                return true;
            }
            else {
                return false;
            }
        }
        bool operator==(common_iterator<false> it2)const {
            if (ref == it2.ref) {
                return true;
            }
            else {
                return false;
            }
        }
        bool operator<(common_iterator<false> it2)const {
            if ((this->i < it2.i) || (this->i == it2.i && this->j < it2.j)) {
                return true;
            }
            else {
                return false;
            }
        }
        bool operator!=(common_iterator<false> it2)const {
            if (ref != it2.ref) {
                return true;
            }
            else {
                return false;
            }
        }
        bool operator<=(common_iterator<false> it2)const {
            if (*this < it2 || *this == it2) {
                return true;
            }
            else {
                return false;
            }
        }
        bool operator>=(common_iterator<false> it2)const {
            if (*this > it2 || *this == it2) {
                return true;
            }
            else {
                return false;
            }
        }

        bool operator>(common_iterator<true> it2)const {
            if ((this->i > it2.i) || (this->i == it2.i && this->j > it2.j)) {
                return true;
            }
            else {
                return false;
            }
        }
        bool operator==(common_iterator<true> it2)const {
            if (ref == it2.ref) {
                return true;
            }
            else {
                return false;
            }
        }
        bool operator<(common_iterator<true> it2)const {
            if ((this->i < it2.i) || (this->i == it2.i && this->j < it2.j)) {
                return true;
            }
            else {
                return false;
            }
        }
        bool operator!=(common_iterator<true> it2)const {
            if (ref != it2.ref) {
                return true;
            }
            else {
                return false;
            }
        }
        bool operator<=(common_iterator<true> it2)const {
            if (*this < it2 || *this == it2) {
                return true;
            }
            else {
                return false;
            }
        }
        bool operator>=(common_iterator<true> it2)const {
            if (*this > it2 || *this == it2) {
                return true;
            }
            else {
                return false;
            }
        }

        //---------------------------------------------------

        common_iterator& operator+=(int temp) {
            if (temp + j < buc && temp + j >= 0) {
                j = temp + j;
                ref = *dq + j;
            }
            else if (temp < 0) {
                return *this -= (-temp);
            }
            else {
                int copy = temp;
                int curr_i = i;
                curr_i += copy / buc;
                dq = dq + copy / buc;
                int curr_j = j + (copy) % buc;
                if (curr_j >= buc) {
                    curr_j %= buc;
                    curr_i++;
                    dq++;
                }
                i = curr_i;
                j = curr_j;
                ref = *dq + j;
            }
            return *this;
        }
        common_iterator& operator-=(int temp) {
            if (j - temp < buc && j - temp >= 0) {
                j = j - temp;
                ref = *dq + j;
            }
            else if (temp > 0) {
                int copy = temp;
                int curr_i = i;
                curr_i -= copy / buc;
                dq = dq - copy / buc;
                int curr_j = j - (copy) % buc;
                if (curr_j < 0) {
                    curr_j = (curr_j + buc) % buc;
                    curr_i--;
                    dq--;
                }
                i = curr_i;
                j = curr_j;
                ref = *dq + j;
            }
            else {
                return *this += (-temp);
            }
            return *this;
        }
        common_iterator operator+(int temp) {
            common_iterator it = *this;
            it += temp;
            return it;
        }
        common_iterator operator-(int temp) {
            common_iterator it = *this;
            it -= temp;
            return it;
        }

        //-----------------------------------------------------

        int operator-(common_iterator& it1) {
            int pos1 = this->j + this->i * buc;
            int pos2 = it1.j + it1.i * buc;
            return pos1 - pos2;
        }
        ~common_iterator() {
            i = 0;
            j = 0;
            dq = nullptr;
            ref = nullptr;
        }
    };

    using iterator = common_iterator<false>;
    using const_iterator = common_iterator<true>;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    //----------------------------------------------------------------------------

    iterator begin() {
        return common_iterator<false>(begin_i, begin_j, *this);
    }
    const_iterator begin() const {
        return common_iterator<true>(begin_i, begin_j, *this);
    }
    iterator end() {
        int curr_i = end_i;
        int curr_j = end_j;
        if (curr_j + 1 >= buc) {
            curr_j = 0;
            curr_i++;
        }
        else {
            curr_j++;
        }
        return common_iterator<false>(curr_i, curr_j, *this);
    }
    const_iterator end() const {
        int curr_i = end_i;
        int curr_j = end_j;
        if (curr_j + 1 >= buc) {
            curr_j = 0;
            curr_i++;
        }
        else {
            curr_j++;
        }
        return common_iterator<true>(curr_i, curr_j, *this);
    }
    const_iterator cbegin() const {
        return common_iterator<true>(begin_i, begin_j, *this);
    }
    const_iterator cend() const {
        int curr_i = end_i;
        int curr_j = end_j;
        if (curr_j + 1 >= buc) {
            curr_j = 0;
            curr_i++;
        }
        else {
            curr_j++;
        }
        return common_iterator<true>(curr_i, curr_j, *this);
    }


    reverse_iterator rbegin() {
        // return reverse_iterator(end()), с остальными аналогично
        int curr_i = end_i;
        int curr_j = end_j;
        if (curr_j + 1 >= buc) {
            curr_j = 0;
            curr_i++;
        }
        else {
            curr_j++;
        }
        iterator it(curr_i, curr_j, *this);
        return reverse_iterator(it);
    }
    const_reverse_iterator rbegin() const {
        int curr_i = end_i;
        int curr_j = end_j;
        if (curr_j + 1 >= buc) {
            curr_j = 0;
            curr_i++;
        }
        else {
            curr_j++;
        }
        const_iterator it(curr_i, curr_j, *this);
        return const_reverse_iterator(it);
    }
    reverse_iterator rend() {
        int curr_i = begin_i;
        int curr_j = begin_j;
        if (curr_j + 1 >= buc) {
            curr_j = 0;
            curr_i++;
        }
        else {
            curr_j++;
        }
        iterator it(curr_i, curr_j, *this);
        return reverse_iterator(it);
    }
    const_reverse_iterator rend() const {
        int curr_i = begin_i;
        int curr_j = begin_j;
        if (curr_j + 1 >= buc) {
            curr_j = 0;
            curr_i++;
        }
        else {
            curr_j++;
        }
        const_iterator it(curr_i, curr_j, *this);
        return const_reverse_iterator(it);
    }
    const_reverse_iterator crbegin() const {
        int curr_i = end_i;
        int curr_j = end_j;
        if (curr_j + 1 >= buc) {
            curr_j = 0;
            curr_i++;
        }
        else {
            curr_j++;
        }
        const_iterator it(curr_i, curr_j, *this);
        return const_reverse_iterator(it);
    }
    const_reverse_iterator crend() const {
        int curr_i = begin_i;
        int curr_j = begin_j;
        if (curr_j + 1 >= buc) {
            curr_j = 0;
            curr_i++;
        }
        else {
            curr_j++;
        }
        const_iterator it(curr_i, curr_j, *this);
        return const_reverse_iterator(it);
    }

    //---------------------------------------------

    void insert(iterator it, const T& val = T()) {
        std::cerr << "in" << '\n';
        // Отловом отсуствия конструктора занимается компилятор. Тут скорее просто экранирование исключений из него. Однако его лучше вызывать в явном виде,
        // Так как запрещённый оператор присваивания может сломать всё
        try {//на отсутствие конструктора копирования
            T copy = this->at(sz - 1);
            int curr_i_1 = end_i;
            int curr_j_1 = end_j;
            if (curr_j_1 >= buc) {
                curr_j_1 = 0;
                curr_i_1++;
            }
            int curr_i_2 = end_i;
            int curr_j_2 = end_j - 1;
            if (curr_j_2 < 0) {
                curr_j_2 = buc - 1;
                curr_i_2--;
            }
            // int pos = (it.i - begin_i - 1) * buc + (buc - begin_j) + it.j;
            while (curr_i_1 != it.i || curr_j_1 != it.j) {
                *(deq[curr_i_1] + curr_j_1) = *(deq[curr_i_2] + curr_j_2);
                curr_j_1--;
                curr_j_2--;
                if (curr_j_2 < 0) {
                    curr_j_2 = buc - 1;
                    curr_i_2--;
                }
                if (curr_j_1 < 0) {
                    curr_j_1 = buc - 1;
                    curr_i_1--;
                }
            }
            *(deq[it.i] + it.j) = val;
            this->push_back(copy);
        }
        catch (...) {
            throw;
        }
    }
    void insert(const_iterator it, const T& val = T()) {
        std::cerr << "in" << '\n';
        try {//на отсутствие конструктора копирования
            T copy = this->at(sz - 1);
            int curr_i_1 = end_i;
            int curr_j_1 = end_j;
            if (curr_j_1 >= buc) {
                curr_j_1 = 0;
                curr_i_1++;
            }
            int curr_i_2 = end_i;
            int curr_j_2 = end_j - 1;

            if (curr_j_2 < 0) {
                curr_j_2 = buc - 1;
                curr_i_2--;
            }
            // int pos = (it.i - begin_i - 1) * buc + (buc - begin_j) + it.j;
            while (curr_i_1 != it.i || curr_j_1 != it.j) {
                *(deq[curr_i_1] + curr_j_1) = *(deq[curr_i_2] + curr_j_2);
                curr_j_1--;
                curr_j_2--;
                if (curr_j_2 < 0) {
                    curr_j_2 = buc - 1;
                    curr_i_2--;
                }
                if (curr_j_1 < 0) {
                    curr_j_1 = buc - 1;
                    curr_i_1--;
                }
            }
            *(deq[it.i] + it.j) = val;
            this->push_back(copy);
        }
        catch(...){
            throw;
        }
    }

    void erase(iterator it) {
        int curr_i_1 = it.i;
        int curr_j_1 = it.j + 1;
        if (curr_j_1 >= buc) {
            curr_j_1 = 0;
            curr_i_1++;
        }
        int curr_i_2 = it.i;
        int curr_j_2 = it.j;
        // int pos = (it.i - begin_i - 1) * buc + (buc - begin_j) + it.j;
        while (curr_i_1 <= end_i && (curr_i_1 <= end_i || curr_i_2 <= end_j)) {
            if (curr_j_2 >= buc) {
                curr_j_2 = 0;
                curr_i_2++;
            }
            if (curr_j_1 >= buc) {
                curr_j_1 = 0;
                curr_i_1++;
            }
            *(deq[curr_i_2] + curr_j_2) = *(deq[curr_i_1] + curr_j_1);
            curr_j_1++;
            curr_j_2++;

        }
        if (curr_j_2 >= buc) {
            curr_j_2 = 0;
            curr_i_2++;
        }
        *(deq[curr_i_2] + curr_j_2) = *(deq[end_i] + end_j);
        *(deq[end_i] + end_j) = T();
        end_j--;
        if (curr_j_2 < 0) {
            end_j = buc - 1;
            end_i--;
        }
        if (end_j < 0) {
            end_j = buc - 1;
            end_i--;
        }
        sz--;
    }
    void erase(const_iterator it) {
        int curr_i_1 = it.i;
        int curr_j_1 = it.j + 1;
        if (curr_j_1 >= buc) {
            curr_j_1 = 0;
            curr_i_1++;
        }
        int curr_i_2 = it.i;
        int curr_j_2 = it.j;
        // int pos = (it.i - begin_i - 1) * buc + (buc - begin_j) + it.j;
        while (curr_i_1 <= end_i && (curr_i_1 <= end_i || curr_i_2 <= end_j)) {
            if (curr_j_2 >= buc) {
                curr_j_2 = 0;
                curr_i_2++;
            }
            if (curr_j_1 >= buc) {
                curr_j_1 = 0;
                curr_i_1++;
            }
            *(deq[curr_i_2] + curr_j_2) = *(deq[curr_i_1] + curr_j_1);
            curr_j_1++;
            curr_j_2++;

        }
        if (curr_j_2 >= buc) {
            curr_j_2 = 0;
            curr_i_2++;
        }
        *(deq[curr_i_2] + curr_j_2) = *(deq[end_i] + end_j);
        *(deq[end_i] + end_j) = T();
        end_j--;
        if (curr_j_2 < 0) {
            end_j = buc - 1;
            end_i--;
        }
        if (end_j < 0) {
            end_j = buc - 1;
            end_i--;
        }
        sz--;
    }

};
//-----------------------------------------------------------------------
template<typename T>
T& Deque<T>::operator[](const int i) {
    if (i + begin_j < buc) {
        //std::cout << "( i - " << begin_i << ", j - " << i + begin_j << " ) ";
        return *(deq[begin_i] + (i + begin_j));
    }
    else {
        int copy = i;
        int curr_i = begin_i;
        curr_i++;
        copy -= buc - begin_j;
        curr_i += copy / buc;
        int curr_j = (copy) % buc;
        //   std::cout << "( i - " << curr_i << ", j - " << curr_j << " ) ";
        return *(deq[curr_i] + curr_j);
    }
}
template<typename T>
const T& Deque<T>::operator[](const int i) const {

    if (i + begin_j < buc) {
        return *(deq[begin_i] + (i + begin_j));
    }
    else {
        int copy = i;
        int curr_i = begin_i;
        curr_i++;
        copy -= buc - begin_j;
        curr_i += copy / buc;
        int curr_j = (copy) % buc;
        return *(deq[curr_i] + curr_j);
    }
}
template<typename T>
T& Deque<T>::at(const int i) {
    if (i >= sz) throw std::out_of_range("out_of_range");
    else if (i + begin_j < buc) {
        //  std::cout << "( i - " << begin_i << ", j - " << i + begin_j << " ) ";
        return *(deq[begin_i] + (i + begin_j));
    }
    else {
        int copy = i;
        int curr_i = begin_i;
        curr_i++;
        copy -= buc - begin_j;
        curr_i += copy / buc;
        int curr_j = (copy) % buc;
        //  std::cout << "( i - " << curr_i << ", j - " << curr_j << " ) ";
        return *(deq[curr_i] + curr_j);
    }
}
// 4 копипасты одного кода, можно было просто определить один приватный метод, доступный для константных и не константных классов (или просто вычисляющий индекс)
// Или ещё как вариант в at сделать вызов [], а в константном варианте сделать const_cast<Deque*> ncThis (this) и так же вызвать [] от неконстантного варианта (хотя по стандарту правильнее наоборот)
template<typename T>
const T& Deque<T>::at(const int i) const {
    if (i >= sz) throw std::out_of_range("out_of_range");
    else if (i + begin_j < buc) {
        //    std::cout << "( i - " << begin_i << ", " << i + begin_j << " ) ";
        return *(deq[begin_i] + i + begin_j);
    }
    else {
        int copy = i;
        int curr_i = begin_i;
        curr_i++;
        copy -= buc - begin_j;
        curr_i += copy / buc;
        int curr_j = (copy) % buc;
        //     std::cout << "( i - " << curr_i << ", " << curr_j << " ) ";
        return *(deq[curr_i] + curr_j);
    }
}
template<typename T>
T* Deque<T>::is_ref(int i) {
    return deq[i];
}
template<typename T>
size_t Deque<T>::size() const {
    return this->sz;
}
//---------------------------------------------------------------------------
template<typename T>
Deque<T>& Deque<T>::operator=(Deque<T> d) {
    swap(d);
    return *this;
}
template<typename T>
void Deque<T>::swap(Deque<T>& d) {
    std::swap(this->deq, d.deq);
    std::swap(this->sz, d.sz);
    std::swap(this->capacity, d.capacity);
    std::swap(this->begin_i, d.begin_i);
    std::swap(this->begin_j, d.begin_j);
    std::swap(this->end_i, d.end_i);
    std::swap(this->end_j, d.end_j);
}
//----------------------------------------------------------------------------
template<typename T>
void Deque<T>::memory_up() {
    try {
        if ((begin_i != 0 || begin_j != 0) && (end_i != capacity - 1 || end_j != buc - 1)) {
            return;
        }
        else {
            std::vector<T*> new_deq;
            int temp = end_i - begin_i + 1;
            new_deq.resize(3 * temp);
            for (int i = 0; i < 3 * temp; ++i) {
                new_deq[i] = reinterpret_cast<T*>(new int8_t[sizeof(T) * buc]);
            }
            capacity = temp * 3;
            int current_i_1 = temp;
            int current_j_1 = begin_j;
            for (int broke = 0; broke < sz; broke++) {
                new(new_deq[current_i_1] + current_j_1) T((*this)[broke]);
                current_j_1++;
                if (current_j_1 == buc) {
                    current_j_1 = 0;
                    current_i_1++;
                }
            }
            begin_i = temp;
            end_i = 2 * temp - 1;
            this->deq = new_deq;
        }
    }
    catch (...) {
        // В данном случае требуется откатить контейнер к предыдущему состоянию, иначе какой смысл ловить исключение? -5%
        throw;
    }
}
template<typename T>
bool Deque<T>::is_const() {
    return false;
}
template<typename T>
bool Deque<T>::is_const() const {
    return true;
}
//-------------------------------------------------------------------------------
template<typename T>
void Deque<T>::push_back(const T& elem) {
    //    std::cerr << elem << ' ';
    try {
        if (sz == 0) {
            sz += 1;
        }
        else if (end_j + 1 >= buc) {
            sz++;
            end_i++;
            end_j = 0;
        }
        else {
            sz++;
            end_j++;
        }
        new(deq[end_i] + end_j) T(elem);
        this->memory_up();
    }
    catch (...) {
        (deq[end_i] + end_j)->~T();
        if (sz == 1) {
            sz--;

        }
        else if (end_j == 0) {
            sz--;
            end_i--;
            end_j = buc - 1;
        }
        else {
            sz--;
            end_j--;
        }

        throw;
    }
}
template<typename T>
void Deque<T>::push_front(const T& elem) {
    try {

        if (sz == 0) {
            sz += 1;
        }
        else if (begin_j == 0) {
            sz++;
            begin_i--;
            begin_j = buc - 1;
        }
        else {
            sz++;
            begin_j--;
        }
        new(deq[begin_i] + begin_j) T(elem);
        this->memory_up();
    }
    catch (...) {
        (deq[begin_i] + begin_j)->~T();
        if (sz == 1) {
            sz--;
        }
        else if (begin_j == buc - 1) {
            sz--;
            begin_j = 0;
            begin_i++;
        }
        else {
            sz--;
            begin_j++;
        }

        throw;
    }
}
template<typename T>
void Deque<T>::pop_back() {
    (deq[end_i] + end_j)->~T();
    sz--;
    if (end_j == 0) {
        end_i--;
        end_j = buc - 1;
    }
    else {
        end_j--;
    }
}
template<typename T>
void Deque<T>::pop_front() {
    (deq[begin_i] + begin_j)->~T();
    sz--;
    if (begin_j == buc - 1) {
        begin_i++;
        begin_j = 0;
    }
    else {
        begin_j++;
    }
}
//--------------------------------------------------------------------------------------------------------------------------