#include <string>
#include <iostream>
#include <vector>
#include <cmath>
class BigInteger {
	static const int base = 10000;
	std::vector<int> digits;
	bool sign = false;
public:

	bool operator==(const BigInteger&) const;
	bool operator!=(const BigInteger&) const;
	bool operator<(const BigInteger&) const;
	bool operator>(const BigInteger&) const;
	bool operator<=(const BigInteger&) const;
	bool operator>=(const BigInteger&) const;

	BigInteger& operator+=(const BigInteger&);
	BigInteger& operator-=(const BigInteger&);
	BigInteger& operator/=(const BigInteger&);
	BigInteger& operator*=(const BigInteger&);
	BigInteger& operator%=(const BigInteger&);

	BigInteger& operator++();
	BigInteger operator++(int);
	BigInteger& operator--();
	BigInteger operator--(int);

	BigInteger();
	BigInteger(std::string&);
	BigInteger(int);
	BigInteger(const BigInteger&);
	BigInteger(const char*);

	friend BigInteger operator+(const BigInteger& b_left, const BigInteger& b_right);
	friend BigInteger operator-(const BigInteger& b_left, const BigInteger& b_right);
	friend BigInteger operator*(const BigInteger& b_left, const BigInteger& b_right);
	friend BigInteger operator/(const BigInteger& b_left, const BigInteger& b_right);
	friend BigInteger operator%(const BigInteger& b_left, const BigInteger& b_right);

	operator std::string();
	explicit operator bool();
	void fast_divide_2() {
		// Каждый разряд при делении ( кроме 0) либо отдаст в меньший 5000 и поделится на 2, 
		// либо просто сам поделится на 2 и ничего не отдаст
		for (size_t i = 0; i < digits.size(); ++i) {
			if (i != 0) {
				if (digits[i] % 2 == 1) {
					digits[i - 1] += 5000;
				}
			}
			digits[i] /= 2;
		}
		if (digits[digits.size() - 1] == 0 && digits.size() > 1) digits.pop_back();// Если последний разряд был просто 1, он уйдет
	}
	BigInteger operator-() const;
	void change_minus() {
		sign = 0;
	}
	void get_r();
	void remove_leading_zeros();
	std::string toString() const;
	bool is_even() const {
		return (digits[0] % 2 == 0);
	}
	BigInteger& ChangeSign(BigInteger& b) {
		if (b == 0) return b;
		b.sign = !(b.sign);
		return b;
	}

	friend std::ostream& operator <<(std::ostream&, const BigInteger&);
	friend std::istream& operator>>(std::istream&, BigInteger&);
};

//--------------------------КОНСТРУКТОРЫ---------------------------------------------------------------------------
BigInteger::BigInteger() {
	digits.push_back(0);
	sign = 0;
}
BigInteger::BigInteger(int i) {
	if (i < 0) {
		sign = 1;
		i = -i;
	}
	if (i == 0) {
		digits.push_back(0);
		sign = 0;
	}
	while (i > 0) {
		digits.push_back(i % base);
		i /= base;
	}
	this->remove_leading_zeros();
}
BigInteger::BigInteger(const BigInteger& b) {
	digits.resize(b.digits.size());
	sign = b.sign;
	for (int i = 0; i < static_cast<int>(b.digits.size()); ++i) {
		digits[i] = b.digits[i];
	}
}
BigInteger::BigInteger(std::string& s) {
	if (s.length() == 0) {
		digits.push_back(0);
		this->sign = 0;
	}
	else {
		if (s[0] == '-') {
			s = s.substr(1);
			this->sign = 1;
		}
		else {
			this->sign = 0;
		}
		for (int i = s.length() - 4; i > -5; i -= 4) {
			if (i < 0) {
				this->digits.push_back(atoi(s.substr(0, 4 + i).c_str()));
			}
			else {
				this->digits.push_back(atoi(s.substr(i, 4).c_str()));
			}
		}
	}
	this->remove_leading_zeros();
}
BigInteger::BigInteger(const char* x) {
	std::string s(x);
	if (s.length() == 0) {
		this->sign = 0;
	}
	else {
		if (s[0] == '-') {
			s = s.substr(1);
			this->sign = 1;
		}
		else {
			this->sign = 0;
		}
		for (int i = s.length() - 4; i > -5; i -= 4) {
			if (i < 0) {
				this->digits.push_back(atoi(s.substr(0, 4 + i).c_str()));
			}
			else {
				this->digits.push_back(atoi(s.substr(i, 4).c_str()));
			}
		}
	}
	this->remove_leading_zeros();
}

//--------------------------ПРОЧЕЕ----------------------------------------------------------------------------------
void BigInteger::remove_leading_zeros() {
	while (this->digits.size() > 1 && this->digits.back() == 0) {
		this->digits.pop_back();
	}
	if (this->digits.size() == 1 && this->digits[0] == 0) {
		this->sign = 0;
	}
}
BigInteger BigInteger::operator-() const {
	BigInteger copy = *this;
	if (copy == 0) return copy;
	else {
		copy.sign = !(copy.sign);
		return copy;
	}
}
void BigInteger::get_r() {

	if (digits.size() == 0) {
		digits.push_back(0);
		return;
	}
	digits.push_back(digits[digits.size() - 1]);
	for (int i = static_cast<int>(digits.size()) - 2; i > 0; --i) {
		digits[i] = digits[i - 1];
	}
	digits[0] = 0;
}
std::string BigInteger::toString() const {
	std::string ans;
	if (sign) ans += "-";
	ans += std::to_string(digits[digits.size() - 1]);

	for (int i = digits.size() - 2; i >= 0; --i) {
		if (digits[i] < 10) ans += "000";
		else if (digits[i] < 100) ans += "00";
		else if (digits[i] < 1000) ans += "0";
		ans += std::to_string(digits[i]);

	}
	return ans;
}

//-------------------------ЛОГИЧЕСКИЕ ОПЕРАЦИИ----------------------------------------------------------------------
bool BigInteger::operator==(const BigInteger& b_right) const {
	if (digits.size() != b_right.digits.size()) return false;
	if (digits.empty() == 1 && b_right.digits.empty() == 1) return true;
	if (sign != b_right.sign) return false;
	for (int i = 0; i < static_cast<int>(digits.size()); ++i) {
		if (b_right.digits[i] != digits[i]) return false;
	}
	return true;
}
bool BigInteger::operator!=(const BigInteger& b_right) const {
	if (*this == b_right) return false;
	else return true;
}
bool BigInteger::operator<(const BigInteger& b_right) const {
	if (*this == b_right) return false;
	if (sign) {
		if (b_right.sign) return ((-b_right) < (-*this));
		else return true;
	}
	else if (b_right.sign) return false;
	else {
		if (digits.size() != b_right.digits.size()) {
			return digits.size() < b_right.digits.size();
		}
		else {
			for (int i = static_cast<int>(digits.size() - 1); i >= 0; --i) {
				if (digits[i] != b_right.digits[i]) return digits[i] < b_right.digits[i];
			}
			return false;
		}
	}
}
bool BigInteger::operator>(const BigInteger& b_right) const {
	if (*this == b_right || *this < b_right) return false;
	else return true;
}
bool BigInteger::operator <=(const BigInteger& b_right) const {
	if (*this < b_right || *this == b_right) return true;
	else return false;
}
bool BigInteger::operator>=(const BigInteger& b_right) const {
	if (*this == b_right || *this > b_right) return true;
	else return false;
}

//-------------------------СОСТАВНОЕ ПРИСВАИВАНИЕ-------------------------------------------------------------------
BigInteger& BigInteger::operator+=(const BigInteger& b) {
	if (sign) {
		if (b.sign) {
			return ChangeSign(ChangeSign(*this) += (-b));
		}
		else {
			return ChangeSign(ChangeSign(*this) -= b);
		}
	}
	else if (b.sign) return (*this -= (-b));
	else {
		int transfer = 0;
		for (size_t i = 0; i < std::max(digits.size(), b.digits.size()) || transfer != 0; ++i) {
			if (i == digits.size()) digits.push_back(0);
			digits[i] += transfer + (i < b.digits.size() ? b.digits[i] : 0);
			transfer = digits[i] >= base;
			if (transfer != 0) digits[i] -= base;
		}
		return *this;
	}
}
BigInteger& BigInteger::operator-=(const BigInteger& b) {
	if (sign) {
		if (b.sign) return ChangeSign(ChangeSign(*this) -= (-b));
		else return ChangeSign(ChangeSign(*this) += (b));
	}
	else if (b.sign) {
		return *this += -b;
	}
	else {
		if (*this < b) {
			BigInteger temp = *this;
			*this = b;
			return ChangeSign((*this -= temp));
		}
		else {
			int transfer = 0;
			for (size_t i = 0; i < b.digits.size() || transfer != 0; ++i) {
				digits[i] -= transfer + (i < b.digits.size() ? b.digits[i] : 0);
				transfer = digits[i] < 0;
				if (transfer != 0) digits[i] += base;
			}
			this->remove_leading_zeros();
			return *this;
		}
	}
}
BigInteger& BigInteger::operator*=(const BigInteger& b) {
	BigInteger result;
	result.digits.resize(digits.size() + b.digits.size());
	for (int i = 0; i < static_cast<int>(digits.size()); ++i) {
		int transfer = 0;
		for (int j = 0; j < static_cast<int>(b.digits.size()) || transfer != 0; ++j) {
			int res = result.digits[i + j] + digits[i] * (j < static_cast<int>(b.digits.size()) ? b.digits[j] : 0) + transfer;
			result.digits[i + j] = res % base;
			transfer = res / base;
		}
	}
	result.sign = (sign != b.sign);
	result.remove_leading_zeros();
	*this = result;
	return *this;
}
BigInteger& BigInteger::operator/=(const BigInteger& b) {
	BigInteger big = b;
	BigInteger copy = *this;
	digits.clear();
	big.sign = 0;
	BigInteger current = 0;
	digits.resize(copy.digits.size());
	for (int i = static_cast<int>(digits.size()) - 1; i >= 0; --i) {
		current.get_r();
		current.digits[0] = copy.digits[i];
		current.remove_leading_zeros();
		int left = 0, right = base;
		int temp = 0;
		while (right >= left) {
			int middle = (left + right) / 2;
			BigInteger turn = big * BigInteger(middle);
			if (turn <= current) {
				temp = middle;
				left = middle + 1;
			}
			else right = middle - 1;
		}
		BigInteger turn = big * BigInteger(left);
		if (turn <= current) temp = left;
		digits[i] = temp;
		current -= big * BigInteger(temp);
	}
	sign = (sign != b.sign);
	remove_leading_zeros();
	return *this;
}
BigInteger& BigInteger::operator%=(const BigInteger& b) {
	*this = *this - (*this / b) * b;
	return *this;
}

//-------------------------ОСНОВНЫЕ ОПЕРАЦИИ-------------------------------------------------------------------------
BigInteger operator+(const BigInteger& b_left, const BigInteger& b_right) {
	BigInteger copy = b_left;
	copy += b_right;
	return copy;
}
BigInteger operator-(const BigInteger& b_left, const BigInteger& b_right) {
	BigInteger copy = b_left;
	copy -= b_right;
	return copy;
}
BigInteger operator*(const BigInteger& b_left, const BigInteger& b_right) {
	BigInteger copy = b_left;
	copy *= b_right;
	return copy;
}
BigInteger operator/(const BigInteger& b_left, const BigInteger& b_right) {
	BigInteger copy = b_left;
	copy /= b_right;
	return copy;
}
BigInteger operator%(const BigInteger& b_left, const BigInteger& b_right) {
	BigInteger copy = b_left - (b_left / b_right) * b_right;
	return copy;
}

//-------------------------ИНКРЕМЕНТЫ И ДЕКРЕМЕНТЫ--------------------------------------------------------------------
BigInteger& BigInteger::operator++() {
	*this += 1;
	return *this;
}
BigInteger BigInteger::operator++(int) {
	BigInteger copy = *this;
	*this += 1;
	return copy;
}
BigInteger& BigInteger::operator--() {
	*this -= 1;
	return *this;
}
BigInteger BigInteger::operator--(int) {
	BigInteger copy = *this;
	*this -= 1;
	return copy;
}

//-------------------------ВВОД И ВЫВОД-------------------------------------------------------------------------------
std::ostream& operator <<(std::ostream& out, const BigInteger& b) {
	if (b.digits.size() == 0) out << 0;
	else {
		std::string str = b.toString();
		for (size_t i = 0; i < str.size(); i++)
			out << str[i];
	}
	return out;
}
std::istream& operator>>(std::istream& in, BigInteger& b) {
	char c;
	std::string str;
	while (in.read(&c, 1)) {
		if (c == ' ' || c == '\0' || c == '\n') break;
		str += c;
	}
	b.digits.clear();
	if (str.length() == 0)
		b.sign = false;
	else
	{
		if (str[0] == '-') {
			b.sign = true;
			str = str.substr(1);
		}
		else b.sign = false;
		for (long long i = str.length(); i > 0; i -= 4) {
			if (i < 4)
				b.digits.push_back(atoi(str.substr(0, i).c_str()));
			else
				b.digits.push_back(atoi(str.substr(i - 4, 4).c_str()));
		}
	}
	while (b.digits.size() > 1 && b.digits.back() == 0) {
		b.digits.pop_back();
	}
	if (b.digits.size() == 1 && b.digits[0] == 0) b.sign = false;
	return in;
}

//-------------------------ПРЕОБРАЗОВАНИЯ-----------------------------------------------------------------------------
BigInteger::operator std::string() {
	std::string s = "";
	if (digits.size() == 0) s += '0';
	else {
		if (sign) s += '-';
		for (int i = digits.size() - 1; i >= 0; --i) {
			if (i != static_cast<int>(digits.size()) - 1 && digits[i] / 1000 == 0) {
				if (digits[i] == 0) {
					s += "0000";
					continue;
				}
				int temp = digits[i];
				int sum = 0;
				while (temp != 0) {
					sum++;
					temp /= 10;
				}
				sum = 4 - sum;
				while (sum != 0) {
					s += '0';
					sum--;
				}
				std::string t = std::to_string(digits[i]);
				s += t;
			}
			else {
				std::string temp = std::to_string(digits[i]);
				s += temp;
			}
		}
	}
	return s;
}
BigInteger::operator bool() {
	BigInteger temp = 0;
	if (*this == temp) return false;
	else return true;
}

//-------------------------ОПЕРАТОРЫ BI-------------------------------------------------------------------------------
BigInteger operator""_bi(const char* x) {
	return BigInteger(x);
}
BigInteger operator""_bi(unsigned long long int x) {
	return BigInteger(x);
}
BigInteger operator""_bi(char x) {
	return BigInteger(x);
}
BigInteger NOD(BigInteger b1, BigInteger b2) {
	// для Rational, бинарный алгоритм Евклида, сделал без реккурсии, чтобы было меньше копий
	if (b1 < 0) b1.change_minus();// Чтобы не создавать копии при обработке в to_irreducible_fraction, сделаю так
	if (b2 < 0) b2.change_minus();
	BigInteger ans = 1;
	while (true) {
		if (b1 == 0) return b2 * ans;
		if (b2 == 0) return b1 * ans;
		if (b1 == 1) return ans;
		if (b2 == 1) return ans;
		else if (b1.is_even() && b2.is_even()) {
			b1.fast_divide_2();// Без него TL
			b2.fast_divide_2();
			ans *= 2;
		}
		else if (!(b1.is_even()) && b2.is_even()) b2.fast_divide_2();
		else if (b1.is_even() && !(b2.is_even())) b1.fast_divide_2();
		else if (!(b1.is_even()) && !(b2.is_even()) && b1 >= b2) {
			b1 -= b2;
			b1.fast_divide_2();
		}
		else if (!(b1.is_even()) && !(b2.is_even()) && b1 < b2) {
			b2 -= b1;
			b2.fast_divide_2();
		}
	}
}
//--------------------------------------------------------------------------------------------------------------------




//-------------------------RATIONAL-----------------------------------------------------------------------------------
class Rational {

	BigInteger numerator = 0;
	BigInteger denominator = 1;
public:
	Rational();
	Rational(const BigInteger&, const BigInteger&);
	Rational(int);

	Rational& operator=(Rational);
	bool operator==(const Rational&) const;
	bool operator!=(const Rational&) const;
	bool operator<=(const Rational&) const;
	bool operator>=(const Rational&) const;
	bool operator<(const Rational&) const;
	bool operator>(const Rational&) const;

	Rational& operator+=(const Rational&);
	Rational& operator-=(const Rational&);
	Rational& operator*=(const Rational&);
	Rational& operator/=(const Rational&);

	std::string toString() const;
	std::string asDecimal(size_t) const;
	explicit operator double() const;

	friend Rational operator+(const Rational&, const Rational&);
	friend Rational operator-(const Rational&, const Rational&);
	friend Rational operator*(const Rational&, const Rational&);
	friend Rational operator/(const Rational&, const Rational&);
	BigInteger support(const BigInteger& b) {
		BigInteger copy = b;
		if (copy >= 0) return copy;
		else
		{
			copy *= -1;
			return copy;
		}
	}
	Rational operator-() const;
	void to_irreducible_fraction(BigInteger& b1, BigInteger& b2) {
		if (b2 == 1) return;
		else if (b2 == -1) {
			b2 *= -1;
			b1 *= -1;
		}
		else {
			BigInteger nod = NOD(support(b1), support(b2));
			if ((b1 >= 0 && b2 > 0) || (b1 < 0 && b2 < 0)) {
				b1 = support(b1 / nod);
				b2 = support(b2 / nod);
			}
			else
			{
				b1 = -support(b1 / nod);
				b2 = support(b2 / nod);
			}
		}
	}
	friend std::istream& operator >> (std::istream&, Rational&);
};
std::istream& operator >> (std::istream& in, Rational& r) {
	int elem;
	in >> elem;
	r = Rational(elem);
	return in;
}
//------------------------------КОНСТРУКТОРЫ--------------------------------------------
Rational::Rational() {
	numerator = 0;
	denominator = 1;
}
Rational::Rational(int i) :Rational(i, 1) {};
Rational::Rational(const BigInteger& b1, const BigInteger& b2 = 1) {
	numerator = b1;
	denominator = b2;
	to_irreducible_fraction(numerator, denominator);
}

//------------------------------БУЛЕВЫ ОПЕРАЦИИ-----------------------------------------
bool Rational::operator==(const Rational& r) const {
	if (numerator == r.numerator && denominator == r.denominator) return true;
	else return false;
}
bool Rational::operator!=(const Rational& r) const {
	if (numerator == r.numerator && denominator == r.denominator) return false;
	else return true;
}
bool Rational::operator<=(const Rational& r) const {
	if (*this < r || *this == r) return false;
	else return true;
}
bool Rational::operator>=(const Rational& r) const {
	if (*this > r || *this == r) return true;
	else return false;
}
bool Rational::operator<(const Rational& r) const {
	if (numerator < 0 && r.numerator >= 0) return true;
	else if (numerator >= 0 && r.numerator < 0) return false;
	else if (numerator >= 0 && r.numerator >= 0)return numerator * r.denominator < r.numerator* denominator;
	else return numerator * r.denominator > r.numerator * denominator;
}
bool Rational::operator>(const Rational& r) const {
	if (*this < r) return false;
	else return true;
}

//------------------------------СОСТАВНОЕ ПРИСВАИВАНИЕ И КОПИРОВАНИЕ----------------------------------
Rational& Rational::operator=(Rational r) {
	std::swap(numerator, r.numerator);
	std::swap(denominator, r.denominator);
	return *this;
}
Rational& Rational::operator+=(const Rational& r) {
	numerator = numerator * r.denominator + r.numerator * denominator;
	denominator = denominator * r.denominator;
	to_irreducible_fraction(numerator, denominator);
	return *this;
}
Rational& Rational::operator-=(const Rational& r) {
	numerator = numerator * r.denominator - r.numerator * denominator;
	denominator = denominator * r.denominator;
	to_irreducible_fraction(numerator, denominator);
	return *this;
}
Rational& Rational::operator*=(const Rational& r) {
	numerator *= r.numerator;
	denominator *= r.denominator;
	to_irreducible_fraction(numerator, denominator);
	return *this;
}
Rational& Rational::operator/=(const Rational& r) {
	numerator *= r.denominator;
	denominator *= r.numerator;
	to_irreducible_fraction(numerator, denominator);
	return *this;
}

//------------------------------ОСНОВНЫЕ ОПЕРАЦИИ----------------------------------------
Rational operator+(const Rational& r1, const Rational& r2) {
	Rational copy = r1;
	copy += r2;
	return copy;
}
Rational operator-(const Rational& r1, const Rational& r2) {
	Rational copy = r1;
	copy -= r2;
	return copy;
}
Rational operator*(const Rational& r1, const Rational& r2) {
	Rational copy = r1;
	copy *= r2;
	return copy;
}
Rational operator/(const Rational& r1, const Rational& r2) {
	Rational copy = r1;
	copy /= r2;
	return copy;
}

//-------------------------------ПРОЧИЕ---------------------------------------------------
Rational Rational::operator-() const {
	Rational copy = *this;
	if (numerator == 0) return copy;
	else {
		copy.numerator *= -1;
		return copy;
	}
}
std::string Rational::toString() const {
	std::string s = "";

	if (numerator == 0) {
		s += '0';
		return s;
	}
	if (denominator == 1) {
		s += numerator.toString();;
		return s;
	}
	else {
		s += numerator.toString();
		s += '/';
		s += denominator.toString();
		return s;
	}
}
std::string Rational::asDecimal(size_t prec = 0) const {
	std::string ans;
	if (numerator < 0) ans += '-';
	BigInteger numer_new = (numerator > 0 ? numerator : -numerator);
	BigInteger z = numer_new / denominator;
	BigInteger irra = numer_new % denominator;
	ans += z.toString();
	if (prec > 0) {
		ans += '.';
		for (size_t i = 0; i < prec; ++i) {
			irra *= 10;
			ans += (irra / denominator).toString();
			irra %= denominator;
		}
	}
	return ans;
}
Rational::operator double() const {
	std::string temp = asDecimal(16);
	double ans = std::atof(temp.c_str());
	return ans;
}
Rational abs(const Rational& r) {
	// Нужно для вычислений в Finite и Matrix
	if (r > 0) return r;
	else return -r;
}

//#####################################################################################################
//#####################################################################################################
//#####################################################################################################

//-----------------------------------------Finite------------------------------------------------------
template<int N>
class Finite {
	long long element;
public:
	Finite(long long);
	Finite(const Finite&);

	Finite& operator+=(const Finite<N>&);
	Finite& operator-=(const Finite<N>&);
	Finite& operator*=(const Finite<N>&);
	Finite& operator/=(const Finite<N>&);

	Finite& operator++();
	Finite& operator++(int);

	void gcdex(long long f1, long long  f2, long long& x, long long& y) {
		// Решение линейного диофантового уравнения для нахождения обратного элемента (нашел...)
		if (f2 == 0) {
			x = 1;
			y = 0;
			return;
		}
		long long x1, y1;
		gcdex(f2, f1 % f2, x1, y1);
		x = y1;
		y = x1 - (f1 / f2) * y1;
		return;
	}
	long long reverse_elem(long long elem) {
		// Нахождение обратного элемента через диофантово
		long long x, y;
		gcdex(elem, N, x, y);
		long long rever_elem = in_finite(x);
		return rever_elem;
	}

	int elem_finite_to_int() const {
		// Нужно для дальнейших операций сравнения и т.п.
		return element;
	}
	long long  in_finite(long long elem) {
		//-10%3=-1 в с++, поэтому нужно сделать отдельное преобразование
		if (elem >= 0) return (elem % N);
		else {
			elem = (elem + N * (std::abs(elem / N) + 1)) % N;
			return elem;
		}
	}

	~Finite() = default;
};
template<int N>
int abs(const Finite<N>& f) {
	// Нужно для вычислений в матрицах
	if (f.elem_finite_to_int() >= 0) return f.elem_finite_to_int();
	else return f.elem_finite_to_int();
}
//----------------------------------КОНСТРУКТОР--------------------------------------------------------
template<int N>
Finite<N>::Finite(const Finite& elem) {
	element = elem.element;
}
template<int N>
Finite<N>::Finite(long long elem) {
	element = in_finite(elem);
}
//-----------------------------------------СОСТАВНОЕ ПРИСВАИВАНИЕ--------------------------------------
template<int N>
Finite<N>& Finite<N>::operator+=(const Finite<N>& elem) {
	element += elem.element;
	element = in_finite(element);
	return *this;
}
template<int N>
Finite<N>& Finite<N>::operator-=(const Finite<N>& elem) {
	element -= elem.element;
	element = in_finite(element);
	return *this;
}
template<int N>
Finite<N>& Finite<N>::operator*=(const Finite<N>& elem) {
	element *= elem.element;
	element = in_finite(element);
	return *this;
}
template<int N>
Finite<N>& Finite<N>::operator/=(const Finite<N>& elem) {
	element *= reverse_elem(elem.element);
	element = in_finite(element);
	return *this;
}
//-------------------------------------------ИНКРЕМЕНТЫ И ДЕКРЕМЕНТЫ (пригодятся)-----------------------
template<int N>
Finite<N>& Finite<N>::operator++() {
	*this += 1;
	return *this;
}
template<int N>
Finite<N>& Finite<N>::operator++(int) {
	Finite<N> copy = *this;
	++* this;
	return copy;
}
//-------------------------------------------Булевы операции от двух аргументов-------------------------
template<int N, int K>
bool operator==(const Finite<N>& fn, const Finite<K>& fk) {
	if (N != K) return false;
	else if (fn.elem_finite_to_int() == fk.elem_finite_to_int()) return true;
	else return false;
}
template<int N, int K>
bool operator!=(const Finite<N>& fn, const Finite<K>& fk) {
	if (fn == fk) return false;
	else return true;
}
template<int N>
bool operator==(const Finite<N>& fn, int elem) {
	// Проверка справа
	if (fn.elem_finite_to_int() == elem) return true;
	else return false;
}
template<int N>
bool operator==(int elem, const Finite<N>& fn) {
	// Проверка слева
	if (fn.elem_finite_to_int() == elem) return true;
	else return false;
}
template<int N>
bool operator!=(const Finite<N>& fn, int elem) {
	// Проверка справа
	if (fn == elem) return false;
	else return true;
}
template<int N>
bool operator!=(int elem, const Finite<N>& fn) {
	// Проверка слева
	if (fn == elem) return false;
	else return true;
}

//-------------------------------------------Основные операции-----------------------------------------
template<int N>
Finite<N> operator+(const Finite<N>& f1, const Finite<N>& f2) {
	Finite<N> result = f1;
	result += f2;
	return result;
}
template<int N>
Finite<N> operator-(const Finite<N>& f1, const Finite<N>& f2) {
	Finite<N> result = f1;
	result -= f2;
	return result;
}
template<int N>
Finite<N> operator*(const Finite<N>& f1, const Finite<N>& f2) {
	Finite<N> result = f1;
	result *= f2;
	return result;
}
template<int N>
Finite<N> operator/(const Finite<N>& f1, const Finite<N>& f2) {
	Finite<N> result = f1;
	result /= f2;
	return result;
}

//-----------------------------------------------------------------------------------------------------


//-----------------------------------------Matrix------------------------------------------------------
template<unsigned M, unsigned N, typename Field = Rational>
class Matrix {
	std::vector<std::vector<Field>> matrix;
public:
	Matrix();
	template<typename T>
	Matrix(std::vector<std::vector<T>>);
	Matrix(const Matrix<M, N, Field>& mat1);

	Matrix<M, N, Field>& operator+=(const Matrix<M, N, Field>& matrx);
	Matrix<M, N, Field>& operator-=(const Matrix<M, N, Field>& matrx);
	Matrix<M, N, Field>& operator*=(Field elem);

	const std::vector<Field>& operator[](size_t) const;
	std::vector<Field>& operator[](size_t pos);

	Matrix<N, M, Field> transposed() const;
	long long rank() const;
	Field trace() const;

	std::vector<Field> getRow(unsigned pos) const;
	std::vector<Field> getColumn(unsigned pos) const;

	~Matrix() = default;
};
//------------------------------------------КОНСТРУКТОРЫ----------------------------------------------
template<unsigned M, unsigned N, typename Field>
Matrix<M,N,Field>::Matrix() {
	matrix.resize(M, std::vector<Field>(N, 0));
}
template<unsigned M, unsigned N, typename Field>
template<typename T>
Matrix<M, N, Field>::Matrix(std::vector<std::vector<T>> vect) {
	matrix.resize(vect.size(), std::vector < Field >(vect[0].size(), 0));
	for (size_t i = 0; i < vect.size(); ++i) {
		for (size_t j = 0; j < vect[i].size(); ++j) {
			matrix[i][j] = vect[i][j];
		}
	}
}
template<unsigned M, unsigned N, typename Field>
Matrix<M, N, Field>::Matrix(const Matrix<M, N, Field>& mat1) {
	matrix.resize(M, std::vector<Field>(N, 0));
	for (size_t i = 0; i < M; ++i) {
		for (size_t j = 0; j < N; ++j) {
			matrix[i][j] = mat1[i][j];
		}
	}
}

//--------------------------------------СОСТАВНОЕ ПРИСВАИВАНИЕ------------------------------------------
template<unsigned M, unsigned N, typename Field>
Matrix<M, N, Field>& Matrix<M, N, Field>::operator+=(const Matrix<M, N, Field>& matrx) {
	for (size_t i = 0; i < matrix.size(); ++i) {
		for (size_t j = 0; j <matrix[0].size(); ++j) {
			matrix[i][j] += matrx.matrix[i][j];
		}
	}
	return *this;
}
template<unsigned M, unsigned N, typename Field>
Matrix<M, N, Field>& Matrix<M, N, Field>::operator-=(const Matrix<M, N, Field>& matrx) {
	for (size_t i = 0; i <matrix.size(); ++i) {
		for (size_t j = 0; j < matrix[0].size(); ++j) {
			matrix[i][j] -= matrx.matrix[i][j];
		}
	}
	return *this;
}
template<unsigned M, unsigned N, typename Field>
Matrix<M, N, Field>& Matrix<M, N, Field>::operator*=(Field elem) {
	for (size_t i = 0; i < matrix.size(); ++i) {
		for (size_t j = 0; j < matrix[0].size(); ++j) {
			matrix[i][j] *= elem;
		}
	}
	return *this;
}

//--------------------------------------КВАДРАТНЫЕ СКОБКИ---------------------------------------------
template<unsigned M, unsigned N, typename Field>
const std::vector<Field>& Matrix<M,N,Field>::operator[](size_t pos) const {
	return matrix[pos];
}
template<unsigned M, unsigned N, typename Field>
std::vector<Field>& Matrix<M, N, Field>::operator[](size_t pos) {
	return matrix[pos];
}

//------------------------------------ПРОЧИЕ ОПЕРАЦИИ------------------------------------------------
template<unsigned M, unsigned N, typename Field>
Matrix<N, M, Field> Matrix<M,N,Field>::transposed() const {
	Matrix<N, M, Field> result;
	for (size_t i = 0; i < matrix.size(); ++i) {
		for (size_t j = 0; j < matrix[0].size(); ++j) {
			result[j][i] = matrix[i][j];
		}
	}
	return result;
}
template<unsigned M, unsigned N, typename Field>
long long Matrix<M, N, Field>::rank() const {// Нахождение ранга с помощью модифицированного метода Гаусса
	long long rk = N;
	Matrix<M, N, Field> copy = *this;
	std::vector<bool> is_ready_line(M, 0);
	for (int i = 0; i < static_cast<int>(N); ++i) {
		int j = 0;
		for (j = 0; j < static_cast<int>(M); ++j) {
			if (!is_ready_line[j] && copy[j][i] != 0) // Ищу первую ненулевую ячейку в i-ом столбце, с учетом готовности строки
				break;
		}
		if (j == static_cast<int>(M)) --rk; // Если весь столбец нулевой, просто пропустим его и вычтем 1 из ранга,
		// т.е. забудем "выкинем" его из матрицы
		else {
			is_ready_line[j] = 1; // Обрабатываем строку j, где был найден первый не нулевой
			for (int k = i + 1; k < static_cast<int>(N); ++k) {// k берем уже с учетом сделанных операций
				copy[j][k] /= copy[j][i]; // Приведем эту ячейку к единичному виду, поделив всю строку на её значение
			}
			for (int l = 0; l < static_cast<int>(M); ++l) {// Зануляем все прочие элементы в рассматриваемом столбце
				if (l != j && copy[l][i] != 0) {
					for (int k = i + 1; k < static_cast<int>(N); ++k) {
						copy[l][k] -= copy[j][k] * copy[l][i];// copy[l][i] - на сколько надо умножить строку с первым ненулевым,
						// чтобы занулить ячейку в рассматриваемой на данный момент строке.
						// Т.е. вычитаем друг из друга соответствующие значения с этим коэффицентом
					}
				}
			}
		}
	}
	return rk;
}
template<unsigned M, unsigned N, typename Field>
Field  Matrix<M, N, Field>::trace() const {
	// Сумма элементов на главной диагонали квадратной подматрицы
	Field trace = 0;
	for (size_t i = 0; i < static_cast<int>(std::min(N, M)); ++i) {
		trace += matrix[i][i];
	}
	return trace;
}
template<unsigned M, unsigned N, typename Field>
std::vector<Field> Matrix<M, N, Field>::getRow(unsigned pos) const {
	std::vector < Field> row = matrix[pos];
	return row;
}
template<unsigned M, unsigned N, typename Field>
std::vector<Field> Matrix<M, N, Field>::getColumn(unsigned pos) const {
	std::vector<Field>  column(matrix[pos].size());
	for (size_t i = 0; i < matrix[pos].size(); ++i) {
		column[i] = matrix[pos][i];
	}
	return column;
}

//-----------------------------------------------------------------------------------------------------
// Основные операции и операции сравнения после определения квадратной матрицы

//-----------------------------------------------------------------------------------------------------

//----------------------------------------Квадратная матрица---------------------------------------------

template<unsigned M, typename Field>
class Matrix<M, M, Field> {
	std::vector<std::vector<Field>> matrix;
public:
	// Функции из реализации произвольной матрицы

	Matrix();
	template<typename T>
	Matrix(std::vector<std::vector<T>>);
	Matrix(const Matrix<M, M, Field>& );

	Matrix<M, M, Field>& operator+=(const Matrix<M, M, Field>&);
	Matrix<M, M, Field>& operator-=(const Matrix<M, M, Field>&);
	Matrix<M, M, Field>& operator*=(Field);

	const std::vector<Field>& operator[](size_t) const;
	std::vector<Field>& operator[](size_t);

	Matrix<M, M, Field> transposed() const;
	long long rank() const;
	Field trace() const;

	std::vector<Field> getRow(unsigned pos) const;
	std::vector<Field> getColumn(unsigned pos) const;

	// Функции, реализуемые только для квадратной матрицы

	Matrix<M, M, Field>& operator*=(const Matrix<M, M, Field>&);
	Field det() const;
	void invert();
	Matrix<M, M, Field> inverted();

	~Matrix() = default;
};
//------------------------------------------КОНСТРУКТОРЫ----------------------------------------------
template<unsigned M, typename Field>
Matrix<M,M,Field>::Matrix() {
	matrix.resize(static_cast<int>(M), std::vector<Field>(static_cast<int>(M), 0));
	for (size_t i = 0; i < M; ++i) {
		matrix[i][i] = 1;
	}
}
template<unsigned M, typename Field>
template<typename T>
Matrix<M, M, Field>::Matrix(std::vector<std::vector<T>> vect) {
	matrix.resize(vect.size(), std::vector < Field >(vect[0].size(), 0));
	for (size_t i = 0; i < vect.size(); ++i) {
		for (size_t j = 0; j < vect[i].size(); ++j) {
			matrix[i][j] = vect[i][j];
		}
	}
}
template<unsigned M, typename Field>
Matrix<M, M, Field>::Matrix(const Matrix<M, M, Field>& mat1) {
	matrix.resize(M, std::vector<Field>(M, 0));
	for (size_t i = 0; i < M; ++i) {
		for (size_t j = 0; j < M; ++j) {
			matrix[i][j] = mat1[i][j];
		}
	}
}

//-----------------------------------------СОСТАВНОЕ ПРИСВАИВАНИЕ------------------------------------
template<unsigned M,typename Field>
Matrix<M, M, Field>& Matrix<M, M, Field>::operator+=(const Matrix<M, M, Field>& matrx) {
	for (size_t i = 0; i < matrix.size(); ++i) {
		for (size_t j = 0; j < matrix[0].size(); ++j) {
			matrix[i][j] += matrx.matrix[i][j];
		}
	}
	return *this;
}
template<unsigned M,typename Field>
Matrix<M, M, Field>& Matrix<M, M, Field>::operator-=(const Matrix<M, M, Field>& matrx) {
	for (size_t i = 0; i < matrix.size(); ++i) {
		for (size_t j = 0; j < matrix[0].size(); ++j) {
			matrix[i][j] -= matrx.matrix[i][j];
		}
	}
	return *this;
}
template<unsigned M,typename Field>
Matrix<M, M, Field>& Matrix<M, M, Field>::operator*=(Field elem) {
	for (size_t i = 0; i < matrix.size(); ++i) {
		for (size_t j = 0; j < matrix[0].size(); ++j) {
			matrix[i][j] *= elem;
		}
	}
	return *this;
}

//--------------------------------------КВАДРАТНЫЕ СКОБКИ---------------------------------------------
template<unsigned M,typename Field>
const std::vector<Field>& Matrix<M,M,Field>::operator[](size_t pos) const {
	return matrix[pos];
}
template<unsigned M,typename Field>
std::vector<Field>& Matrix<M, M, Field>::operator[](size_t pos) {
	return matrix[pos];
}
//------------------------------------ПРОЧИЕ ОПЕРАЦИИ------------------------------------------------
template<unsigned M,typename Field>
Matrix<M, M, Field> Matrix<M, M, Field>::transposed() const {
	Matrix<M, M, Field> result;
	for (size_t i = 0; i < matrix.size(); ++i) {
		for (size_t j = 0; j < matrix[i].size(); ++j) {
			result[j][i] = matrix[i][j];
		}
	}
	return result;
}
template<unsigned M,typename Field>
long long Matrix<M, M, Field>::rank() const {
	// Все рассуждения полностью аналогичны нахождению ранга произвольной матрицы
	long long rk = M;
	Matrix<M, M, Field> copy = *this;
	std::vector<bool> is_ready_line(M, 0);
	for (int i = 0; i < static_cast<int>(M); ++i) {
		int j = 0;
		for (j = 0; j < static_cast<int>(M); ++j) {
			if (!is_ready_line[j] && copy[j][i] != 0)
				break;
		}
		if (j == static_cast<int>(M)) --rk;
		else {
			is_ready_line[j] = 1;
			for (int k = i + 1; k < static_cast<int>(M); ++k) {
				copy[j][k] /= copy[j][i];
			}
			for (int l = 0; l < static_cast<int>(M); ++l) {
				if (l != j && copy[l][i] != 0) {
					for (int k = i + 1; k < static_cast<int>(M); ++k) {
						copy[l][k] -= copy[j][k] * copy[l][i];
					}
				}
			}
		}
	}
	return rk;
}
template<unsigned M,typename Field>
Field  Matrix<M, M, Field>::trace() const {// Сумма элементов на главной диагонали квадратной подматрицы
	Field trace = 0;
	for (size_t i = 0; i < M; ++i) {
		trace += matrix[i][i];
	}
	return trace;
}
template<unsigned M,typename Field>
std::vector<Field> Matrix<M, M, Field>::getRow(unsigned pos) const {
	std::vector < Field> row = matrix[static_cast<int>(pos)];
	return row;
}
template<unsigned M,typename Field>
std::vector<Field> Matrix<M, M, Field>::getColumn(unsigned pos) const {
	std::vector<Field>  column;
	for (size_t i = 0; i < matrix[static_cast<int>(pos)].size(); ++i) {
		column.push_back(matrix[pos][i]);
	}
	return column;
}
//------------------------------ПРОЧИЕ ОПЕРАЦИИ ДЛЯ КВАДРАТНОЙ МАТРИЦЫ----------------------------------
template<unsigned M, typename Field>
Matrix<M, M, Field>& Matrix<M,M,Field>::operator*=(const Matrix<M, M, Field>& matr) {
	Matrix<M, M, Field> copy = *this;
	for (size_t i = 0; i < static_cast<int>(M); ++i) {
		for (size_t j = 0; j < static_cast<int>(M); ++j) {
			matrix[i][j] = 0;
			for (size_t p = 0; p < static_cast<int>(M); ++p) {
				matrix[i][j] += copy[i][p] * matr[p][j];
			}
		}
	}
	return *this;
}
template<unsigned M, typename Field>
Field Matrix<M,M,Field>::det() const {
	// Привожу методом Гаусса матрицу к диагональному виду, после определитель - произведение диагональных значений
	// Не использую деление строки на константу, поэтому может меняться только по знаку
	Matrix<M, M, Field> copy = *this;
	Field det = 1;
	for (size_t i = 0; i < static_cast<int>(M); ++i) {
		size_t i1 = i;
		for (size_t j = i + 1; j < static_cast<int>(M); ++j) {
			if (abs(copy[j][i]) > abs(copy[i1][i])) i1 = j;
			// Ищем бОльший элемент в i-ом столбце, чтобы сделать проверку на зануление столбца
		}
		if (copy[i1][i] == 0) {
			// Проверка на зануление столбца
			det = 0;
			break;
		}
		std::swap(copy[i], copy[i1]);// Поставили наибольшее число в столбце в диагональ
		if (i != i1) det *= Field(-1); // Если строки все-таки поменялись
		det *= copy[i][i];
		for (size_t j = i + 1; j < static_cast<int>(M); ++j) {
			copy[i][j] /= copy[i][i];
		}
		// Далее вычитаем из всех строк найденную
		// Рассуждения аналогичны нахождению ранга
		for (size_t j = 0; j < static_cast<int>(M); ++j) {
			if (j != i && copy[j][i] != Field(0)) {
				for (size_t i2 = i + 1; i2 < static_cast<int>(M); ++i2) {
					copy[j][i2] -= copy[j][i] * copy[i][i2];
				}
			}
		}
	}
	return det;
}
template<unsigned M, typename Field>
void Matrix<M,M,Field>::invert() {
	// Нахождение преобразованием исходной матрицы до единичной, преобразовывая теми же действиями единичную
	// Преобразовываем также методом Гаусса по столбцам
	Matrix<M, M, Field> result;
	for (size_t i = 0; i < M; ++i) {
		size_t first_no_zero = i;
		for (size_t j = i; j < M; ++j) {
			if (matrix[j][i] != Field(0)) {
				first_no_zero = j;
				break;
			}
		}
		std::swap(matrix[i], matrix[first_no_zero]);
		std::swap(result[i], result[first_no_zero]);
		// Теперь обнулим весь оставшийся столбец, вычитанием из соответвующих строк рассматриваемую
		for (size_t j = 0; j < M; ++j) {
			if (j != i) {
				Field coef = matrix[j][i] / matrix[i][i];// Коэффициент, чтобы занулить соотвествующую строке ячейку
				// в рассматриваемом столбце
				for (size_t l = 0; l < M; ++l) {
					matrix[j][l] -= matrix[i][l] * coef;// Также вычитаем и из всех других ячеек строки соответсвующие
					// значения из рассматриваемой строки
					result[j][l] -= result[i][l] * coef;// Аналогично с обратной
				}
			}
		}
		// Теперь приведем к единичному виду соответствующую ячейку делением всех элементов в строке на её значение
		Field coef = matrix[i][i];
		for (size_t j = 0; j < M; ++j) {
			matrix[i][j] /= coef;
			result[i][j] /= coef;
		}
	}
	// Вернем матрицу 
	for (size_t i = 0; i < M; ++i) {
		for (size_t j = 0; j < M; ++j) {
			matrix[i][j] = result[i][j];
		}
	}
	return;
}
template<unsigned M, typename Field>
Matrix<M, M, Field> Matrix<M,M,Field>::inverted() {
	Matrix<M, M, Field> inverted = *this;
	inverted.invert();
	return inverted;
}


//--------------------------------------------Основные операции---------------------------------------
template<unsigned M, unsigned N, typename Field = Rational>
Matrix <M, N, Field> operator*(Field f, const Matrix<M, N, Field>& matr) {
	// На число слева
	Matrix<M, N, Field> result = matr;
	result *= f;
	return result;
}
template<unsigned M, unsigned N, typename Field = Rational>
Matrix <M, N, Field> operator*(const Matrix<M, N, Field>& matr, Field f) {
	// На число справа
	Matrix<M, N, Field> result = matr;
	result *= f;
	return result;
}
template<unsigned M, unsigned N, unsigned P, typename Field = Rational>
Matrix<M, N, Field> operator *(const Matrix<M, P, Field>& matr1, const Matrix<P, N, Field>& matr2) {
	// Приведет к ошибке компиляции, если умножение будет невозможно
	Matrix<M, N, Field> result;
	for (size_t i = 0; i < static_cast<int>(M); ++i) {
		for (size_t j = 0; j < static_cast<int>(N); ++j) {
			result[i][j] = 0;
			for (size_t p = 0; p < static_cast<int>(P); ++p) {
				result[i][j] += matr1[i][p] * matr2[p][j];
			}
		}
	}
	return result;
}
template<unsigned M, unsigned N, typename Field = Rational>
Matrix<M, N, Field> operator +(const Matrix<M, N, Field>& matr1, const Matrix<M, N, Field>& matr2) {
	Matrix<M, N, Field> result = matr1;
	result += matr2;
	return result;
}
template<unsigned M, unsigned N, typename Field = Rational>
Matrix<M, N, Field> operator -(const Matrix<M, N, Field>& matr1, const Matrix<M, N, Field>& matr2) {
	Matrix<M, N, Field> result = matr1;
	result -= matr2;
	return result;
}

//------------------------------------------Булевы функции--------------------------------------------
template<unsigned M, unsigned N, unsigned P, unsigned T, typename Field = Rational>
bool operator ==(const Matrix<M, N, Field>& matr1, const Matrix<P, T, Field>& matr2) {
	if (M != P || T != N) return false; // Проверка несовпадения размеров
	for (size_t i = 0; i < static_cast<int>(M); ++i) {
		if (matr1[i] != matr2[i]) return false; // Проверка по строкам
	}
	return true;
}
template<unsigned M, unsigned N, unsigned P, unsigned T, typename Field = Rational>
bool operator !=(const Matrix<M, N, Field>& matr1, const Matrix<P, T, Field>& matr2) {
	if (matr1 == matr2) return false;
	else return true;
}

template <unsigned N, typename Field = Rational>
using SquareMatrix = Matrix<N, N, Field>;
