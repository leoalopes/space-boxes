#version 330 core

layout (location = 0) in vec3 in_position;
layout (location = 1) in vec2 in_tex_coord;

out vec3 dynamic_color;
out vec2 tex_coord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    gl_Position = projection * view * model * vec4(in_position, 1.0);
    dynamic_color = in_position + 0.5;
    tex_coord = in_tex_coord;
}
