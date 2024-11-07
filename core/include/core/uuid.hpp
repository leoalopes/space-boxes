#pragma once
#include <random>
#include <sstream>

namespace UUID {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<> dis(0, 15);
    static std::uniform_int_distribution<> dis2(8, 11);

    inline std::string v4() {
        int i;
        std::stringstream stream;

        stream << std::hex;
        for (i = 0; i < 8; i++) {
            stream << dis(gen);
        }

        stream << "-";
        for (i = 0; i < 4; i++) {
            stream << dis(gen);
        }

        stream << "-4";
        for (i = 0; i < 3; i++) {
            stream << dis(gen);
        }

        stream << "-";
        stream << dis2(gen);
        for (i = 0; i < 3; i++) {
            stream << dis(gen);
        }

        stream << "-";
        for (i = 0; i < 12; i++) {
            stream << dis(gen);
        }

        return stream.str();
    }
}
