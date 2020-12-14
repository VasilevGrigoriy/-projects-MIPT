#include <iostream>
#include <cstring>
class String {
private:
    size_t sz = 0;
    size_t capacity = 0;
    char* str = nullptr;
public:

    String() = default;
    String(const char);
    String(const char*);
    String(size_t sz, char);
    String(const String&);
    void swap(String&);
    String& operator=(String);
    String& operator=(const char*);
    String& operator +=(const char);
    String& operator +=(const String&);
    char* EXTRA_MEMORY(size_t);
    char* DELETE_EXTRA_MEMORY(size_t);
    int length() const;
    void push_back(const char);
    void pop_back();
    bool operator==(const String&);
    bool operator==(const char*);
    char& front();
    const char& front() const;
    char& back();
    const char& back() const;
    const char& operator[](size_t) const;
    char& operator[](size_t);
    size_t find(const String&) const;
    size_t rfind(const String&) const;
    String substr(size_t start, size_t) const;
    bool empty() const;
    void clear();
    friend std::istream& operator>>(std::istream&, String&);
    friend std::ostream& operator<<(std::ostream&, const String&);
    ~String() {
        sz = 0;
        capacity = 0;
        delete[] str;
    }
};

String::String(const char c) :sz(1), capacity(1), str(new char[1]) {
    str[0] = c;
}
String::String(const char* s) : sz(strlen(s)), capacity(sz), str(new char[capacity]) {
    memcpy(str, s, sz);
}
String::String(size_t s, char c = '\0') : sz(s), capacity(s), str(new char[capacity]) {
    memset(str, c, s);
}
String::String(const String& s) : String(s.sz, '\0') {
    memcpy(str, s.str, s.sz);
}
void String::swap(String& s) {
    std::swap(sz, s.sz);
    std::swap(capacity, s.capacity);
    std::swap(str, s.str);
}
String& String::operator=(String s) {
    swap(s);
    return *this;
}
String& String:: operator=(const char* c) {
    delete[] str;
    sz = strlen(c);
    capacity = sz;
    str = new char[capacity];
    memcpy(str, c, sz);
    return *this;
}
String& String::operator +=(const char c) {
    size_t newsz = sz + 1;
    char* newSTR = EXTRA_MEMORY(1);
    if (newSTR != nullptr) {
        newSTR[sz] = c;
        delete[] str;
        str = newSTR;
        sz = newsz;
        return *this;
    }
    else {
        str[sz] = c;
        sz = newsz;
        return *this;
    }
}
String& String::operator+=(const String& s) {
    size_t newsz = sz + s.sz;
    char* newSTR = EXTRA_MEMORY(s.sz);
    if (newSTR != nullptr) {
        memcpy(newSTR + sz, s.str, s.sz);
        delete[] str;
        str = newSTR;
        sz = newsz;
        return *this;
    }
    else {
        memcpy(str + sz, s.str, s.sz);
        sz = newsz;
        return *this;
    }
}
int String::length() const {
    return sz;
}
char* String::EXTRA_MEMORY(size_t extra) {
    if (sz + extra >= capacity) {
        capacity = 2 * (sz + extra);
        char* newSTR = new char[capacity];
        memcpy(newSTR, str, sz);
        return newSTR;
    }
    return nullptr;
}
char* String::DELETE_EXTRA_MEMORY(size_t extra) {
    if (sz - extra <= capacity / 4) {
        capacity /= 2;
        char* newSTR = new char[capacity];
        memcpy(newSTR, str, sz - extra);
        return newSTR;
    }
    else return nullptr;
}
void String::push_back(const char c) {
    *this += c;
}
void String::pop_back() {
    size_t newSZ = sz - 1;
    char* newSTR = DELETE_EXTRA_MEMORY(1);
    if (newSTR != nullptr) {
        delete[] str;
        str = newSTR;
        sz = newSZ;
    }
    else sz = newSZ;
}
bool String::operator==(const String& s) {
    if (sz != s.sz) return false;
    else if (!strncmp(str, s.str, sz)) return true;
    else return false;
}
bool String::operator==(const char* s) {
    if (sz != strlen(s)) return false;
    else if (!strncmp(str, s, sz)) return true;
    else return false;
}
char& String::front() {
    return str[0];
}
const char& String::front() const {
    return str[0];
}
char& String::back() {
    return str[sz - 1];
}
const char& String::back() const {
    return str[sz - 1];
}
const char& String::operator[](size_t index) const {
    return str[index];
}
char& String::operator[](size_t index) {
    return str[index];
}
size_t String::find(const String& s) const {
    if (s.sz > sz) return sz;
    else {
        for (size_t i = 0; i <= sz - s.sz; ++i) {
            if (!strncmp(str + i, s.str, s.sz)) return i;
        }
        return sz;
    }
}
size_t String::rfind(const String& s) const {
    if (s.sz > sz) return sz;
    else {
        for (size_t i = sz - s.sz; i > 0; --i) {
            if (!strncmp(str + i, s.str, s.sz)) return i;
        }
        if (!strncmp(str, s.str, s.sz)) return 0;
        return sz;
    }
}
String String::substr(size_t start, size_t count) const {
    char* newSTR = new char[start + 1];
    memcpy(newSTR, str + start, count);
    newSTR[start] = '\0';
    String s = newSTR;
    s.sz = count;
    return s;
}
bool String::empty() const {
    return (sz == 0);
}
void String::clear() {
    capacity = 0;
    sz = 0;
    delete[] str;
    str = nullptr;
}
String operator+(const String& s1, const String& s2) {
    String copy = s1;
    copy += s2;
    return copy;
}
String operator+(const String& s1, const char c) {
    String copy = s1;
    copy += c;
    return copy;
}
String operator+(const char c, const String& s1) {
    String copy = c;
    copy += s1;
    return copy;
}
std::ostream& operator<<(std::ostream& out, const String& s) {
    for (size_t i = 0; i < s.sz; ++i) {
        out << s.str[i];
    }
    return out;
}
std::istream& operator>>(std::istream& in, String& s) {
    delete[] s.str;
    char c;
    while (in.read(&c, 1)) {
        if (c == ' ' || c == '\0' || c == '\n') break;
        s.push_back(c);
    }
    return in;
}