#include "core/scene.hpp"
#include "core/collision.hpp"
#include "core/collision_aware.hpp"
#include "core/object.hpp"
#include <map>
#include <vector>

Scene::Scene(unsigned int width, unsigned int height)
    : matrix(1.0f), aspectRatio((float)width / (float)height),
      mouseDelta(0.0f) {}

void Scene::addObject(Object *object) {
    this->objects.push_back(object);
    object->setScene(this);
}

void Scene::setCamera(Camera *camera) {
    this->camera = camera;
    this->matrix = glm::perspective(glm::radians(this->camera->getFov()),
                                    this->aspectRatio, 0.1f, 100.0f);
}

/*bool Scene::detectCollisions(Object *root, Object *target) {*/
/*    if (root == nullptr || target == nullptr ||*/
/*        root->getCollider() == nullptr || target->getCollider() == nullptr)
 * {*/
/*        return false;*/
/*    }*/
/**/
/*    glm::vec3 targetOrigin = target->getOrigin();*/
/*    glm::vec3 targetDimensions = target->getDimensions();*/
/**/
/*    glm::vec3 location = this->getParent()->getTransform()->getLocation();*/
/*    glm::vec3 targetLocation =*/
/*        target->getParent()->getTransform()->getLocation();*/
/*    glm::vec3 scale = this->getParent()->getTransform()->getScale();*/
/*    glm::vec3 targetScale = target->getParent()->getTransform()->getScale();*/
/**/
/*    glm::vec3 positionedOrigin{location.x + this->origin.x,*/
/*                               location.y + this->origin.y,*/
/*                               location.z + this->origin.z};*/
/*    glm::vec3 targetPositionedOrigin{targetLocation.x + targetOrigin.x,*/
/*                                     targetLocation.y + targetOrigin.y,*/
/*                                     targetLocation.z + targetOrigin.z};*/
/**/
/*    glm::vec3 scaledDimensions{this->dimensions.x * scale.x,*/
/*                               this->dimensions.y * scale.y,*/
/*                               this->dimensions.z * scale.z};*/
/*    glm::vec3 targetScaledDimensions{targetDimensions.x * targetScale.x,*/
/*                                     targetDimensions.y * targetScale.y,*/
/*                                     targetDimensions.z * targetScale.z};*/
/**/
/*    float xSize = positionedOrigin.x + scaledDimensions.x;*/
/*    float ySize = positionedOrigin.y + scaledDimensions.y;*/
/*    float zSize = positionedOrigin.z + scaledDimensions.z;*/
/**/
/*    float targetXSize = targetPositionedOrigin.x + targetScaledDimensions.x;*/
/*    float targetYSize = targetPositionedOrigin.y + targetScaledDimensions.y;*/
/*    float targetZSize = targetPositionedOrigin.z + targetScaledDimensions.z;*/
/**/
/*    bool xAxis =*/
/*        xSize >= targetPositionedOrigin.x && targetXSize >=
 * positionedOrigin.x;*/
/*    bool yAxis =*/
/*        ySize >= targetPositionedOrigin.y && targetYSize >=
 * positionedOrigin.y;*/
/*    bool zAxis =*/
/*        zSize >= targetPositionedOrigin.z && targetZSize >=
 * positionedOrigin.z;*/
/**/
/*    return xAxis && yAxis && zAxis;*/
/*}*/

std::map<std::string, std::vector<Collision>> Scene::detectCollisions() {
    std::map<std::string, std::vector<Collision>> collisionMap{};
    for (int i = 0; i < this->objects.size(); i++) {
        Object *root = this->objects.at(i);
        if (root != nullptr) {
            if (!root->isCollisionAware()) {
                continue;
            }

            CollisionAware *collisionAwareRoot =
                dynamic_cast<CollisionAware *>(root);
            if (!collisionAwareRoot) {
                std::cout << "Failed to cast " << root->getName()
                          << " to CollisionAware" << '\n';
                continue;
            }

            Collider *rootCollider = collisionAwareRoot->getCollider();
            if (rootCollider == nullptr) {
                std::cout << "No collider found for " << root->getName()
                          << ", skipping" << '\n';
                continue;
            }

            for (int j = i + 1; j < this->objects.size(); j++) {
                Object *target = this->objects.at(j);
                if (target != nullptr) {
                    if (!target->isCollisionAware()) {
                        continue;
                    }

                    CollisionAware *collisionAwareTarget =
                        dynamic_cast<CollisionAware *>(target);
                    if (!collisionAwareTarget) {
                        std::cout << "Failed to cast " << target->getName()
                                  << " to CollisionAware" << '\n';
                        continue;
                    }

                    Collider *targetCollider =
                        collisionAwareTarget->getCollider();
                    if (!targetCollider || targetCollider == nullptr) {
                        std::cout << "No collider found for "
                                  << target->getName() << ", skipping" << '\n';
                        continue;
                    }

                    glm::vec3 direction{0.0f};
                    bool isColliding = rootCollider->checkCollision(
                        targetCollider, &direction);
                    if (isColliding) {
                        Collision rootCollision{target, direction};
                        Collision targetCollision{root, -direction};

                        bool alreadyAddedRoot =
                            collisionMap.contains(root->getUUID());
                        if (alreadyAddedRoot) {
                            collisionMap.at(root->getUUID())
                                .push_back(rootCollision);
                        } else {
                            collisionMap[root->getUUID()] = {rootCollision};
                        }

                        bool alreadyAddedTarget =
                            collisionMap.contains(target->getUUID());
                        if (alreadyAddedTarget) {
                            collisionMap.at(target->getUUID())
                                .push_back(targetCollision);
                        } else {
                            collisionMap[target->getUUID()] = {targetCollision};
                        }
                    }
                }
            }
        }
    }

    return collisionMap;
}

void Scene::draw() {
    if (this->camera == nullptr) {
        return;
    }

    auto collisionMap = this->detectCollisions();

    glm::mat4 viewMatrix = this->camera->getLookAt();
    for (int i = 0; i < this->objects.size(); i++) {
        if (this->objects.at(i) != nullptr) {
            this->objects.at(i)->setProjectionMatrix(this->matrix);
            this->objects.at(i)->setViewMatrix(viewMatrix);
            if (collisionMap.contains(this->objects.at(i)->getUUID())) {
                this->objects.at(i)->update(
                    collisionMap.at(this->objects.at(i)->getUUID()));
                this->objects.at(i)->draw(true);
            } else {
                this->objects.at(i)->update({});
                this->objects.at(i)->draw(false);
            }
        }
    }

    this->setMouseDelta({0, 0});
}
