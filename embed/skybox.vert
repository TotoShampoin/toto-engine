#version 330 core
layout(location = 0) in vec3 a_position;

uniform mat4 u_projection;
uniform mat4 u_view;

out vec3 v_position;

void main() {
    v_position = a_position;

    mat4 rot_view = mat4(mat3(u_view)); // remove translation from the view matrix
    vec4 clip_pos = u_projection * rot_view * vec4(v_position, 1.0);

    gl_Position = clip_pos.xyww;
}
