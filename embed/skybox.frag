#version 330 core
out vec4 f_color;

in vec3 v_position;

uniform samplerCube u_cubemap;

void main() {
    vec3 env_color = texture(u_cubemap, v_position).rgb;

    env_color = env_color / (env_color + vec3(1.0));
    env_color = pow(env_color, vec3(1.0 / 2.2));

    f_color = vec4(env_color, 1.0);
}