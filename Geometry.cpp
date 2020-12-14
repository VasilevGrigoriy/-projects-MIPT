#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
using namespace std;

// функция, чтобы правильно учитывать погрешность, заданную в задаче
bool is_peer(double a, double b) {
    if (abs(a - b) < 1e-7) return true;
    else return false;
}



class Point {
public:
    double x = 0.0;
    double y = 0.0;

    Point() = default;
    Point(double, double);

    bool operator ==(const Point&) const;
    bool operator !=(const Point&) const;

    ~Point() = default;
};

//------------------------------------Конструктор----------------------------------------------
Point::Point(double x, double y) {
    this->x = x;
    this->y = y;
}

//------------------------------------Булевы операции------------------------------------------
bool Point::operator==(const Point& p) const {
    if (is_peer(this->x, p.x) && is_peer(this->y, p.y)) return true;
    else return false;
}
bool Point::operator!=(const Point& p) const {
    if (*this == p) return false;
    else return true;
}

//----------------------------------------------------------------------------------------



class Line {
public:
    double a = 0.0;
    double b = 0.0;
    double c = 0.0;

    Line() = default;
    Line(const Point&, const Point&);
    Line(double, double);
    Line(const Point&, double);

    bool operator==(const Line&) const;
    bool operator!=(const Line&) const;

    ~Line() = default;
};

//----------------------------------------Конструкторы----------------------------------------------
Line::Line(const Point& p1, const Point& p2) {
    this->a = p2.y - p1.y;
    this->b = p1.x - p2.x;
    this->c = p1.y * p2.x - p1.x * p2.y;
}
Line::Line(double k, double bb) {
    this->a = -k;
    this->b = 1;
    this->c = -bb;
}
Line::Line(const Point& p, double k) {
    this->a = -k;
    this->b = 1;
    this->c = k * p.x - p.y;
}

//----------------------------------------Булевы операции-------------------------------------------
bool Line::operator==(const Line& l) const {
    double k = a / l.a;
    if ((a == 0 && l.a != 0) || (a != 0 && l.a == 0)) return false;
    if ((b == 0 && l.b != 0) || (b != 0 && l.b == 0)) return false;
    if ((c == 0 && l.c != 0) || (c != 0 && l.c == 0)) return false;
    if (is_peer(a, k * l.a) && is_peer(b, k * l.b) && is_peer(c, k * l.c)) return true;
    else return false;
}
bool Line::operator!=(const Line& l) const {
    if (*this == l) return false;
    else return true;
}

//----------------------------------------------------------------------------------------



class Shape {
public:
    virtual double perimeter() const = 0;
    virtual double area() const = 0;

    virtual bool operator==(const Shape&) const = 0;
    virtual bool operator!=(const Shape&) const = 0;
    virtual bool isCongruentTo(const Shape&) const = 0;
    virtual bool isSimilarTo(const Shape&) const = 0;
    virtual bool containsPoint(Point) const = 0;

    virtual void rotate(Point, double) = 0;
    virtual void reflex(Point) = 0;
    virtual void reflex(Line) = 0;
    virtual void scale(Point, double) = 0;

    virtual ~Shape() = 0;
};

Shape::~Shape() {};

//----------------------------------------------------------------------------------------



class Ellipse :public Shape {
public:
    Point f1;
    Point f2;
    double big_line;
    Ellipse() = default;
    Ellipse(const Point&, const Point&, double);

    std::pair<Point, Point> focuses() const;
    std::pair<Line, Line> directrices() const;
    double eccentricity() const;
    Point center() const;

    double perimeter() const;
    double area() const;

    bool operator==(const Shape&) const;
    bool operator!=(const Shape&) const;
    bool isCongruentTo(const Shape&) const;
    bool isSimilarTo(const Shape&) const;
    bool containsPoint(Point) const;

    void rotate(Point, double);
    void reflex(Point);
    void reflex(Line);
    void scale(Point, double);

    ~Ellipse() = default;
};

//---------------------------------------------Конструктор---------------------------------------------
Ellipse::Ellipse(const Point& p1, const Point& p2, double r) {
    f1.x = p1.x;
    f1.y = p1.y;
    f2.x = p2.x;
    f2.y = p2.y;
    this->big_line = r / 2;
}

//---------------------------------------------Функции эллипсиса---------------------------------------
std::pair<Point, Point> Ellipse::focuses() const {
    return std::pair<Point, Point>(f1, f2);
}
double Ellipse::eccentricity() const {
    double c = sqrt((f1.x - f2.x) * (f1.x - f2.x) + (f1.y - f2.y) * (f1.y - f2.y));
    return c / (2 * big_line);
}
std::pair<Line, Line> Ellipse::directrices() const {
    Line l1;
    Line l2;
    double e = this->eccentricity();
    l1.a = 1;
    l1.b = 0;
    l1.c = big_line / e;
    l2.a = 1;
    l2.b = 0;
    l2.c = -big_line / e;
    return std::pair<Line, Line>(l1, l2);
}
Point Ellipse::center() const {
    Point s;
    s.x = (f1.x + f2.x) / 2;
    s.y = (f1.y + f2.y) / 2;
    return s;
}

//----------------------------------------------Перегрузка функций Shape-------------------------------
double Ellipse::perimeter() const {
    double b_small = sqrt(big_line * big_line * (1 - eccentricity() * eccentricity()));
    return (M_PI * (3 * (big_line + b_small) - sqrt((3 * big_line + b_small) * (big_line + 3 * b_small))));
}
double Ellipse::area() const {
    double b_small = sqrt(big_line * big_line * (1 - eccentricity() * eccentricity()));
    return (M_PI * big_line * b_small);
}
bool Ellipse::operator==(const Shape& s) const {
    if (dynamic_cast<const Ellipse*>(&s) == nullptr) return false;
    const Ellipse copy = dynamic_cast<const Ellipse&>(s);
    double copy_big_line = copy.big_line;
    if (is_peer(copy.f1.x, f1.x) && is_peer(copy.f2.x, f2.x) && is_peer(copy.f1.y, f1.y) && is_peer(copy.f2.y, f2.y) && is_peer(copy_big_line, big_line)) {
        return true;
    }
    else return false;
}
bool Ellipse::operator!=(const Shape& s) const {
    if (dynamic_cast<const Ellipse*>(&s) == nullptr) return false;
    const Ellipse copy = dynamic_cast<const Ellipse&>(s);
    if (*this == copy) return false;
    else return true;
}
bool Ellipse::isCongruentTo(const Shape& sh) const {
    if (dynamic_cast<const Ellipse*>(&sh) == nullptr) return false;
    const Ellipse copy = dynamic_cast<const Ellipse&>(sh);
    if (is_peer(eccentricity(), copy.eccentricity()) &&
        is_peer(big_line * eccentricity(), copy.big_line * copy.eccentricity())) return true;
    else return false;
}
bool Ellipse::isSimilarTo(const Shape& sh) const {
    if (dynamic_cast<const Ellipse*>(&sh) == nullptr) return false;
    const Ellipse copy = dynamic_cast<const Ellipse&>(sh);
    if (is_peer(eccentricity(), copy.eccentricity())) return true;
    else return false;
}
bool Ellipse::containsPoint(Point p) const {
    double dist_1 = sqrt((p.x - f1.x) * (p.x - f1.x) + (p.y - f1.y) * (p.y - f1.y));
    double dist_2 = sqrt((p.x - f2.x) * (p.x - f2.x) + (p.y - f2.y) * (p.y - f2.y));
    if (dist_1 + dist_2 < big_line * 2 || is_peer(dist_1 + dist_2, big_line * 2)) return true;
    else return false;
}
void Ellipse::rotate(Point p, double angle) {
    angle = angle * M_PI / 180;
    double dist_1 = f1.x - p.x;
    double dist_2 = f1.y - p.y;
    f1.x = dist_1 * cos(angle) - dist_2 * sin(angle) + p.x;
    f1.y = dist_1 * sin(angle) + dist_2 * cos(angle) + p.y;
    dist_1 = f2.x - p.x;
    dist_2 = f2.y - p.y;
    f2.x = dist_1 * cos(angle) - dist_2 * sin(angle) + p.x;
    f2.y = dist_1 * sin(angle) + dist_2 * cos(angle) + p.y;
    if (f1.x > f2.x || (is_peer(f1.x, f2.x) && f1.y > f2.y)) std::swap(f1, f2);
}
void Ellipse::reflex(Point p) {
    double dist_1 = -(f1.x - p.x);
    double dist_2 = -(f1.y - p.y);
    f1.x = dist_1 * 2 + f1.x;
    f1.y = dist_2 * 2 + f1.y;
    dist_1 = -(f2.x - p.x);
    dist_2 = -(f2.y - p.y);
    f2.x = dist_1 * 2 + f2.x;
    f2.y = dist_2 * 2 + f2.y;
    if (f1.x > f2.x || (is_peer(f1.x, f2.x) && f1.y > f2.y)) std::swap(f1, f2);
}
void Ellipse::reflex(Line l) {
    double coef = (-l.c - l.b * f1.y - l.a * f1.x) / (l.a * l.a + l.b * l.b);
    f1.x = f1.x + 2 * coef * (l.a);
    f1.y = f1.y + 2 * coef * (l.b);
    coef = (-l.c - l.b * f2.y - l.a * f2.x) / (l.a * l.a + l.b * l.b);
    f2.x = f2.x + 2 * coef * (l.a);
    f2.y = f2.y + 2 * coef * (l.b);
    if (f1.x > f2.x || (is_peer(f1.x, f2.x) && f1.y > f2.y)) std::swap(f1, f2);
}
void Ellipse::scale(Point p, double coef) {
    double e = eccentricity();
    double dist_1 = f1.x - p.x;
    double dist_2 = f1.y - p.y;
    f1.x = dist_1 * coef + p.x;
    f1.y = dist_2 * coef + p.y;
    dist_1 = f2.x - p.x;
    dist_2 = f2.y - p.y;
    f2.x = dist_1 * coef + p.x;
    f2.y = dist_2 * coef + p.y;
    big_line = sqrt((f1.x - f2.x) * (f1.x - f2.x) + (f1.y - f2.y) * (f1.y - f2.y)) / (2 * e);
    if (f1.x > f2.x || (is_peer(f1.x, f2.x) && f1.y > f2.y)) std::swap(f1, f2);
}

//------------------------------------------------------------------------------------------------------



class Circle :public Ellipse {
public:
    Circle() = default;
    Circle(const Point&, double);

    double radius() const;

    ~Circle() = default;
};

//-----------------------------------------------------Конструктор---------------------------------------
Circle::Circle(const Point& p, double r) {
    f1 = p;
    f2 = p;
    this->big_line = r;
}

//-----------------------------------------------------Функции-------------------------------------------
double Circle::radius() const {
    return big_line;
}

//-------------------------------------------------------------------------------------------------------



class Polygon :public Shape {
protected:
    std::vector<Point> points;
public:
    Polygon() = default;
    Polygon(const std::vector<Point>&);
    Polygon(std::initializer_list<Point>);

    int verticesCount() const;
    std::vector<Point> getVertices() const;
    bool isConvex() const;

    double perimeter() const;
    double area() const;

    bool operator==(const Shape&) const;
    bool operator!=(const Shape&) const;
    bool isCongruentTo(const Shape&) const;
    bool isSimilarTo(const Shape&) const;
    bool containsPoint(Point) const;

    void rotate(Point, double);
    void reflex(Point);
    void reflex(Line);
    void scale(Point, double);

    ~Polygon() = default;
};
//------------------------------------Конструкторы-----------------------------------------------
Polygon::Polygon(const std::vector<Point>& v) {
    for (int i = 0; i < static_cast<int>(v.size()); ++i) {
        points.push_back(v[i]);
    }
}
Polygon::Polygon(std::initializer_list<Point> s) {
    for (auto p : s) {
        points.push_back(p);
    }
}

//-------------------------------------Прочие----------------------------------------------------
int Polygon::verticesCount() const {
    return points.size();
}
std::vector<Point> Polygon::getVertices() const {
    return points;
}
bool Polygon::isConvex() const {
    //прохожусь по всем сторонам и смотрю, как они поворачиваются
    bool znak = 0;
    Point a = points[0];
    Point b = points[1];
    Point c = points[2];
    double povorot = (b.x - a.x) * (c.y - b.y) - (b.y - a.y) * (c.x - b.x);
    if (povorot > 0.0) znak = 1;
    for (int i = 0; i < static_cast<int> (points.size()); ++i) {
        bool znak_current = 0;
        if (i == static_cast<int>(points.size()) - 1) {
            Point a = points[i];
            Point b = points[0];
            Point c = points[1];
            double pov_current = (b.x - a.x) * (c.y - b.y) - (b.y - a.y) * (c.x - b.x);
            if (pov_current > 0) znak_current = 1;
        }
        else if (i == static_cast<int>(points.size()) - 2) {
            Point a = points[i];
            Point b = points[i + 1];
            Point c = points[0];
            double pov_current = (b.x - a.x) * (c.y - b.y) - (b.y - a.y) * (c.x - b.x);
            if (pov_current > 0) znak_current = 1;
        }
        else {
            Point a = points[i];
            Point b = points[i + 1];
            Point c = points[i + 2];
            double pov_current = (b.x - a.x) * (c.y - b.y) - (b.y - a.y) * (c.x - b.x);
            if (pov_current > 0) znak_current = 1;
        }
        if (znak != znak_current) return false;
    }
    return true;
}

//-------------------------------------Перегрузка наследуемых функций----------------------------
double Polygon::perimeter() const {
    double sum = 0;
    for (int i = 0; i < static_cast<int>(points.size()); ++i) {
        if (i == static_cast<int>(points.size()) - 1) {
            Point a = points[i];
            Point b = points[0];
            sum += sqrt((b.x - a.x) * (b.x - a.x) + (b.y - a.y) * (b.y - a.y));
        }
        else {
            Point a = points[i];
            Point b = points[i + 1];
            sum += sqrt((b.x - a.x) * (b.x - a.x) + (b.y - a.y) * (b.y - a.y));
        }
    }
    return sum;
}
double Polygon::area() const {
    double sum = 0;
    for (int i = 0; i < static_cast<int>(points.size()); ++i) {
        if (i == static_cast<int>(points.size()) - 1) sum += points[i].x * points[0].y;
        else sum += points[i].x * points[i + 1].y;
    }
    for (int i = 0; i < static_cast<int>(points.size()); ++i) {
        if (i == static_cast<int>(points.size()) - 1) sum -= points[i].y * points[0].x;
        else sum -= points[i].y * points[i + 1].x;
    }
    sum = abs(sum) / 2;
    return sum;
}
bool Polygon::operator==(const Shape& sh) const {
    //проверяю по точкам со сдвигом и обратным порядком, если нужно
    if (dynamic_cast<const Polygon*>(&sh) == nullptr) return false;
    const Polygon pol = dynamic_cast<const Polygon&>(sh);
    Point a = points[0];
    if (std::find(pol.points.begin(), pol.points.end(), a) == pol.points.end() || points.size() != pol.points.size()) return false;
    else {
        auto f_e = std::find(pol.points.begin(), pol.points.end(), a);
        auto f_e_copy = f_e;
        f_e_copy++;
        bool route = 0;
        if (f_e == pol.points.begin()) {
            auto prev = pol.points.end();
            auto next = f_e;
            prev--;
            next++;
            if (*prev == points[1]) {
                route = 1;
            }
            else if (*next == points[1]) {
                route = 0;
            }
            else {
                return false;
            }
        }
        else if (f_e_copy == pol.points.end()) {
            auto prev = f_e;
            auto next = pol.points.begin();
            prev--;
            if (*prev == points[1]) {
                route = 1;
            }
            else if (*next == points[1]) {
                route = 0;
            }
            else {
                return false;
            }
        }
        else {
            auto prev = f_e;
            auto next = f_e;
            prev--;
            next++;
            if (*prev == points[1]) {
                route = 1;
            }
            else if (*next == points[1]) {
                route = 0;
            }
            else {
                return false;
            }
        }
        if (route == 0) {
            for (int i = 0; i < static_cast<int>(points.size()); ++i) {
                if (f_e == pol.points.end()) f_e = pol.points.begin();
                if (*f_e != points[i]) return false;
                f_e++;
            }
        }
        else if (route == 1) {
            for (int i = 0; i < static_cast<int>(points.size()); ++i) {
                if (*f_e != points[i]) return false;
                if (f_e == pol.points.begin()) f_e = pol.points.end();
                f_e--;
            }
        }
        return true;
    }
}
bool Polygon::operator!=(const Shape& sh) const {
    if (dynamic_cast<const Polygon*>(&sh) == nullptr) return false;
    const Polygon pol = dynamic_cast<const Polygon&>(sh);
    if (*this == pol) return false;
    else return true;
}
bool Polygon::isCongruentTo(const Shape& sh) const {
    // создаю массивы длин сторон и проверяю их совпадение со сдвигом или обратным порядком
    if (dynamic_cast<const Polygon*>(&sh) == nullptr) return false;
    const Polygon copy = dynamic_cast<const Polygon&>(sh);
    if (points.size() != copy.points.size()) return false;
    std::vector<double> lines_1;
    std::vector<double> lines_2;
    for (int i = 0; i < static_cast<int>(points.size()); ++i) {
        if (i == static_cast<int>(points.size()) - 1) {
            lines_1.push_back(sqrt((points[i].x - points[0].x) * (points[i].x - points[0].x)
                + (points[i].y - points[0].y) * (points[i].y - points[0].y)));
        }
        else {
            lines_1.push_back(sqrt((points[i].x - points[i + 1].x) * (points[i].x - points[i + 1].x)
                + (points[i].y - points[i + 1].y) * (points[i].y - points[i + 1].y)));
        }
    }
    for (int i = 0; i < static_cast<int>(points.size()); ++i) {
        if (i == static_cast<int>(points.size()) - 1) {
            lines_2.push_back(sqrt((copy.points[i].x - copy.points[0].x) * (copy.points[i].x - copy.points[0].x)
                + (copy.points[i].y - copy.points[0].y) * (copy.points[i].y - copy.points[0].y)));
        }
        else {
            lines_2.push_back(sqrt((copy.points[i].x - copy.points[i + 1].x) * (copy.points[i].x - copy.points[i + 1].x)
                + (copy.points[i].y - copy.points[i + 1].y) * (copy.points[i].y - copy.points[i + 1].y)));
        }
    }
    std::vector<int>first_peer;
    for (int i = 0; i < static_cast<int>(points.size()); ++i) {
        if (is_peer(lines_2[i], lines_1[0])) first_peer.push_back(i);
    }

    for (int i = 0; i < static_cast<int>(first_peer.size()); ++i) {
        bool b = 1;
        for (int j = 0; j < static_cast<int>(points.size()); ++j) {
            if (!is_peer(lines_2[(j + first_peer[i]) % static_cast<int>(points.size())], lines_1[j])) b = 0;
        }
        b = 1;
        for (int j = static_cast<int>(points.size()) - 1; j >= 0; --j) {
            if (!is_peer(lines_2[(first_peer[i] - j + static_cast<int>(points.size())) % static_cast<int>(points.size())], lines_1[j])) b = 0;
        }
        if (b == 1) return true;
    }
    return false;
}
bool Polygon::isSimilarTo(const Shape& sh) const {
    //сравниваю отношения площадей и периметров
    if (dynamic_cast<const Polygon*>(&sh) == nullptr) return false;
    const Polygon copy = dynamic_cast<const Polygon&>(sh);
    if (points.size() != copy.points.size()) return false;
    double k = copy.perimeter() / perimeter();
    double k_proverka = copy.area() / area();
    if (is_peer(k * k, k_proverka)) return true;
    else return false;
}
bool Polygon::containsPoint(Point p) const {
    double sum = 0;
    for (int i = 0; i < static_cast<int>(points.size()); ++i) {
        if (i == static_cast<int>(points.size()) - 1) {
            Polygon pol{ p,points[i],points[0] };
            sum += pol.area();
        }
        else {
            Polygon pol{ p,points[i],points[i + 1] };
            sum += pol.area();
        }
    }
    if (sum > this->area() || !is_peer(sum, (this->area()))) return false;
    else return true;
}

//-------------------------------------Повороты фигуры---------------------------------------------
void Polygon::rotate(Point p, double angle) {
    angle = angle * M_PI / 180;
    for (int i = 0; i < static_cast<int>(points.size()); ++i) {
        points[i].x -= p.x;
        points[i].y -= p.y;
        double x = points[i].x;
        points[i].x = points[i].x * cos(angle) - points[i].y * sin(angle);
        points[i].y = x * sin(angle) + points[i].y * cos(angle);
        points[i].x += p.x;
        points[i].y += p.y;
    }
}
void Polygon::reflex(Point p) {
    for (int i = 0; i < static_cast<int>(points.size()); ++i) {
        double dist_1 = -points[i].x + p.x;
        double dist_2 = -points[i].y + p.y;
        points[i].x = points[i].x + 2 * dist_1;
        points[i].y = points[i].y + 2 * dist_2;
    }
}
void Polygon::reflex(Line l) {
    for (int i = 0; i < static_cast<int>(points.size()); ++i) {
        double coef = (-l.c - l.b * points[i].y - l.a * points[i].x)
            / ((l.a) * (l.a) + (l.b) * (l.b));
        points[i].x = points[i].x + 2 * coef * (l.a);
        points[i].y = points[i].y + 2 * coef * (l.b);
    }
}
void Polygon::scale(Point center, double coef) {
    for (int i = 0; i < static_cast<int>(points.size()); ++i) {
        points[i].x = coef * (points[i].x - center.x) + center.x;
        points[i].y = coef * (points[i].y - center.y) + center.y;
    }
}

//--------------------------------------------------------------------------------------------------



class Rectangle : public Polygon {
public:
    Rectangle() = default;
    Rectangle(const Point&, const Point&, double k);

    Point center() const;
    std::pair<Line, Line> diagonals() const;

    ~Rectangle() = default;
};
//------------------------------Конструктор-----------------------------------------------------------
Rectangle::Rectangle(const Point& a, const Point& c, double k) {
    if (k > 1) k = 1 / k;
    double diag = sqrt((a.x - c.x) * (a.x - c.x) + (a.y - c.y) * (a.y - c.y));
    double long_line = sqrt(diag * diag / (1 + k * k));
    double short_line = long_line * k;
    Point center{ (a.x + c.x) / 2,(a.y + c.y) / 2 };
    if (a.y > c.y) {
        double sn_d = 2 * long_line / diag * short_line / diag;
        double cs_d = -(2 * (short_line / diag) * (short_line / diag) - 1);
        double sn_b = 2 * long_line / diag * short_line / diag;
        double cs_b = 2 * (short_line / diag) * (short_line / diag) - 1;
        Point d;
        d.x = a.x * cs_d - a.y * sn_d;
        d.y = a.x * sn_d + a.y * cs_d;
        Point b;
        b.x = a.x * cs_b - a.y * sn_b;
        b.y = a.x * sn_b + a.y * cs_b;
        points.push_back(a);
        points.push_back(b);
        points.push_back(c);
        points.push_back(d);
    }
    else {
        double sn_b = 2 * long_line / diag * short_line / diag;
        double cs_b = -(2 * (long_line / diag) * (long_line / diag) - 1);
        double sn_d = 2 * long_line / diag * short_line / diag;
        double cs_d = 2 * (long_line / diag) * (long_line / diag) - 1;
        Point d;
        d.x = c.x * cs_d - c.y * sn_d;
        d.y = c.x * sn_d + c.y * cs_d;
        Point b;
        b.x = c.x * cs_b - c.y * sn_b;
        b.y = c.x * sn_b + c.y * cs_b;
        points.push_back(a);
        points.push_back(b);
        points.push_back(c);
        points.push_back(d);
    }
}

//------------------------------Функции---------------------------------------------------------------
Point Rectangle::center() const {
    Point center;
    center.x = (points[0].x + points[2].x) / 2;
    center.y = (points[0].y + points[2].y) / 2;
    return center;
}
std::pair<Line, Line> Rectangle::diagonals() const {
    Line diag_1{ points[0],points[2] };
    Line diag_2{ points[1],points[3] };
    return std::pair<Line, Line>(diag_1, diag_2);
}

//----------------------------------------------------------------------------------------------------



class Square :public Rectangle {
public:
    Square() = default;
    Square(const Point&, const Point&);

    Circle circumscribedCircle() const;
    Circle inscribedCircle() const;

    ~Square() = default;
};
//------------------------------------------Конструктор-------------------------------------------------
Square::Square(const Point& a, const Point& c) {
    double k = 1;
    double diag = sqrt((a.x - c.x) * (a.x - c.x) + (a.y - c.y) * (a.y - c.y));
    double long_line = sqrt(diag * diag / (1 + k * k));
    double short_line = long_line * k;
    Point center{ (a.x + c.x) / 2,(a.y + c.y) / 2 };
    if (a.y > c.y) {
        double sn_d = 2 * long_line / diag * short_line / diag;
        double cs_d = -(2 * (short_line / diag) * (short_line / diag) - 1);
        double sn_b = 2 * long_line / diag * short_line / diag;
        double cs_b = 2 * (short_line / diag) * (short_line / diag) - 1;
        Point d;
        d.x = a.x * cs_d - a.y * sn_d;
        d.y = a.x * sn_d + a.y * cs_d;
        Point b;
        b.x = a.x * cs_b - a.y * sn_b;
        b.y = a.x * sn_b + a.y * cs_b;
        points.push_back(a);
        points.push_back(b);
        points.push_back(c);
        points.push_back(d);
    }
    else {
        double sn_b = 2 * long_line / diag * short_line / diag;
        double cs_b = -(2 * (long_line / diag) * (long_line / diag) - 1);
        double sn_d = 2 * long_line / diag * short_line / diag;
        double cs_d = 2 * (long_line / diag) * (long_line / diag) - 1;
        Point d;
        d.x = c.x * cs_d - c.y * sn_d;
        d.y = c.x * sn_d + c.y * cs_d;
        Point b;
        b.x = c.x * cs_b - c.y * sn_b;
        b.y = c.x * sn_b + c.y * cs_b;
        points.push_back(a);
        points.push_back(b);
        points.push_back(c);
        points.push_back(d);
    }
}

//------------------------------------------Функции-----------------------------------------------------
Circle Square::circumscribedCircle() const {
    double line = sqrt((points[1].x - points[0].x) * (points[1].x - points[0].x)
        + (points[1].y - points[0].y) * (points[1].y - points[0].y));
    double radius = line / sqrt(2);
    Point center;
    center.x = (points[0].x + points[2].x) / 2;
    center.y = (points[0].y + points[2].x) / 2;
    Circle around(center, radius);
    return around;
}
Circle Square::inscribedCircle() const {
    double line = sqrt((points[1].x - points[0].x) * (points[1].x - points[0].x)
        + (points[1].y - points[0].y) * (points[1].y - points[0].y));
    double radius = line / 2;
    Point center;
    center.x = (points[0].x + points[2].x) / 2;
    center.y = (points[0].y + points[2].x) / 2;
    Circle in(center, radius);
    return in;
}

//------------------------------------------------------------------------------------------------------



class Triangle :public Polygon {
public:
    Triangle() = default;
    Triangle(Point&, Point&, Point&);

    Circle circumscribedCircle() const;
    Circle inscribedCircle() const;
    Point centroid() const;
    Point orthocenter() const;
    Line EulerLine() const;
    Circle ninePointsCircle() const;

    ~Triangle() = default;
};
Triangle::Triangle(Point& a, Point& b, Point& c) {
    points.push_back(a);
    points.push_back(b);
    points.push_back(c);
}
//------------------------------------------------Функции-----------------------------------------------
Circle Triangle::circumscribedCircle() const {
    //формула с определителями трех матриц
    double a = sqrt((points[0].x - points[1].x) * (points[0].x - points[1].x)
        + (points[0].y - points[1].y) * (points[0].y - points[1].y));
    double b = sqrt((points[1].x - points[2].x) * (points[1].x - points[2].x)
        + (points[1].y - points[2].y) * (points[1].y - points[2].y));
    double c = sqrt((points[2].x - points[0].x) * (points[2].x - points[0].x)
        + (points[2].y - points[0].y) * (points[2].y - points[0].y));
    double radius = a * b * c / (4 * area());
    double x1 = points[0].x;
    double y1 = points[0].y;
    double x2 = points[1].x;
    double y2 = points[1].y;
    double x3 = points[2].x;
    double y3 = points[2].y;
    //чтобы было легче, записал все координаты в компактные переменные
    double first_determ = (x1 * x1 + y1 * y1) * (y2 - y3) - y1 * (x2 * x2 + y2 * y2 - x3 * x3 - y3 * y3)
        + (x2 * x2 + y2 * y2) * y3 - y2 * (x3 * x3 + y3 * y3);
    double second_determ = 2 * (x1 * (y2 - y3) - y1 * (x2 - x3) + x2 * y3 - x3 * y2);
    double third_determ = x1 * (x2 * x2 + y2 * y2 - x3 * x3 - y3 * y3) - (x1 * x1 + y1 * y1) * (x2 - x3)
        + x2 * (x3 * x3 + y3 * y3) - x3 * (x2 * x2 + y2 * y2);
    Point center;
    center.x = first_determ / second_determ;
    center.y = third_determ / second_determ;
    Circle out(center, radius);
    return out;
}
Circle Triangle::inscribedCircle() const {
    double radius = 2 * area() / perimeter();
    // чтобы было легче, вычислил в переменные все стороны
    double a = sqrt((points[0].x - points[1].x) * (points[0].x - points[1].x)
        + (points[0].y - points[1].y) * (points[0].y - points[1].y));
    double b = sqrt((points[1].x - points[2].x) * (points[1].x - points[2].x)
        + (points[1].y - points[2].y) * (points[1].y - points[2].y));
    double c = sqrt((points[2].x - points[0].x) * (points[2].x - points[0].x)
        + (points[2].y - points[0].y) * (points[2].y - points[0].y));
    Point center;
    center.x = (a * points[2].x + b * points[0].x + c * points[1].x) / (a + b + c);
    center.y = (a * points[2].y + b * points[0].y + c * points[1].y) / (a + b + c);
    Circle in(center, radius);
    return in;
}
Point Triangle::centroid() const {
    Point centroid;
    centroid.x = (points[0].x + points[1].x + points[2].x) / 3;
    centroid.y = (points[0].y + points[1].y + points[2].y) / 3;
    return centroid;
}
Point Triangle::orthocenter() const {
    //формула с определителями трех матриц
    double x1 = points[0].x;
    double y1 = points[0].y;
    double x2 = points[1].x;
    double y2 = points[1].y;
    double x3 = points[2].x;
    double y3 = points[2].y;
    //чтобы было легче, записал все координаты в компактные переменные
    double first_determ = y1 * (x3 * x1 + y2 * y2 - x1 * x2 - y3 * y3) - (x2 * x3 + y1 * y1) * (y2 - y3)
        + y2 * (x1 * x2 + y3 * y3) - y3 * (x3 * x1 + y2 * y2);
    double second_determ = (x1 * x1 + y2 * y3) * (x2 - x3) + (x2 * x2 * x3 + y3 * y1 * x3 - x2 * x3 * x3 - x2 * y1 * y2)
        - x1 * (x2 * x2 + y1 * y3 - x3 * x3 - y1 * y2);
    double third_determ = x1 * (y2 - y3) - y1 * (x2 - x3) + x2 * y3 - x3 * y2;
    Point orto;
    orto.x = first_determ / third_determ;
    orto.y = second_determ / third_determ;
    return orto;
}
Line Triangle::EulerLine() const {
    //прямая через ортоцентр и центр описанной окружности
    Circle o = circumscribedCircle();
    Point p1 = o.f1;
    Point p2 = orthocenter();
    Line euler(p1, p2);
    return euler;
}
Circle Triangle::ninePointsCircle() const {
    Circle o = circumscribedCircle();
    Point p1 = o.f1;
    Point p2 = orthocenter();
    Point center_of_line((points[0].x + points[1].x) / 2, (points[0].y + points[1].y) / 2);
    Point center;
    // центр этой окружности - середина отрезка с концами в ортоцентре и центре описанной окружности
    center.x = (p1.x + p2.x) / 2;
    center.y = (p1.y + p2.y) / 2;
    //ищу напрямую через расстояние между точками, лежащими на окружности
    double radius = sqrt((center.x - center_of_line.x) * (center.x - center_of_line.x)
        + (center.y - center_of_line.y) * (center.y - center_of_line.y));
    Circle ninepoints(center, radius);
    return ninepoints;
}

//-------------------------------------------------------------------------------------------------------

