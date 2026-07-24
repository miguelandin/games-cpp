#pragma once

#include <tuple>

#include "Components.hpp"

using ComponentTuple = std::tuple<CTransform, CShape, CCollision, CInput,
                                  CScore, CLifespan, CWeapon>;

class Entity {
  friend class EntityManager;
  ComponentTuple m_components;
  bool m_alive = true;
  std::string m_tag = "default";
  size_t m_id = 0;
  Entity(const std::string &tag, size_t id) : m_tag(tag), m_id(id) {}

public:
  size_t id() const { return m_id; }

  void destroy() { m_alive = false; }

  bool isAlive() const { return m_alive; }

  const std::string &tag() const { return m_tag; }

  template <typename T> T &get() { return std::get<T>(m_components); }

  template <typename T> const T &get() const {
    return std::get<T>(m_components);
  }

  template <typename T> bool has() const {
    return std::get<T>(m_components).exists;
  }

  template <typename T> void remove() { get<T>() = T(); }

  template <typename T, typename... TArgs> T &add(TArgs &&...mArgs) {
    auto &component = get<T>();
    component = T(std::forward<TArgs>(mArgs)...);
    component.exists = true;
    return component;
  }
};
