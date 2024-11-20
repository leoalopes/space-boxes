#version 330 core

in vec3 dynamic_color;
in vec2 tex_coord;

out vec4 FragColor;

uniform sampler2D texture1;

void main() {
    FragColor = texture(texture1, tex_coord * 2.5f) * vec4(0.2, 0.2, 0.2, 1.0f);
}
