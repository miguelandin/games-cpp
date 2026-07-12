#pragma once

#include <cmath>
#include <string>

#include <SFML/Graphics.hpp>

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

  std::string toString() const {
    return "[x: " + std::to_string(x) + ", y: " + std::to_string(y) + "]";
  }

  T disSq(const Vec2 &rhs) const {
    T dx = rhs.x - x;
    T dy = rhs.y - y;
    return (dx * dx + dy * dy);
  }

  T dis(const Vec2 &rhs) const { return std::sqrt(disSq(rhs)); }

  float angle() const { return std::atan2f(y, x); }

  float angle(const Vec2 &rhs) const {
    T dx = rhs.x - x;
    T dy = rhs.y - y;
    return std::atan2f(dy, dx);
  }

  T lengthSq() const { return (x * x + y * y); }

  T length() const { return std::sqrt(lengthSq()); }

  Vec2 normalize() const {
    T len = length();
    if (len > 0) {
      return Vec2(x / len, y / len);
    }
    return Vec2();
  }

  static Vec2 normalize(float angle) {
    return Vec2(std::cos(angle), std::sin(angle));
  }
};

using Vec2i = Vec2<int>;
using Vec2f = Vec2<float>;
