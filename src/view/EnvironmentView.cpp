#include <glad.h>

#include <genesis/view/EnvironmentView.hpp>
#include <genesis/view/Uniforms.hpp>

#include "glm/ext/matrix_transform.hpp"

using namespace gen;

EnvironmentView::EnvironmentView() {
}

EnvironmentView::~EnvironmentView() {
    if (m_Vao) glDeleteVertexArrays(1, &m_Vao);
    if (m_Vbo) glDeleteBuffers(1, &m_Vbo);
}

void EnvironmentView::Init() {
    m_Shader.BuildEnvironment();

    // Fullscreen quad (no indices needed)
    const float quadVerts[] = {-0.5f, -0.5f, 0.5f, -0.5f, -0.5f, 0.5f, 0.5f, 0.5f};

    glGenVertexArrays(1, &m_Vao);
    glGenBuffers(1, &m_Vbo);
    glBindVertexArray(m_Vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_Vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVerts), quadVerts, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

    glBindVertexArray(0);
}

void EnvironmentView::Draw(const glm::vec2& size, const glm::mat4& vp) {
    m_Shader.Bind();

    glm::mat4 model = glm::scale(glm::mat4(1.0f), glm::vec3(size.x, size.y, 1.0f));
    glm::mat4 mvp = vp * model;

    m_Shader.SetMat4(Uniforms::MVP, mvp);
    m_Shader.SetVec2(Uniforms::SIZE, size);
    m_Shader.SetFloat(Uniforms::CORNER_RADIUS, m_Radius);
    m_Shader.SetFloat(Uniforms::SHADOW_SIZE, m_ShadowSize);
    m_Shader.SetVec4(Uniforms::COLOR, m_ColorInner);
    m_Shader.SetVec4(Uniforms::SHADOW_COLOR, m_ColorShadow);

    glBindVertexArray(m_Vao);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}
