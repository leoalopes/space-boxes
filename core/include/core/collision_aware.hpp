#pragma once

#include "core/collider.hpp"
#include <memory>

class CollisionAware {
  protected:
    std::unique_ptr<Collider> collider;

  public:
    CollisionAware(Object *parent, glm::vec3 origin, glm::vec3 dimensions) {
        this->collider = std::make_unique<Collider>(parent, origin, dimensions);
    }

    Collider *getCollider() { return this->collider.get(); }
};
