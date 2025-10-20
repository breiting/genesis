#pragma once
// clang-format off
#include <glad.h>
#include <GLFW/glfw3.h>
// clang-format on

#include <functional>
#include <string>

namespace gen {

/** Thin GLFW window wrapper with basic frame control. */
class Window {
   public:
    struct CreateInfo {
        int width = 1200;
        int height = 800;
        std::string title = "genesis";
        int msaa = 4;
    };

    bool Create(const CreateInfo& ci);
    void Destroy();

    bool Poll();        // returns false if window should close
    void BeginFrame();  // clears
    void EndFrame();    // swap buffers

    float Aspect() const;
    int GetWidth() const {
        return m_FramebufferWidth;
    }
    int GetHeight() const {
        return m_FramebufferHeight;
    }

    void SetKeyPressedCallback(std::function<void(int, int)> cb) {
        m_KeyPressedCallback = cb;
    }

    void SetMouseButtonCallback(std::function<void(int, int, int)> cb) {
        m_MouseButtonCallback = cb;
    }

    void SetMouseMoveCallback(std::function<void(double, double)> cb) {
        m_MouseMoveCallback = cb;
    }

    void SetWindowSizeCallback(std::function<void(int, int)> cb) {
        m_WindowSizeCallback = cb;
    }

    void SetScrollCallback(std::function<void(double, double)> cb) {
        m_ScrollCallback = cb;
    }

    GLFWwindow* GetNativeWindow() {
        return m_Window;
    }

   private:
    void InitGlfwCallbacks();

   private:
    GLFWwindow* m_Window;

    // Callbacks
    std::function<void(int, int)> m_KeyPressedCallback;
    std::function<void(int, int, int)> m_MouseButtonCallback;
    std::function<void(double, double)> m_MouseMoveCallback;
    std::function<void(double, double)> m_ScrollCallback;
    std::function<void(int, int)> m_WindowSizeCallback;

    int m_FramebufferWidth;
    int m_FramebufferHeight;
};

}  // namespace gen
