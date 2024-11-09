#pragma once

#include "core/collider.hpp"
#include <memory>

class CollisionAware {
  protected:
    std::unique_ptr<Collider> collider;

  public:
    CollisionAware() = default;

    void initializeCollider(Object *parent, glm::vec3 origin,
                            glm::vec3 dimensions,
                            std::vector<std::string> collideWith = {}) {
        this->collider =
            std::make_unique<Collider>(parent, origin, dimensions, collideWith);
    }

    virtual Collider *getCollider() { return this->collider.get(); }
};
