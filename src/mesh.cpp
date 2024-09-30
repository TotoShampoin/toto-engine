#include "toto-engine/mesh.hpp"
#include <vector>

namespace toto {

Model::Model(const std::vector<Vertex>& vertices, const std::vector<GLuint>& indices) {
    vbo.bind();
    vbo.data(vertices, GL_STATIC_DRAW);
    vao.bind();
    vao.setAttribPointer<Vertex, glm::vec3, offsetof(Vertex, position)>(0);
    vao.setAttribPointer<Vertex, glm::vec3, offsetof(Vertex, normal)>(1);
    vao.setAttribPointer<Vertex, glm::vec2, offsetof(Vertex, texcoord)>(2);
    ibo.bind();
    ibo.data(indices, GL_STATIC_DRAW);
    index_count = indices.size();
}

}; // namespace toto
