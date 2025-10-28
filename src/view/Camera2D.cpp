#include <algorithm>
#include <genesis/view/Camera2D.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace gen {

Camera2D::Camera2D() : m_Zoom(1.0f), m_Left(-1.6f), m_Right(1.6f), m_Bottom(-0.9f), m_Top(0.9f) {
}

void Camera2D::SetOrtho(float l, float r, float b, float t) {
    m_Left = l;
    m_Right = r;
    m_Bottom = b;
    m_Top = t;
}

void Camera2D::SetViewportSize(const glm::vec2& size) {
    m_ViewportSize = size;
}

void Camera2D::SetPosition(glm::vec2 pos) {
    m_Position = m_TargetPosition = pos;
}
void Camera2D::SetZoom(float z) {
    m_TargetZoom = m_Zoom = glm::clamp(z, m_MinZoom, m_MaxZoom);
}

void Camera2D::Update(float /*dt*/) {
    // smooth zoom
    m_Zoom += (m_TargetZoom - m_Zoom) * m_ZoomLerp;

    // smooth move
    m_Position += (m_TargetPosition - m_Position) * m_MoveLerp;

    // apply velocity damping if used
    m_Velocity *= m_PanDamping;
}

void Camera2D::ZoomAtCursor(float zoomDelta, const glm::vec2& cursorScreen) {
    // world pos before zoom
    glm::vec2 before = ScreenToWorld(cursorScreen);

    // target zoom adjust
    m_TargetZoom *= (1.0f + zoomDelta);
    m_TargetZoom = glm::clamp(m_TargetZoom, m_MinZoom, m_MaxZoom);

    // world pos after zoom
    glm::vec2 after = ScreenToWorld(cursorScreen);

    // shift camera so the cursor point stays fixed
    m_TargetPosition += (before - after);
}

void Camera2D::Pan(const glm::vec2& delta) {
    m_TargetPosition -= delta / m_Zoom;
}

void Camera2D::FitTo(const glm::vec2& areaSize) {
    float scaleX = (m_ViewportSize.x / areaSize.x);
    float scaleY = (m_ViewportSize.y / areaSize.y);
    m_TargetZoom = 0.9f * std::min(scaleX, scaleY);
}

glm::vec2 Camera2D::ScreenToWorld(const glm::vec2& screen) {
    // screen → NDC (-1..1)
    glm::vec2 ndc = (screen / m_ViewportSize) * 2.0f - 1.0f;
    // world half extents
    float halfH = (m_ViewportSize.y * 0.5f) / m_Zoom;
    float halfW = (m_ViewportSize.x * 0.5f) / m_Zoom;
    // world position
    glm::vec2 world = m_Position + ndc * glm::vec2(halfW, halfH);
    // flip y so top screen = +y up
    world.y = m_Position.y - ndc.y * halfH;
    return world;
}

glm::mat4 Camera2D::ViewProj() const {
    // height in world units = viewport height / zoom
    float halfH = (m_ViewportSize.y * 0.5f) / m_Zoom;
    float halfW = (m_ViewportSize.x * 0.5f) / m_Zoom;

    glm::mat4 proj = glm::ortho(-halfW, halfW, -halfH, halfH);
    glm::mat4 view(1.0f);
    view = glm::translate(view, glm::vec3(-m_Position, 0.0f));
    return proj * view;
}

}  // namespace gen
