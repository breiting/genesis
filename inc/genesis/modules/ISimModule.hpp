#pragma once
#include <glm/mat4x4.hpp>
#include <string>

namespace gen {

/**
 * @brief Abstract base class for all simulation modules in Genesis.
 *
 * A simulation module represents a self-contained world or process
 * that can be updated, rendered, and controlled via a UI panel.
 *
 * Examples: Brownian motion, agent evolution, reaction-diffusion, etc.
 */
class ISimModule {
   public:
    virtual ~ISimModule() = default;

    /**
     * @brief Returns the display name of this simulation module.
     * Used for UI and module selection.
     */
    virtual std::string Name() const = 0;

    /**
     * @brief Called once when the module is created and attached.
     * Use this to initialize GPU resources or state.
     */
    virtual void OnAttach() {
    }

    /**
     * @brief Called once when the module is removed or replaced.
     * Clean up any allocated resources.
     */
    virtual void OnDetach() {
    }

    /**
     * @brief Reset the simulation to its initial state.
     * Called when the user presses "Reset" or restarts the sim.
     */
    virtual void Reset() = 0;

    /**
     * @brief Advance the simulation logic.
     * @param dt Delta time in seconds.
     */
    virtual void Update(double dt) = 0;

    /**
     * @brief Render the simulation’s visual world content.
     * @param vp View-projection matrix from the camera.
     */
    virtual void RenderWorld(const glm::mat4& vp) = 0;

    /**
     * @brief Draw this module’s ImGui user interface controls.
     * Called inside the global app’s left control panel.
     */
    virtual void RenderImGui() = 0;
};

}  // namespace gen
