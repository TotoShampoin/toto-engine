#include "toto-engine/mesh.hpp"
#include "toto-engine/gl/gldebug.hpp"
#include <format>
#include <vector>

namespace toto {

Model::Model(const std::vector<Vertex>& vertices, const std::vector<GLuint>& indices, const std::string& name)
    : name(name) {
    GLDebug::pushGroup(std::format("Create model: {}", name));

    vbo.bind();
    vbo.data(vertices, GL_STATIC_DRAW);
    vao.bind();
    vao.setAttribPointer<Vertex, glm::vec3, offsetof(Vertex, position)>(0);
    vao.setAttribPointer<Vertex, glm::vec3, offsetof(Vertex, normal)>(1);
    vao.setAttribPointer<Vertex, glm::vec2, offsetof(Vertex, texcoord)>(2);
    ibo.bind();
    ibo.data(indices, GL_STATIC_DRAW);
    index_count = indices.size();

    GLDebug::popGroup();
}

Model::~Model() {
    GLDebug::pushGroup(std::format("Delete model: {}", name));
    GLDebug::popGroup();
}

}; // namespace toto
