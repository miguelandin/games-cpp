#pragma once
#include <SFML/Graphics.hpp>
#include <cmath>

template <typename T> class Vec2 {
public:
  T x = 0;
  T y = 0;

  Vec2() = default;
  Vec2(T xin, T yin) : x(xin), y(yin) {}

  template <typename U>
  Vec2(const sf::Vector2<U> &vec)
      : x(static_cast<T>(vec.x)), y(static_cast<T>(vec.y)) {}

  operator sf::Vector2<T>() const { return sf::Vector2<T>(x, y); }

  Vec2 operator+(const Vec2 &rhs) const { return Vec2(x + rhs.x, y + rhs.y); }

  Vec2 operator-(const Vec2 &rhs) const { return Vec2(x - rhs.x, y - rhs.y); }

  Vec2 operator/(const T val) const { return Vec2(x / val, y / val); }

  Vec2 operator*(const T val) const { return Vec2(x * val, y * val); }

  bool operator==(const Vec2 &rhs) const { return (x == rhs.x && y == rhs.y); }

  bool operator!=(const Vec2 &rhs) const { return (x != rhs.x || y != rhs.y); }

  Vec2 &operator+=(const Vec2 &rhs) {
    x += rhs.x;
    y += rhs.y;
    return *this;
  }

  Vec2 &operator-=(const Vec2 &rhs) {
    x -= rhs.x;
    y -= rhs.y;
    return *this;
  }

  Vec2 &operator*=(T val) {
    x *= val;
    y *= val;
    return *this;
  }

  Vec2 &operator/=(T val) {
    x /= val;
    y /= val;
    return *this;
  }

  T disSq(const Vec2 &rhs) const {
    T dx = rhs.x - x;
    T dy = rhs.y - y;
    return (dx * dx + dy * dy);
  }

  T dis(const Vec2 &rhs) const { return std::sqrt(disSq(rhs)); }

  T lengthSq() const { return (x * x + y * y); }

  T length() const { return std::sqrt(lengthSq()); }

  void normalize() {
    T len = length();
    if (len > 0) {
      *this /= len;
    }
  }
};

using Vec2i = Vec2<int>;
using Vec2f = Vec2<float>;
