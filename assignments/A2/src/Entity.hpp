#include "Components.hpp"
#include "EntityManager.hpp"
#include <tuple>

class EntityManager;

using ComponentTuple =
    std::tuple<CTransform, CShape, CCollision, CInput, CScore, CLifespan>;

class Entity{
    friend class EntityManager;

};
