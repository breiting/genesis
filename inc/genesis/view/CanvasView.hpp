#pragma once
#include <genesis/view/Shader.hpp>
#include <glm/glm.hpp>

namespace gen {

/**
 * @brief Visual representation of a 2D canvas (the world rectangle).
 *
 * Draws a smooth rectangular background with rounded corners and subtle shadow.
 */
class CanvasView {
   public:
    CanvasView();
    ~CanvasView();

    void Init();
    void Draw(const glm::vec2& size, const glm::mat4& vp);

   private:
    Shader m_Shader;
    unsigned int m_Vao = 0, m_Vbo = 0;

    float m_Radius = 0.25;
    float m_ShadowSize = 5.0f;
    glm::vec4 m_ColorInner = {0.2f, 0.8f, 0.3f, 1.0f};  //{0.17f, 0.17f, 0.18f, 1.0f};
    // glm::vec4 m_ColorShadow = {0.07f, 0.07f, 0.07f, 1.0f};
    glm::vec4 m_ColorShadow = {1.0, 0.0, 0.0, 1.0};
};

}  // namespace gen
