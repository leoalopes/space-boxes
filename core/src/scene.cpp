#include "core/scene.hpp"
#include "core/object.hpp"

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

void Scene::draw() {
    if (this->camera == nullptr) {
        return;
    }

    glm::mat4 viewMatrix = this->camera->getLookAt();
    for (int i = 0; i < this->objects.size(); i++) {
        if (this->objects.at(i) != nullptr) {
            this->objects.at(i)->update();
            this->objects.at(i)->setProjectionMatrix(this->matrix);
            this->objects.at(i)->setViewMatrix(viewMatrix);
            this->objects.at(i)->draw();
        }
    }

    this->setMouseDelta(glm::vec2({0, 0}));
}
