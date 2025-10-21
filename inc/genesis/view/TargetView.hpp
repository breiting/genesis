#pragma once
#include <genesis/view/Shader.hpp>
#include <glm/mat4x4.hpp>

namespace gen {

/** Simple visual representation of a target (point) */
class TargetView {
   public:
    void Init();
    void Draw(const glm::vec2& target, float radius, const glm::vec4& color, const glm::mat4& vp);

   private:
    Shader m_Shader;
    unsigned int m_VAO = 0, m_VBO = 0;
};

}  // namespace gen
