#version 460 core

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec3 a_normal;
layout(location = 2) in vec2 a_texcoord;

out vec3 v_view_position;
out vec3 v_view_normal;
out vec2 v_texcoord;

void main() {
    v_view_position = a_position;
    v_view_normal = a_normal;
    v_texcoord = a_texcoord;

    gl_Position = vec4(a_position, 1.0);
}
