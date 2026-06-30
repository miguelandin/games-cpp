#pragma once
#include <SFML/Graphics.hpp>
#include <math.h>

template <typename T> class Vec2 {
public:
  T x = 0;
  T y = 0;

  Vec2() = default;
  Vec2(T xin, T yin) : x(xin), y(yin) {}
  Vec2(const sf::Vector2<T> &vec) : x(vec.x), y(vec.y) {}
  Vec2(const sf::Vector2i &vec) : x(vec.x), y(vec.y) {}

  operator sf::Vector2<T>() { return sf::Vector2<T>(x, y); }
  Vec2 operator+(const Vec2 &rhs) const {
    // TODO
  }

  Vec2 operator-(const Vec2 &rhs) const {
    // TODO
  }

  Vec2 operator/(const T val) const {
    // TODO
  }

  Vec2 operator*(const T val) const {
    // TODO
  }

  bool operator==(const Vec2 &rhs) const {
    // TODO
  }

  bool operator!=(const Vec2 &rhs) const {
    // TODO
  }

  void operator+=(const Vec2 &rhs) {
    // TODO
  }
  void operator-=(const Vec2 &rhs) {
    // TODO
  }

  void operator*=(const Vec2 &rhs) {
    // TODO
  }

  void operator/=(const Vec2 &rhs) {
    // TODO
  }

  float dis(const Vec2 &rhs) {
    // TODO
  }

  float length() const {
    // TODO
  }

  void normalize() {
    // TODO
  }
};

using Vec2f = Vec2<float>;
