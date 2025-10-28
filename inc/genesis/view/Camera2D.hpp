#pragma once
#include <glm/mat4x4.hpp>
#include <string>

namespace gen {

/** Simple orthographic 2D camera (view-projection only). */
class Camera2D {
   public:
    Camera2D();
    void SetOrtho(float l, float r, float b, float t);

    void SetViewportSize(const glm::vec2& size);

    glm::vec2 ScreenToWorld(const glm::vec2& screenPos);

    void SetPosition(glm::vec2 pos);
    void SetZoom(float z);

    glm::mat4 ViewProj() const;

    void ZoomAtCursor(float zoomDelta, const glm::vec2& cursorScreen);
    void Pan(const glm::vec2& delta);
    void FitTo(const glm::vec2& areaSize);

    void Update(float dt);

    std::string GetInfo() const;

   private:
    glm::vec2 m_Position{0.0f};
    glm::vec2 m_TargetPosition{0.0f};
    glm::vec2 m_Velocity{0.0f};

    float m_Zoom = 1.0f;
    float m_TargetZoom = 1.0f;

    float m_Left, m_Right, m_Bottom, m_Top;

    // Settings
    float m_MinZoom = 5.0f;
    float m_MaxZoom = 100.0f;
    float m_PanDamping = 0.85f;
    float m_ZoomLerp = 0.2f;
    float m_MoveLerp = 0.2f;

    glm::vec2 m_ViewportSize{800.0f, 600.0f};

    //
};

}  // namespace gen
