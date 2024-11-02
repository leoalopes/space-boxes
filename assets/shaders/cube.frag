#version 330 core

in vec3 dynamic_color;
in vec2 tex_coord;

out vec4 FragColor;

uniform sampler2D texture1;

void main() {
    FragColor = texture(texture1, tex_coord) * vec4(0.27f, 0.33f, 0.35f, 1.0f);
}
