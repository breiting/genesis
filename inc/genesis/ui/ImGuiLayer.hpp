#pragma once
#include <genesis/ui/IGuiLayer.hpp>

struct ImGuiContext;
struct GLFWwindow;

namespace gen {

class ImGuiLayer : public IGuiLayer {
   public:
    explicit ImGuiLayer(GLFWwindow* window);
    ~ImGuiLayer() override;

    void BeginFrame() override;
    void Render() override;  // default does nothing
    void EndFrame() override;

   private:
    ImGuiContext* m_Ctx = nullptr;
    GLFWwindow* m_Window = nullptr;
};

}  // namespace gen
