#version 330 core

in vec3 dynamic_color;
in vec2 tex_coord;

out vec4 FragColor;

uniform sampler2D texture1;

void main() {
    FragColor = texture(texture1, tex_coord * 2.0f) * vec4(0.502f, 0.502f, 0.502f, 1.0f);
}
