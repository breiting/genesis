#pragma once

#include <genesis/core/IApp.hpp>
#include <genesis/modules/ISimModule.hpp>
#include <genesis/ui/IGuiLayer.hpp>
#include <genesis/view/Camera2D.hpp>
#include <genesis/view/CanvasView.hpp>
#include <memory>

using namespace gen;

struct Viewport {
    int x = 0, y = 0, w = 0, h = 0;  // y = top-left origin
};

/**
 * @brief Main Genesis application runner.
 *
 * Manages window, camera, ImGui UI, and simulation module lifecycle.
 * The app itself contains no simulation logic — it delegates all logic
 * to the currently active ISimModule (e.g., BrownianSim, NeuroEvolutionSim).
 */
class EvolutionApp : public IApp {
   public:
    EvolutionApp() = default;
    ~EvolutionApp() = default;

    // IApp interface
    std::string Name() const override;
    bool Init(AppContext& ctx) override;
    void Update(AppContext& ctx, double dt) override;
    void Render(AppContext& ctx) override;

   private:
    void DrawControlPanel();
    void ApplyDarkStyle();

    // Viewport handling
    bool MouseInWorldVP(const glm::vec2& mouse);
    glm::vec2 ToLocalViewport(const glm::vec2& mouseScreen);
    glm::vec2 GetMousePosition();

   private:
    // Modules
    std::vector<std::unique_ptr<ISimModule>> m_Modules;
    ISimModule* m_CurrentModule = nullptr;

    std::unique_ptr<IGuiLayer> m_Gui;

    Camera2D m_Camera;
    Viewport m_ViewportWorld;
    Viewport m_ViewportUi;

    // Views
    CanvasView m_CanvasView;
    glm::vec2 m_WorldSize{10.0f, 10.0f};

    // Mouse handling
    glm::vec2 m_MousePos{0, 0};
    glm::vec2 m_DragStart{0, 0};
    bool m_IsDragging = false;

    float m_Timescale = 1.0f;

    bool m_Quit{false};

    static constexpr int ImGuiPanelWidth = 350;
};
