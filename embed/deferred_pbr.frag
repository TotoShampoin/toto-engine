#version 460 core

in vec3 v_view_position;
in vec3 v_view_normal;
in vec2 v_texcoord;

uniform sampler2D u_position;
uniform sampler2D u_normal;
uniform sampler2D u_albedo;
uniform sampler2D u_metallic_roughness_ao;

uniform vec3 u_light_direction;
uniform float u_light_intensity;
uniform vec3 u_light_color;

uniform samplerCube u_irradiance_map;
uniform samplerCube u_prefiltered_map;
uniform sampler2D u_brdf_lut;
uniform float u_exposure;

uniform mat4 u_view;

out vec4 f_color;

// https://learnopengl.com/PBR/Lighting

const float PI = 3.14159265359;

vec3 fresnelSchlick(float cosTheta, vec3 F0) {
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}
vec3 fresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness) {
    return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
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
    vec4 position = texture(u_position, v_texcoord);
    vec3 view_normal = normalize(texture(u_normal, v_texcoord).xyz);
    vec3 albedo = texture(u_albedo, v_texcoord).rgb;
    vec4 metallic_roughness_ao = texture(u_metallic_roughness_ao, v_texcoord);

    float metallic = metallic_roughness_ao.r;
    float roughness = metallic_roughness_ao.g;
    float ao = metallic_roughness_ao.b;
    float alpha = metallic_roughness_ao.a;

    vec3 view_direction = normalize(-position.xyz);

    mat3 inv_view = transpose(mat3(u_view));
    vec3 world_normal = inv_view * view_normal;

    vec3 world_direction = inv_view * view_direction;
    float NdotV_view = max(dot(view_normal, view_direction), 0.0);
    float NdotV_world = max(dot(world_normal, view_direction), 0.0);

    vec3 F0 = vec3(0.04);
    F0 = mix(F0, albedo, metallic);

    vec3 light_out = vec3(0.0);
    {
        vec3 light_direction = mat3(u_view) * -u_light_direction;
        vec3 half_direction = normalize(light_direction + view_direction);
        float HdotV = max(dot(half_direction, view_direction), 0.0);
        float NDF = DistributionGGX(view_normal, half_direction, roughness);
        float G = GeometrySmith(view_normal, view_direction, light_direction, roughness);
        vec3 F = fresnelSchlick(HdotV, F0);

        vec3 kS = F;
        vec3 kD = vec3(1.0) - kS;
        kD *= 1.0 - metallic;

        vec3 numerator = NDF * G * F;
        float denominator = 4.0 * max(dot(view_normal, view_direction), 0.0) * max(dot(view_normal, light_direction), 0.0) + 0.0001;
        vec3 specular = numerator / denominator;

        float NdotL = max(dot(view_normal, light_direction), 0.0);
        light_out += (kD * albedo / PI + specular) * u_light_color * NdotL;
    }

    vec3 ambient;
    {
        vec3 kS = fresnelSchlickRoughness(NdotV_view, F0, roughness);
        vec3 kD = 1.0 - kS;
        kD *= 1.0 - metallic;
        vec3 irradiance = texture(u_irradiance_map, world_normal).rgb;
        vec3 diffuse = irradiance * albedo;

        vec3 R = reflect(-world_direction, world_normal);
        const float MAX_REFLECTION_LOD = 4.0;
        vec3 prefiltered_color = textureLod(u_prefiltered_map, R, roughness * MAX_REFLECTION_LOD).rgb;
        vec2 env_brdf = texture(u_brdf_lut, vec2(NdotV_view, roughness)).rg;
        vec3 specular = prefiltered_color * (kS * env_brdf.x + env_brdf.y);
        // vec3 specular = texture(u_prefiltered_map, R, roughness).rgb;

        ambient = (kD * diffuse + specular) * ao;
    }

    vec3 color = ambient + light_out;

    // color = color / (color + vec3(1.0));
    color = vec3(1.0) - exp(-color * u_exposure);

    color = pow(color, vec3(1.0 / 2.2));

    f_color = vec4(color, alpha);

    gl_FragDepth = position.z;
}