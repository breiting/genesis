#include <genesis/core/Window.hpp>
#include <iostream>
#include <stdexcept>

#include "genesis/core/Colors.hpp"

namespace gen {

bool Window::Create(const CreateInfo& ci) {
    if (!glfwInit()) return false;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, ci.msaa);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    m_Window = glfwCreateWindow(ci.width, ci.height, ci.title.c_str(), nullptr, nullptr);
    if (!m_Window) {
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(m_Window);
    if (!gladLoadGL((GLADloadfunc)glfwGetProcAddress)) {
        throw std::runtime_error("Failed to initialize GLAD");
    }

    glfwSwapInterval(1);
    glEnable(GL_MULTISAMPLE);

    glfwGetFramebufferSize(m_Window, &m_FramebufferWidth, &m_FramebufferHeight);
    glViewport(0, 0, m_FramebufferWidth, m_FramebufferHeight);

    InitGlfwCallbacks();

    std::cout << "GL Version:  " << glGetString(GL_VERSION) << std::endl;
    std::cout << "GL Renderer: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "GLSL:        " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

    return true;
}

void Window::Destroy() {
    if (m_Window) {
        glfwDestroyWindow(m_Window);
        m_Window = nullptr;
    }
    glfwTerminate();
}

void Window::InitGlfwCallbacks() {
    glfwSetWindowUserPointer(m_Window, this);

    glfwSetKeyCallback(m_Window, [](GLFWwindow* w, int key, int /*sc*/, int action, int mods) {
        auto* self = static_cast<Window*>(glfwGetWindowUserPointer(w));
        if (!self || !self->m_KeyPressedCallback || action != GLFW_PRESS) return;
        self->m_KeyPressedCallback(key, mods);
    });
    glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* w, int button, int action, int mods) {
        auto* self = static_cast<Window*>(glfwGetWindowUserPointer(w));
        if (!self || !self->m_MouseButtonCallback) return;
        self->m_MouseButtonCallback(button, action, mods);
    });

    glfwSetScrollCallback(m_Window, [](GLFWwindow* w, double xoff, double yoff) {
        auto* self = static_cast<Window*>(glfwGetWindowUserPointer(w));
        if (!self || !self->m_ScrollCallback) return;
        self->m_ScrollCallback(xoff, yoff);
    });

    glfwSetCursorPosCallback(m_Window, [](GLFWwindow* w, double x, double y) {
        auto* self = static_cast<Window*>(glfwGetWindowUserPointer(w));
        if (!self || !self->m_MouseMoveCallback) return;
        self->m_MouseMoveCallback(x, y);
    });

    glfwSetFramebufferSizeCallback(m_Window, [](GLFWwindow* w, int width, int height) {
        auto* self = static_cast<Window*>(glfwGetWindowUserPointer(w));
        glViewport(0, 0, width, height);

        if (!self || !self->m_WindowSizeCallback) return;
        self->m_WindowSizeCallback(width, height);
    });
}

bool Window::Poll() {
    if (!m_Window) return false;
    if (glfwWindowShouldClose(m_Window)) return false;

    glfwPollEvents();
    glfwGetFramebufferSize(m_Window, &m_FramebufferWidth, &m_FramebufferHeight);
    glViewport(0, 0, m_FramebufferWidth, m_FramebufferHeight);
    return true;
}

float Window::Aspect() const {
    return float(m_FramebufferWidth) / float(m_FramebufferHeight);
}

void Window::BeginFrame() {
    auto bg = Colors::Nord0;
    glClearColor(bg.r, bg.g, bg.b, bg.a);
    glClear(GL_COLOR_BUFFER_BIT);
}

void Window::EndFrame() {
    glfwSwapBuffers(m_Window);
}

}  // namespace gen
