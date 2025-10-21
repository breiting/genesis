#pragma once
#include <glm/mat4x4.hpp>

namespace gen {

/** Simple orthographic 2D camera (view-projection only). */
class Camera2D {
   public:
    Camera2D();
    void SetOrtho(float l, float r, float b, float t);
    void SetOrthoByHeight(float height, float aspect);
    void SetOrthoByWidth(float width, float aspect);

    glm::vec2 ScreenToWorld(const glm::vec2& screenPos, int width, int height);

    void SetPosition(glm::vec2 pos);
    void SetZoom(float z);
    glm::mat4 ViewProj() const;

    void ZoomAtCursor(float zoomDelta, const glm::vec2& cursorScreen, int screenWidth, int screenHeight);
    void Pan(const glm::vec2& delta);

    void Update(float dt);

   private:
    glm::vec2 m_Position;
    float m_Left, m_Right, m_Bottom, m_Top;
    float m_Zoom;
    glm::vec2 m_TargetPosition;
    float m_TargetZoom = 1.0f;
    float m_LerpSpeed = 0.15f;  // for smooth panning/zooming
};

}  // namespace gen
