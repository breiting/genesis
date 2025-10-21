#pragma once

#include <genesis/model/Agent.hpp>
#include <genesis/view/Shader.hpp>
#include <glm/mat4x4.hpp>
#include <vector>

namespace gen {

/**
 * @brief GPU instanced renderer for a collection of embodied agents
 *
 * Call Init() once to create OpenGL buffers, then UpdateInstances() to upload
 * particle transforms each frame prior to Draw().
 */
class AgentView {
   public:
    AgentView();
    ~AgentView();

    /** @brief Lazily create shader program and instance buffers. */
    void Init();
    /**
     * @brief Upload per-agent instance data to the GPU.
     * @param agents Source agent collection.
     */
    void UpdateInstances(const std::vector<std::unique_ptr<Agent>>& agents);
    /**
     * @brief Render the batched agents using the given view-projection matrix.
     */
    void Draw(const glm::mat4& vp);

   private:
    struct InstanceData {
        glm::vec2 position;
        float radius;
        glm::vec4 color;
    };

    GLuint m_Vao = 0;
    GLuint m_Vbo = 0;
    Shader m_Shader;

    std::vector<InstanceData> m_InstanceData;
};

}  // namespace gen
