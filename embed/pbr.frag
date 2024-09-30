#version 460 core

in vec3 v_view_position;
in vec3 v_view_normal;
in vec2 v_texcoord;

out vec4 f_color;

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

uniform vec3 u_light_direction;
uniform vec3 u_light_color;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

// https://learnopengl.com/PBR/Lighting

const float PI = 3.14159265359;

vec3 fresnelSchlick(float cosTheta, vec3 F0) {
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

float DistributionGGX(vec3 N, vec3 H, float roughness) {
    float a = roughness * roughness;
    float a2 = a * a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH * NdotH;

    float num = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return num / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness) {
    float r = (roughness + 1.0);
    float k = (r * r) / 8.0;

    float num = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return num / denom;
}
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness) {
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}

void main() {
    vec3 normal = normalize(v_view_normal);
    vec3 view_direction = normalize(-v_view_position);

    vec3 albedo = u_albedo;
    float alpha = u_alpha;
    float roughness = u_roughness;
    float metallic = u_metallic;
    float ao = u_ao;

    if(u_use_albedo_map) {
        albedo = texture(u_albedo_map, v_texcoord).rgb;
    }
    if(u_use_alpha_map) {
        alpha = texture(u_alpha_map, v_texcoord).r;
    }
    if(u_use_roughness_map) {
        roughness = texture(u_roughness_map, v_texcoord).r;
    }
    if(u_use_metallic_map) {
        metallic = texture(u_metallic_map, v_texcoord).r;
    }
    if(u_use_ao_map) {
        ao = texture(u_ao_map, v_texcoord).r;
    }
    if(u_use_normal_map) {
        vec3 normal_map = texture(u_normal_map, v_texcoord).rgb;
        normal_map = normalize(normal_map * 2.0 - 1.0);
        // TODO: Implement tangent space normal mapping
    }

    vec3 F0 = vec3(0.04);
    F0 = mix(F0, albedo, metallic);

    vec3 Lo = vec3(0.0);
    {
        vec3 light_direction = mat3(u_view) * -u_light_direction;
        vec3 half_direction = normalize(light_direction + view_direction);
        float HdotV = max(dot(half_direction, view_direction), 0.0);
        float NDF = DistributionGGX(normal, half_direction, roughness);
        float G = GeometrySmith(normal, view_direction, light_direction, roughness);
        vec3 F = fresnelSchlick(HdotV, F0);

        vec3 kS = F;
        vec3 kD = vec3(1.0) - kS;
        kD *= 1.0 - metallic;

        vec3 numerator = NDF * G * F;
        float denominator = 4.0 * max(dot(normal, view_direction), 0.0) * max(dot(normal, light_direction), 0.0) + 0.0001;
        vec3 specular = numerator / denominator;

        float NdotL = max(dot(normal, light_direction), 0.0);
        Lo += (kD * albedo / PI + specular) * u_light_color * NdotL;
    }

    vec3 ambient = vec3(0.03) * albedo * ao;
    vec3 color = ambient + Lo;

    color = color / (color + vec3(1.0));
    color = pow(color, vec3(1.0 / 2.2));

    f_color = vec4(color, 1.0);
}
