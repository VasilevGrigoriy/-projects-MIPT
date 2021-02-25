#include <string>
#include <iostream>
#include <vector>
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
	BigInteger(std::string);
	BigInteger(int);
	BigInteger(const BigInteger&);
	BigInteger(const char*);

	operator std::string();
	explicit operator bool();

	BigInteger operator-() const;

	void get_r();
	void remove_leading_zeros();
	std::string toString() const;

	BigInteger& ChangeSign(BigInteger& b) {
		if (b == 0) return b;
		b.sign = !(b.sign);
		return b;
	}
	bool is_even() const {
		return (digits[0] % 2 == 0);
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
	else if (i == 0) {
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
	digits = b.digits;
}
BigInteger::BigInteger(std::string s) {
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
		for (int i = s.length() -log(base); i > -log(base)-1; i -= log(base)) {
			if (i < 0) {
				this->digits.push_back(atoi(s.substr(0, log(base) + i).c_str()));
			}
			else {
				this->digits.push_back(atoi(s.substr(i, log(base)).c_str()));
			}
		}
	}
	this->remove_leading_zeros();
}
BigInteger::BigInteger(const char* x):BigInteger(std::string(x)) {}

//--------------------------ПРОЧЕЕ----------------------------------------------------------------------------------
void BigInteger::remove_leading_zeros() {
	while (this->digits.size() > 1 && this->digits.back() == 0) {
		this->digits.pop_back();
	}
	if (this->digits.size() == 1 && this->digits[0] == 0) {
		this->sign = 0;
	}
}
BigInteger BigInteger::operator-() const {// Как то  ты к нему без уважения... прочее...
	BigInteger copy = *this;
	if (copy == 0) return copy;
	else {
		copy.sign = !(copy.sign);
		return copy;
	}
}
void BigInteger::get_r() {
	// перенос на разряд вперед, нужно в деление
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
	// меняю знаки, если оба не + +
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
		if (b.sign) return ChangeSign(ChangeSign(*this) -= (-b));// Здесь в момент -b так же происходит копирование, но ладно
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
			int res;
			if (j < static_cast<int>(b.digits.size())) res = result.digits[i + j] + digits[i] * b.digits[j] + transfer;
			else  res = transfer;
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
		out << str;
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
				sum = log(base) - sum;
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
// Зачем так сложно, просто return *this!=0
BigInteger::operator bool() {
	return (*this != 0);
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
	BigInteger ans = 1;
	while (true) {
		if (b1 == 0) return b2*ans;
		if (b2 == 0) return b1*ans;
		if (b1 == 1) return ans;
		if (b2 == 1) return ans;
		else if (b1.is_even() && b2.is_even()) {
			b1 /= 2;
			b2 /= 2;
			ans *= 2;
		}
		else if (!(b1.is_even()) && b2.is_even()) b2 /= 2;
		else if (b1.is_even() && !(b2.is_even())) b1 /= 2;
		else if (!(b1.is_even()) && !(b2.is_even()) && b1 >= b2) {
			b1 -= b2;
			b1 /= 2;
		}
		else if (!(b1.is_even()) && !(b2.is_even()) && b1 < b2) {
			b2 -= b1;
			b2 /= 2;
		}
	}
}
//--------------------------------------------------------------------------------------------------------------------




//-------------------------RATIONAL-----------------------------------------------------------------------------------
class Rational {

	BigInteger numerator = 0;// числитель
	BigInteger denominator = 1;// знаменатель
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
	std::string asDecimal(const size_t&) const;
	explicit operator double() const;

	BigInteger Change_minus(const BigInteger& b) {
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
		// приведение к несократимой дроби
		if (b2 == 1) return;
		else if (b2 == -1) {
			b2 *= -1;
			b1 *= -1;
		}
		else {
			BigInteger nod = NOD(Change_minus(b1), Change_minus(b2));
			if ((b1 >= 0 && b2 > 0) || (b1 < 0 && b2 < 0)) {
				b1 = Change_minus(b1 / nod);
				b2 = Change_minus(b2 / nod);
			}
			else
			{
				b1 = -Change_minus(b1 / nod);
				b2 = Change_minus(b2 / nod);
			}
		}
	}

};

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
	// в BigInt самостоятельно хранятся - , поэтому ненужно рассматривать все случаи на знаки числителя и знаменателя
	numerator = numerator * r.denominator + r.numerator * denominator;
	denominator = denominator * r.denominator;
	to_irreducible_fraction(numerator, denominator);
	return *this;
}
Rational& Rational::operator-=(const Rational& r) {
	// в BigInt самостоятельно хранятся - , поэтому ненужно рассматривать все случаи на знаки числителя и знаменателя
	numerator = numerator * r.denominator - r.numerator * denominator;
	denominator = denominator * r.denominator;
	to_irreducible_fraction(numerator, denominator);
	return *this;
}
Rational& Rational::operator*=(const Rational& r) {
	// в BigInt самостоятельно хранятся - , поэтому ненужно рассматривать все случаи на знаки числителя и знаменателя
	numerator = numerator * r.numerator;
	denominator = denominator * r.denominator;
	to_irreducible_fraction(numerator, denominator);
	return *this;
}
Rational& Rational::operator/=(const Rational& r) {
	// в BigInt самостоятельно хранятся - , поэтому ненужно рассматривать все случаи на знаки числителя и знаменателя
	numerator = numerator * r.denominator;
	denominator = denominator * r.numerator;
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
	// Через функции BigInt для числителя и знаменателя
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
std::string Rational::asDecimal(const size_t& prec = 0) const {
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
