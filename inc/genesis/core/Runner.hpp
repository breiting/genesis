#pragma once
#include <genesis/core/AppContext.hpp>
#include <genesis/core/IApp.hpp>
#include <genesis/core/Window.hpp>
#include <memory>
#include <string>

namespace gen {

/** Acts as the controller and orchestrates everything
 */
class Runner {
   private:
    class RunnerContext : public AppContext {
       public:
        RunnerContext(Runner* runner) : m_Runner(runner) {
        }
        double Aspect() const override {
            return m_Runner->Aspect();
        }
        int GetWidth() const override {
            return m_Runner->GetWidth();
        }
        int GetHeight() const override {
            return m_Runner->GetHeight();
        }
        void RequestQuit() override {
            m_Runner->RequestQuit();
        }
        double GetFrameDt() override {
            return m_Runner->GetFrameDt();
        }
        double GetUpdateDt() override {
            return m_Runner->GetUpdateDt();
        }

        Window& GetWindow() override {
            return m_Runner->GetWindow();
        }

       private:
        Runner* m_Runner;
    };

   public:
    Runner() = default;
    ~Runner();

    /** Create window + GL context and take ownership of the app.
     * @param app The application to run.
     * @param width Window width.
     * @param height Window height.
     * @param title Window title.
     * @param msaa MSAA samples.
     * @throws std::runtime_error if initialization fails.
     */
    void Init(std::unique_ptr<IApp> app, int width, int height, const std::string& title, int msaa = 4);

    /** Run the application loop. */
    void Run();

    /** Set the update rate in Hz.
     * @param rate Update rate.
     */
    void SetUpdateRate(double rate);  // in Hz (e.g. 60)

   private:
    gen::Window m_Window;
    std::unique_ptr<IApp> m_App;
    RunnerContext m_Context{this};

    double m_UpdateRate{60.0};
    double m_FrameDt{0.0};
    double m_UpdateDt{0.0};

    bool m_Quit{false};

    double Aspect() const;
    int GetWidth() const;
    int GetHeight() const;
    void RequestQuit();
    double GetFrameDt();
    double GetUpdateDt();
    Window& GetWindow();

    void BeginFrame();
    void EndFrame();
    bool Poll();
};

}  // namespace gen
