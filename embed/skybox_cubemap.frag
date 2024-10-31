#version 330 core

in vec3 v_position;

out vec4 f_color;

uniform sampler2D u_hdri;

const vec2 invAtan = vec2(0.1591, 0.3183);
vec2 sampleSphericalMap(vec3 v) {
    vec2 uv = vec2(atan(v.z, v.x), asin(v.y));
    uv *= invAtan;
    uv += 0.5;
    return uv;
}

void main() {
    vec2 uv = sampleSphericalMap(normalize(v_position));
    vec3 color = texture(u_hdri, uv).rgb;

    f_color = vec4(color, 1.0);
}