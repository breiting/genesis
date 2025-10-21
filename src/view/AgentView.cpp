// clang-format off
#include <glad.h>
#include <GLFW/glfw3.h>
// clang-format on

#include <genesis/view/AgentView.hpp>
#include <genesis/view/Uniforms.hpp>
#include <glm/glm.hpp>

namespace gen {

AgentView::AgentView() {
}

AgentView::~AgentView() {
    if (m_Vao) glDeleteVertexArrays(1, &m_Vao);
    if (m_Vbo) glDeleteBuffers(1, &m_Vbo);
}

void AgentView::Init() {
    m_Shader.BuildAgent();

    glGenVertexArrays(1, &m_Vao);
    glGenBuffers(1, &m_Vbo);

    // Layout (binding für Shader-Instanced Attributes)
    std::size_t stride = sizeof(InstanceData);

    // ----- Body VAO -----
    glBindVertexArray(m_Vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_Vbo);

    glEnableVertexAttribArray(1);  // position
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(InstanceData, position));
    glVertexAttribDivisor(1, 1);

    glEnableVertexAttribArray(2);  // radius
    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(InstanceData, radius));
    glVertexAttribDivisor(2, 1);

    glEnableVertexAttribArray(3);  // color
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(InstanceData, color));
    glVertexAttribDivisor(3, 1);

    glBindVertexArray(0);
}

void AgentView::UpdateInstances(const std::vector<std::unique_ptr<Agent>>& agents) {
    m_InstanceData.clear();
    m_InstanceData.reserve(agents.size());

    for (auto& agent : agents) {
        InstanceData d;
        d.position = agent->GetEmbodiment()->GetPosition();
        d.radius = agent->GetEmbodiment()->GetRadius();
        float f = agent->GetFitness();
        glm::vec3 c = glm::mix(glm::vec3(1, 0, 0), glm::vec3(0, 1, 0), f);
        d.color = glm::vec4(c.r, c.g, c.b, 1.0);
        m_InstanceData.push_back(d);
    }

    glBindBuffer(GL_ARRAY_BUFFER, m_Vbo);
    glBufferData(GL_ARRAY_BUFFER, m_InstanceData.size() * sizeof(InstanceData), m_InstanceData.data(), GL_DYNAMIC_DRAW);
}

void AgentView::Draw(const glm::mat4& vp) {
    if (m_InstanceData.empty()) return;

    m_Shader.Bind();
    m_Shader.SetMat4(Uniforms::MVP, vp);
    glBindVertexArray(m_Vao);
    glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, static_cast<GLsizei>(m_InstanceData.size()));

    glBindVertexArray(0);
}

}  // namespace gen
