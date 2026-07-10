#pragma once

#include <map>

#include "Entity.hpp"

using EntityVec = std::vector<std::shared_ptr<Entity>>;
using EntityMap = std::map<std::string, EntityVec>;

class EntityManager {
  EntityVec m_entities;
  EntityVec m_toAdd;
  EntityMap m_entityMap;
  size_t m_totalEntities = 0;

public:
  EntityManager() = default;

  void update() {
    for (auto e : m_toAdd) {
      m_entities.push_back(e);
      m_entityMap[e->tag()].push_back(e);
    }

    m_toAdd.clear();

    static auto isDead = [](const auto &e) { return !e->isAlive(); };

    std::erase_if(m_entities, isDead);

    for (auto &[tag, vec] : m_entityMap) {
      std::erase_if(vec, isDead);
    }
  }

  std::shared_ptr<Entity> addEntity(const std::string &tag) {
    auto e = std::shared_ptr<Entity>(new Entity(tag, m_totalEntities++));
    m_toAdd.push_back(e);
    return e;
  }

  EntityVec &getEntities() { return m_entities; }

  EntityVec &getEntities(const std::string &tag) { return m_entityMap[tag]; }
};
