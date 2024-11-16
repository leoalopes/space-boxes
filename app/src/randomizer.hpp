#pragma once

#include "glm/geometric.hpp"
#include <glm/glm.hpp>
#include <random>

class Randomizer {
    std::mt19937 generator;
    std::uniform_real_distribution<> distribution;

  public:
    Randomizer() {
        std::random_device device;
        const std::mt19937 generator(device());
        this->generator = generator;

        const std::uniform_real_distribution<> distribution(0.0f, 1.0f);
        this->distribution = distribution;
    }

    float randomize() { return this->distribution(this->generator); }

    bool getRandomBool(int odds) {
        return this->randomize() > (100.0f - odds) / 100;
    }

    float getRandomBetween(float min, float max) {
        return min + (max - min) * randomize();
    }

    glm::vec3 getRandomDirection() {
        glm::vec3 direction{this->randomize() * 2 - 1, 0.0f,
                            this->randomize() * 2 - 1};
        return glm::normalize(direction);
    }
};
