#include "core/collider.hpp"

/*bool Collider::isColliding(Object *root, Object *target,*/
/*                           Collider *targetCollider) {*/
/*    if (target == nullptr) {*/
/*        return false;*/
/*    }*/
/**/
/*    std::array<glm::vec3, 2> rootBB = this->getBoundingBox();*/
/*    std::array<glm::vec3, 2> targetBB = targetCollider->getBoundingBox();*/
/**/
/*    std::cout << "--- Comparing ---" << '\n';*/
/*    std::cout << "x: " << rootBB[0].x << " -> " << rootBB[1].x*/
/*              << " to x: " << targetBB[0].x << " -> " << targetBB[1].x <<
 * '\n';*/
/*    std::cout << "y: " << rootBB[0].y << " -> " << rootBB[1].y*/
/*              << " to y: " << targetBB[0].y << " -> " << targetBB[1].y <<
 * '\n';*/
/*    std::cout << "z: " << rootBB[0].z << " -> " << rootBB[1].z*/
/*              << " to z: " << targetBB[0].z << " -> " << targetBB[1].z <<
 * '\n';*/
/**/
/*    bool xAxis = rootBB[1].x >= targetBB[0].x && targetBB[1].x >=
 * rootBB[0].x;*/
/*    bool yAxis = rootBB[1].y >= targetBB[0].y && targetBB[1].y >=
 * rootBB[0].y;*/
/*    bool zAxis = rootBB[1].z >= targetBB[0].z && targetBB[1].z >=
 * rootBB[0].z;*/
/**/
/*    return xAxis && yAxis && zAxis;*/
/*}*/

/*
bool Collider::isColliding(Object *root, Object *target,
                           Collider *targetCollider) {
    if (root == nullptr || target == nullptr || targetCollider == nullptr) {
        return false;
    }

    glm::vec3 targetOrigin = targetCollider->getOrigin();
    glm::vec3 targetDimensions = targetCollider->getDimensions();

    glm::vec3 location = root->getTransform()->getLocation();
    glm::vec3 targetLocation = target->getTransform()->getLocation();
    glm::vec3 scale = root->getTransform()->getScale();
    glm::vec3 targetScale = target->getTransform()->getScale();

    glm::vec3 positionedOrigin{location.x + this->origin.x,
                               location.y + this->origin.y,
                               location.z + this->origin.z};
    glm::vec3 targetPositionedOrigin{targetLocation.x + targetOrigin.x,
                                     targetLocation.y + targetOrigin.y,
                                     targetLocation.z + targetOrigin.z};

    glm::vec3 scaledDimensions{this->dimensions.x * scale.x,
                               this->dimensions.y * scale.y,
                               this->dimensions.z * scale.z};
    glm::vec3 targetScaledDimensions{targetDimensions.x * targetScale.x,
                                     targetDimensions.y * targetScale.y,
                                     targetDimensions.z * targetScale.z};

    float xSize = positionedOrigin.x + scaledDimensions.x;
    float ySize = positionedOrigin.y + scaledDimensions.y;
    float zSize = positionedOrigin.z + scaledDimensions.z;

    float targetXSize = targetPositionedOrigin.x + targetScaledDimensions.x;
    float targetYSize = targetPositionedOrigin.y + targetScaledDimensions.y;
    float targetZSize = targetPositionedOrigin.z + targetScaledDimensions.z;

    bool xAxis =
        xSize >= targetPositionedOrigin.x && targetXSize >= positionedOrigin.x;
    bool yAxis =
        ySize >= targetPositionedOrigin.y && targetYSize >= positionedOrigin.y;
    bool zAxis =
        zSize >= targetPositionedOrigin.z && targetZSize >= positionedOrigin.z;

    return xAxis && yAxis && zAxis;
}
*/
