#include "toto-engine/utils/shapes.hpp"
#include "toto-engine/mesh.hpp"
#include <glm/gtc/constants.hpp>
#include <vector>

namespace toto {

namespace shape {

Model quad(float width, float height) {
    auto vertices = std::vector<Vertex>({
        {{-width / 2, -height / 2, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}},
        { {width / 2, -height / 2, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}},
        {  {width / 2, height / 2, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
        { {-width / 2, height / 2, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},
    });
    auto indices = std::vector<uint>({
        0, 1, 3, 1, 2, 3, //
    });
    return Model {vertices, indices};
}

Model cube(float width, float height, float depth) {
    auto vertices = std::vector<Vertex>({
        // front
        {{-width / 2, -height / 2, -depth / 2}, {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f}},
        { {width / 2, -height / 2, -depth / 2}, {0.0f, 0.0f, -1.0f}, {1.0f, 0.0f}},
        {  {width / 2, height / 2, -depth / 2}, {0.0f, 0.0f, -1.0f}, {1.0f, 1.0f}},
        { {-width / 2, height / 2, -depth / 2}, {0.0f, 0.0f, -1.0f}, {0.0f, 1.0f}},
        // back
        { {-width / 2, -height / 2, depth / 2},  {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}},
        {  {width / 2, -height / 2, depth / 2},  {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}},
        {   {width / 2, height / 2, depth / 2},  {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
        {  {-width / 2, height / 2, depth / 2},  {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},
        // right
        { {width / 2, -height / 2, -depth / 2},  {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
        {  {width / 2, -height / 2, depth / 2},  {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
        {   {width / 2, height / 2, depth / 2},  {1.0f, 0.0f, 0.0f}, {1.0f, 1.0f}},
        {  {width / 2, height / 2, -depth / 2},  {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f}},
        // left
        {{-width / 2, -height / 2, -depth / 2}, {-1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
        { {-width / 2, -height / 2, depth / 2}, {-1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
        {  {-width / 2, height / 2, depth / 2}, {-1.0f, 0.0f, 0.0f}, {1.0f, 1.0f}},
        { {-width / 2, height / 2, -depth / 2}, {-1.0f, 0.0f, 0.0f}, {0.0f, 1.0f}},
        // top
        { {-width / 2, height / 2, -depth / 2},  {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
        {  {width / 2, height / 2, -depth / 2},  {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
        {   {width / 2, height / 2, depth / 2},  {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f}},
        {  {-width / 2, height / 2, depth / 2},  {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f}},
        // bottom
        {{-width / 2, -height / 2, -depth / 2}, {0.0f, -1.0f, 0.0f}, {0.0f, 0.0f}},
        { {width / 2, -height / 2, -depth / 2}, {0.0f, -1.0f, 0.0f}, {1.0f, 0.0f}},
        {  {width / 2, -height / 2, depth / 2}, {0.0f, -1.0f, 0.0f}, {1.0f, 1.0f}},
        { {-width / 2, -height / 2, depth / 2}, {0.0f, -1.0f, 0.0f}, {0.0f, 1.0f}},
    });
    auto indices = std::vector<uint>({
        0,  1,  3,  1,  2,  3,  //
        4,  5,  7,  5,  6,  7,  //
        8,  9,  11, 9,  10, 11, //
        12, 13, 15, 13, 14, 15, //
        16, 17, 19, 17, 18, 19, //
        20, 21, 23, 21, 22, 23, //
    });
    return Model {vertices, indices};
}

Model sphere(float radius, uint slices, uint stacks) {
    auto vertices = std::vector<Vertex>();
    auto indices = std::vector<uint>();
    for (uint i = 0; i < stacks; i++) {
        float theta = glm::pi<float>() * i / stacks;
        for (uint j = 0; j < slices; j++) {
            float phi = 2 * glm::pi<float>() * j / slices;

            float x = glm::sin(theta) * glm::cos(phi);
            float y = glm::cos(theta);
            float z = glm::sin(theta) * glm::sin(phi);

            vertices.push_back({
                glm::vec3(x, y, z) * radius,
                glm::vec3(x, y, z),
                {j / static_cast<float>(slices), i / static_cast<float>(stacks)}
            });
        }
    }
    for (uint i = 1; i < stacks; i++) {
        for (uint j = 0; j < slices; j++) {
            indices.push_back((i - 1) * slices + j + 1);
            indices.push_back(i * slices + j + 1);
            indices.push_back(i * slices + (j + 1) % slices + 1);
            indices.push_back((i - 1) * slices + j + 1);
            indices.push_back(i * slices + (j + 1) % slices + 1);
            indices.push_back((i - 1) * slices + (j + 1) % slices + 1);
        }
    }
    return Model {vertices, indices};
}

Model cylinder(float radius, float height, uint slices) {
    auto vertices = std::vector<Vertex>();
    auto indices = std::vector<uint>();
    // top disk
    uint idx = 0;
    vertices.push_back({
        {0.0f, 0.0f, 0.0f},
        {0.0f, -1.0f, 0.0f},
        {0.5f, 0.5f}
    });
    for (uint i = 0; i < slices; i++) {
        float angle = 2 * glm::pi<float>() * i / slices;
        vertices.push_back({
            {radius * glm::cos(angle), 0.0f, radius * glm::sin(angle)},
            {0.0f, -1.0f, 0.0f},
            {0.5f + glm::cos(angle) / 2, 0.5f + glm::sin(angle) / 2}
        });
    }
    for (uint i = 0; i < slices; i++) {
        indices.push_back(0);
        indices.push_back(i + 1);
        indices.push_back((i + 1) % slices + 1);
    }
    idx += slices + 1;
    // bottom disk
    vertices.push_back({
        {0.0f, height, 0.0f},
        {0.0f, 1.0f, 0.0f},
        {0.5f, 0.5f}
    });
    for (uint i = 0; i < slices; i++) {
        float angle = 2 * glm::pi<float>() * i / slices;
        vertices.push_back({
            {radius * glm::cos(angle), height, radius * glm::sin(angle)},
            {0.0f, 1.0f, 0.0f},
            {0.5f + glm::cos(angle) / 2, 0.5f + glm::sin(angle) / 2}
        });
    }
    for (uint i = 0; i < slices; i++) {
        indices.push_back(idx);
        indices.push_back(idx + i + 1);
        indices.push_back(idx + (i + 1) % slices + 1);
    }
    idx += slices + 1;
    // side
    for (uint i = 0; i < slices; i++) {
        float angle = 2 * glm::pi<float>() * i / slices;
        vertices.push_back({
            {radius * glm::cos(angle), 0.0f, radius * glm::sin(angle)},
            {glm::cos(angle), 0.0f, glm::sin(angle)},
            {0.5f + glm::cos(angle) / 2, 0.5f + glm::sin(angle) / 2}
        });
        vertices.push_back({
            {radius * glm::cos(angle), height, radius * glm::sin(angle)},
            {glm::cos(angle), 0.0f, glm::sin(angle)},
            {0.5f + glm::cos(angle) / 2, 0.5f + glm::sin(angle) / 2}
        });
    }
    for (uint i = 0; i < slices; i++) {
        indices.push_back(idx + 2 * i);
        indices.push_back(idx + 2 * i + 1);
        indices.push_back(idx + 2 * ((i + 1) % slices) + 1);
        indices.push_back(idx + 2 * i);
        indices.push_back(idx + 2 * ((i + 1) % slices) + 1);
        indices.push_back(idx + 2 * ((i + 1) % slices));
    }
    return Model {vertices, indices};
}

Model cone(float radius, float height, uint slices) {
    auto vertices = std::vector<Vertex>();
    auto indices = std::vector<uint>();
    vertices.push_back({
        {0.0f, height / 2, 0.0f},
        {0.0f, 1.0f, 0.0f},
        {0.5f, 1.0f}
    });
    for (uint i = 0; i < slices; i++) {
        float angle = 2 * glm::pi<float>() * i / slices;
        vertices.push_back({
            {radius * glm::cos(angle), -height / 2, radius * glm::sin(angle)},
            {glm::cos(angle), 0.0f, glm::sin(angle)},
            {0.5f + glm::cos(angle) / 2, 0.5f + glm::sin(angle) / 2}
        });
    }
    for (uint i = 0; i < slices; i++) {
        indices.push_back(0);
        indices.push_back(i + 1);
        indices.push_back((i + 1) % slices + 1);
    }
    return Model {vertices, indices};
}

// https://github.com/mrdoob/three.js/blob/dev/src/geometries/TorusGeometry.js
Model torus(float major_radius, float minor_radius, uint major_slices, uint minor_slices) {
    auto vertices = std::vector<Vertex>();
    auto indices = std::vector<uint>();
    glm::vec3 vertex = {0.0f, 0.0f, 0.0f};
    glm::vec3 center = {0.0f, 0.0f, 0.0f};
    glm::vec3 normal = {0.0f, 0.0f, 0.0f};
    for (int j = 0; j <= minor_slices; j++) {
        float v = j / static_cast<float>(minor_slices) * 2 * glm::pi<float>();
        for (int i = 0; i <= major_slices; i++) {
            float u = i / static_cast<float>(major_slices) * 2 * glm::pi<float>();

            vertex.x = (major_radius + minor_radius * glm::cos(v)) * glm::cos(u);
            vertex.y = minor_radius * glm::sin(v);
            vertex.z = (major_radius + minor_radius * glm::cos(v)) * glm::sin(u);

            center.x = major_radius * glm::cos(u);
            center.z = major_radius * glm::sin(u);
            normal = glm::normalize(vertex - center);

            glm::vec2 texcoord = {i / static_cast<float>(major_slices), j / static_cast<float>(minor_slices)};

            vertices.push_back({vertex, normal, texcoord});
        }
    }
    for (int j = 1; j <= minor_slices; j++) {
        for (int i = 1; i <= major_slices; i++) {
            int a = (major_slices + 1) * j + i - 1;
            int b = (major_slices + 1) * (j - 1) + i - 1;
            int c = (major_slices + 1) * (j - 1) + i;
            int d = (major_slices + 1) * j + i;
            indices.push_back(a);
            indices.push_back(b);
            indices.push_back(d);
            indices.push_back(b);
            indices.push_back(c);
            indices.push_back(d);
        }
    }
    return Model {vertices, indices};
}

Model disk(float radius, uint slices) {
    auto vertices = std::vector<Vertex>();
    auto indices = std::vector<uint>();
    vertices.push_back({
        {0.0f, 0.0f, 0.0f},
        {0.0f, -1.0f, 0.0f},
        {0.5f, 0.5f}
    });
    for (uint i = 0; i < slices; i++) {
        float angle = 2 * glm::pi<float>() * i / slices;
        vertices.push_back({
            {radius * glm::cos(angle), 0.0f, radius * glm::sin(angle)},
            {0.0f, -1.0f, 0.0f},
            {0.5f + glm::cos(angle) / 2, 0.5f + glm::sin(angle) / 2}
        });
    }
    for (uint i = 0; i < slices; i++) {
        indices.push_back(0);
        indices.push_back(i + 1);
        indices.push_back((i + 1) % slices + 1);
    }
    return Model {vertices, indices};
}

Model capsule(float radius, float height, uint slices) {
    auto vertices = std::vector<Vertex>();
    auto indices = std::vector<uint>();
    // top hemisphere
    uint idx = 0;
    vertices.push_back({
        {0.0f, height / 2, 0.0f},
        {0.0f, 1.0f, 0.0f},
        {0.5f, 1.0f}
    });
    for (uint i = 0; i < slices; i++) {
        float angle = glm::pi<float>() * i / slices;
        for (uint j = 0; j < slices; j++) {
            float phi = 2 * glm::pi<float>() * j / slices;
            vertices.push_back({
                {radius * glm::sin(angle) * glm::cos(phi), height / 2 + radius * glm::cos(angle),
                 radius * glm::sin(angle) * glm::sin(phi)},
                {glm::sin(angle) * glm::cos(phi), glm::cos(angle), glm::sin(angle) * glm::sin(phi)},
                {j / static_cast<float>(slices), i / static_cast<float>(slices)}
            });
        }
    }
    for (uint i = 0; i < slices; i++) {
        for (uint j = 0; j < slices; j++) {
            indices.push_back(idx + i * slices + j + 1);
            indices.push_back(idx + i * slices + (j + 1) % slices + 1);
            indices.push_back(idx + (i + 1) * slices + (j + 1) % slices + 1);
            indices.push_back(idx + i * slices + j + 1);
            indices.push_back(idx + (i + 1) * slices + (j + 1) % slices + 1);
            indices.push_back(idx + (i + 1) * slices + j + 1);
        }
    }
    idx += slices * slices + 1;
    // bottom hemisphere
    vertices.push_back({
        {0.0f, -height / 2, 0.0f},
        {0.0f, -1.0f, 0.0f},
        {0.5f, 0.0f}
    });
    for (uint i = 0; i < slices; i++) {
        float angle = glm::pi<float>() * i / slices;
        for (uint j = 0; j < slices; j++) {
            float phi = 2 * glm::pi<float>() * j / slices;
            vertices.push_back({
                {radius * glm::sin(angle) * glm::cos(phi), -height / 2 + radius * glm::cos(angle),
                 radius * glm::sin(angle) * glm::sin(phi)},
                {glm::sin(angle) * glm::cos(phi), glm::cos(angle), glm::sin(angle) * glm::sin(phi)},
                {j / static_cast<float>(slices), i / static_cast<float>(slices)}
            });
        }
    }
    for (uint i = 0; i < slices; i++) {
        for (uint j = 0; j < slices; j++) {
            indices.push_back(idx + i * slices + j + 1);
            indices.push_back(idx + i * slices + (j + 1) % slices + 1);
            indices.push_back(idx + (i + 1) * slices + (j + 1) % slices + 1);
            indices.push_back(idx + i * slices + j + 1);
            indices.push_back(idx + (i + 1) * slices + (j + 1) % slices + 1);
            indices.push_back(idx + (i + 1) * slices + j + 1);
        }
    }
    // side
    for (uint i = 0; i < slices; i++) {
        float angle = glm::pi<float>() * i / slices;
        vertices.push_back({
            {radius * glm::sin(angle), height / 2 + radius * glm::cos(angle), 0.0f},
            {0.0f, 1.0f, 0.0f},
            {0.5f, 0.5f}
        });
        vertices.push_back({
            {radius * glm::sin(angle), -height / 2 + radius * glm::cos(angle), 0.0f},
            {0.0f, -1.0f, 0.0f},
            {0.5f, 0.5f}
        });
    }
    for (uint i = 0; i < slices; i++) {
        indices.push_back(idx + 2 * i);
        indices.push_back(idx + 2 * i + 1);
        indices.push_back(idx + 2 * ((i + 1) % slices) + 1);
        indices.push_back(idx + 2 * i);
        indices.push_back(idx + 2 * ((i + 1) % slices) + 1);
        indices.push_back(idx + 2 * ((i + 1) % slices));
    }
    return Model {vertices, indices};
}

} // namespace shape

} // namespace toto
