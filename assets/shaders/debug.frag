#version 330 core

out vec4 FragColor;

uniform int isColliding;

void main() {
    if (isColliding == 1) {
        FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);
    } else {
        FragColor = vec4(0.0f, 1.0f, 0.0f, 1.0f);
    }
}
