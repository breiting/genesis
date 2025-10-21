#include <glad.h>

#include <genesis/view/TargetView.hpp>
#include <genesis/view/Uniforms.hpp>

namespace gen {

void TargetView::Init() {
    m_Shader.BuildTarget();
    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
}

void TargetView::Draw(const glm::vec2& target, float radius, const glm::vec4& color, const glm::mat4& vp) {
    m_Shader.Bind();
    m_Shader.SetMat4(Uniforms::MVP, vp);
    m_Shader.SetVec2(Uniforms::POS, target);
    m_Shader.SetFloat(Uniforms::uRADIUS, radius);
    m_Shader.SetFloat(Uniforms::uBLENDWIDTH, 0.05f);
    m_Shader.SetVec4(Uniforms::uCOLOR, color);

    glBindVertexArray(m_VAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}

}  // namespace gen
