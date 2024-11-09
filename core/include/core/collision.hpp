#pragma once
#include "core/object.hpp"

class Object;

class Collision {
  public:
    Object *target;
    glm::vec3 direction;

    Collision(Object *target, glm::vec3 direction)
        : target(target), direction(direction) {}
};
