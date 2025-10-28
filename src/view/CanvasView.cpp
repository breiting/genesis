#include <glad.h>

#include <genesis/view/CanvasView.hpp>
#include <genesis/view/Uniforms.hpp>
#include <glm/ext/matrix_transform.hpp>

using namespace gen;

CanvasView::CanvasView(const glm::vec4& backgroundColor) : m_BackgroundColor(backgroundColor) {
}

CanvasView::~CanvasView() {
    if (m_Vao) glDeleteVertexArrays(1, &m_Vao);
    if (m_Vbo) glDeleteBuffers(1, &m_Vbo);
}

void CanvasView::Init() {
    m_Shader.BuildCanvas();

    const float quadVerts[] = {
        -0.5f, -0.5f, 0.5f, -0.5f, -0.5f, 0.5f, 0.5f, 0.5f,
    };

    glGenVertexArrays(1, &m_Vao);
    glGenBuffers(1, &m_Vbo);

    glBindVertexArray(m_Vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_Vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVerts), quadVerts, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glBindVertexArray(0);
}

void CanvasView::Draw(const glm::vec2& size, const glm::mat4& vp) {
    m_Shader.Bind();

    glm::mat4 model = glm::scale(glm::mat4(1.0f), glm::vec3(size.x, size.y, 1.0f));
    glm::mat4 mvp = vp * model;

    m_Shader.SetMat4(Uniforms::MVP, mvp);
    m_Shader.SetVec2(Uniforms::SIZE, size);
    m_Shader.SetFloat(Uniforms::RADIUS, size.x / 100.0);  // m_Radius);
    m_Shader.SetVec3(Uniforms::GLASS_COLOR, m_GlassColor);
    m_Shader.SetFloat(Uniforms::ALPHA, 0.2f);
    m_Shader.SetFloat(Uniforms::FROST, 0.15f);
    m_Shader.SetVec3(Uniforms::BACKGROUND_COLOR, m_BackgroundColor);

    glBindVertexArray(m_Vao);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}
