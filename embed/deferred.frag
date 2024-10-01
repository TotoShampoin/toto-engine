#version 460 core

in vec3 v_view_position;
in vec3 v_view_normal;
in vec2 v_texcoord;

layout(location = 0) out vec4 f_position;
layout(location = 1) out vec4 f_normal;
layout(location = 2) out vec4 f_albedo;
layout(location = 3) out vec4 f_metallic_roughness_ao;

uniform vec3 u_albedo;
uniform float u_metallic;
uniform float u_roughness;
uniform float u_ao;
uniform float u_alpha;

uniform bool u_use_albedo_map;
uniform bool u_use_metallic_map;
uniform bool u_use_roughness_map;
uniform bool u_use_ao_map;
uniform bool u_use_alpha_map;
uniform bool u_use_normal_map;

uniform sampler2D u_albedo_map;
uniform sampler2D u_metallic_map;
uniform sampler2D u_roughness_map;
uniform sampler2D u_ao_map;
uniform sampler2D u_alpha_map;
uniform sampler2D u_normal_map;

void main() {
    f_position = vec4(v_view_position, 1.0);
    f_normal = vec4(v_view_normal, 1.0);
    f_albedo = vec4(u_albedo, 1.0);
    f_metallic_roughness_ao = vec4(u_metallic, u_roughness, u_ao, u_alpha);

    if(u_use_albedo_map) {
        f_albedo = texture(u_albedo_map, v_texcoord);
    }
    if(u_use_metallic_map) {
        f_metallic_roughness_ao.x = texture(u_metallic_map, v_texcoord).r;
    }
    if(u_use_roughness_map) {
        f_metallic_roughness_ao.y = texture(u_roughness_map, v_texcoord).r;
    }
    if(u_use_ao_map) {
        f_metallic_roughness_ao.z = texture(u_ao_map, v_texcoord).r;
    }
    if(u_use_alpha_map) {
        f_metallic_roughness_ao.w = texture(u_alpha_map, v_texcoord).r;
    }
    if(u_use_normal_map) {
        // TODO: Implement tangent space normal mapping
    }
}
