#pragma once

#include <genesis/core/IApp.hpp>
#include <genesis/model/Agent.hpp>
#include <genesis/model/Trainer.hpp>
#include <genesis/ui/IGuiLayer.hpp>
#include <genesis/view/AgentView.hpp>
#include <genesis/view/Camera2D.hpp>
#include <memory>

#include "genesis/model/MovementEvaluator.hpp"
#include "genesis/view/CanvasView.hpp"
#include "genesis/view/TargetView.hpp"

using namespace gen;

struct Viewport {
    int x = 0, y = 0, w = 0, h = 0;  // y = top-left origin
};

/**
 * @brief Standalone app for genesis
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
    void DrawWorld();
    void CreateAgent(const glm::vec2& pos, const glm::vec2& target);
    void RepositionAgents(bool random = false);
    void ApplyDarkStyle();

    // Handle ImGui panel
    bool MouseInWorldVP(const glm::vec2& mouse);
    glm::vec2 ToLocalViewport(const glm::vec2& mouseScreen);
    // Gets the correct mouse position in proper viewport
    glm::vec2 GetMousePosition();

   private:
    std::vector<std::unique_ptr<Agent>> m_Agents;
    std::unique_ptr<Trainer> m_Trainer;
    std::unique_ptr<IGuiLayer> m_Gui;

    Camera2D m_Camera;
    Viewport m_ViewportWorld;
    Viewport m_ViewportUi;

    // Views
    CanvasView m_CanvasView;
    AgentView m_AgentView;
    TargetView m_StartView;
    TargetView m_TargetView;

    glm::vec2 m_WorldSize{80.0f, 80.0f};
    glm::vec2 m_StartPos{-30.0f, 0.0f};
    glm::vec2 m_TargetPos{0.0f, 0.0f};

    // Mouse handling
    glm::vec2 m_MousePos{0, 0};
    glm::vec2 m_DragStart{0, 0};
    bool m_IsDragging = false;

    float m_Timescale = 1.0f;
    int m_NumAgents = 100;
    int m_GenerationCount = 0;

    std::unique_ptr<MovementEvaluator> m_Evaluator;

    bool m_IsObserving = false;

    Genome m_BestGenome;

    bool m_SetStartPos = false;

    bool m_Quit{false};
};
