#include <genesis/view/Camera2D.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace gen {

Camera2D::Camera2D() : m_Left(-1.6f), m_Right(1.6f), m_Bottom(-0.9f), m_Top(0.9f), m_Zoom(1.0f) {
}

void Camera2D::SetOrtho(float l, float r, float b, float t) {
    m_Left = l;
    m_Right = r;
    m_Bottom = b;
    m_Top = t;
}

void Camera2D::SetOrthoByHeight(float height, float aspect) {
    float halfH = height * 0.5f;
    float halfW = halfH * aspect;
    m_Left = -halfW;
    m_Right = +halfW;
    m_Bottom = -halfH;
    m_Top = +halfH;
}

void Camera2D::SetOrthoByWidth(float width, float aspect) {
    float halfW = width * 0.5f;
    float halfH = halfW / aspect;
    m_Left = -halfW;
    m_Right = +halfW;
    m_Bottom = -halfH;
    m_Top = +halfH;
}

void Camera2D::SetPosition(glm::vec2 pos) {
    m_Position = pos;
}
void Camera2D::SetZoom(float z) {
    m_Zoom = z;
}

void Camera2D::Update(float /*dt*/) {
    // Smooth Interpolation
    m_Position += (m_TargetPosition - m_Position) * m_LerpSpeed;
    m_Zoom += (m_TargetZoom - m_Zoom) * m_LerpSpeed;
}

void Camera2D::ZoomAtCursor(float zoomDelta, const glm::vec2& cursorScreen, int width, int height) {
    m_TargetZoom = glm::clamp(m_TargetZoom + zoomDelta, 0.05f, 20.0f);

    glm::vec2 worldBefore = ScreenToWorld(cursorScreen, width, height);

    // After zoom, this point would shift – we compensate that
    glm::vec2 worldAfter = ScreenToWorld(cursorScreen, width, height);
    glm::vec2 delta = worldBefore - worldAfter;

    m_TargetPosition += delta;
}

void Camera2D::Pan(const glm::vec2& delta) {
    m_TargetPosition += delta / m_Zoom;
}

glm::vec2 Camera2D::ScreenToWorld(const glm::vec2& screen, int width, int height) {
    float xNDC = 2.0f * (screen.x / (float)width) - 1.0f;
    float yNDC = 1.0f - 2.0f * (screen.y / (float)height);
    glm::vec4 clipCoords(xNDC, yNDC, 0.0f, 1.0f);
    glm::mat4 inv = glm::inverse(ViewProj());
    glm::vec4 world = inv * clipCoords;
    return glm::vec2{world.x, world.y};
}

glm::mat4 Camera2D::ViewProj() const {
    const float cx = m_Position.x, cy = m_Position.y;
    glm::mat4 V = glm::translate(glm::mat4(1.f), glm::vec3(-cx, -cy, 0.f));
    float zl = (m_Right - m_Left) * 0.5f / m_Zoom;
    float zt = (m_Top - m_Bottom) * 0.5f / m_Zoom;
    glm::mat4 P = glm::ortho(-zl, zl, -zt, zt, -1.f, 1.f);
    return P * V;
}

}  // namespace gen
