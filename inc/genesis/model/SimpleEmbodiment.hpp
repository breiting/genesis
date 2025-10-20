#pragma once
#include <genesis/model/IEmbodiment.hpp>

namespace gen {

/**
 * @brief Simple 2D embodiment: position + velocity integration.
 */
class SimpleEmbodiment : public IEmbodiment {
   public:
    explicit SimpleEmbodiment(glm::vec2 pos = {0, 0}) : m_Pos(pos) {
    }

    glm::vec2 GetPosition() const override {
        return m_Pos;
    }
    void SetPosition(const glm::vec2& pos) override {
        m_Pos = pos;
    }

    glm::vec2 GetVelocity() const override {
        return m_Vel;
    }
    void SetVelocity(const glm::vec2& vel) override {
        m_Vel = vel;
    }

    void Update(float dt) override {
        m_Pos += m_Vel * dt;
    }

   private:
    glm::vec2 m_Pos;
    glm::vec2 m_Vel{0, 0};
};

}  // namespace gen
