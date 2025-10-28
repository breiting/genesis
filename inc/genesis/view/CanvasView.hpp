#pragma once
#include <genesis/view/Shader.hpp>
#include <glm/glm.hpp>

#include "genesis/core/Colors.hpp"

namespace gen {

/**
 * @brief Visual representation of a 2D canvas (the world rectangle).
 *
 * Draws a smooth rectangular background with rounded corners and subtle shadow.
 */
class CanvasView {
   public:
    CanvasView(const glm::vec4& backgroundColor = Colors::Nord0);
    ~CanvasView();

    void Init();
    void Draw(const glm::vec2& size, const glm::mat4& vp);

   private:
    Shader m_Shader;
    unsigned int m_Vao = 0, m_Vbo = 0;

    float m_Radius = 1.0;
    glm::vec4 m_GlassColor = {0.75f, 0.78f, 0.82f, 1.0f};
    glm::vec4 m_BackgroundColor;
};

}  // namespace gen
