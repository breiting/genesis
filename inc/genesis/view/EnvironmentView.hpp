#pragma once
#include <genesis/view/Shader.hpp>
#include <glm/glm.hpp>

namespace gen {

/**
 * @brief Visual representation of the environment area (the world rectangle).
 *
 * Draws a smooth rectangular background with rounded corners and subtle shadow.
 */
class EnvironmentView {
   public:
    EnvironmentView();
    ~EnvironmentView();

    void Init();
    void Draw(const glm::vec2& size, const glm::mat4& vp);

   private:
    Shader m_Shader;
    unsigned int m_Vao = 0, m_Vbo = 0;

    glm::vec4 m_ColorInner = {0.16f, 0.16f, 0.17f, 1.0f};
    // glm::vec4 m_ColorShadow = {0.08f, 0.08f, 0.08f, 1.0f};
    glm::vec4 m_ColorShadow = {1.f, 0.f, 0.0f, 1.0f};
    float m_Radius = 4.0f;       // corner radius
    float m_ShadowSize = 0.02f;  // shadow blur factor
};

}  // namespace gen
