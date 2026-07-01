#include "Entity.hpp"
#include <map>

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
    for (auto e : m_entities) {
      if (!e->isAlive()) {
        // TODO remove from entities & entitiyMap
      }
    }
    m_toAdd.clear();
  }

  std::shared_ptr<Entity> addEntity(const std::string &tag) {
    auto e = std::shared_ptr<Entity>(new Entity(tag, m_totalEntities++));
    m_toAdd.push_back(e);
    return e;
  }

  EntityVec &getEntities() { return m_entities; }

  EntityVec &getEntities(const std::string &tag) { return m_entityMap[tag]; }
};
