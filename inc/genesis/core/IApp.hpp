#pragma once
#include <functional>
#include <string>

namespace gen {

class AppContext;

/** Minimal application interface.
 */
class IApp {
   public:
    virtual ~IApp() = default;

    /** @return The name of the application. */
    virtual std::string Name() const = 0;

    /** Initialize the application.
     * @param ctx The application context.
     * @return true if initialization succeeded.
     */
    virtual bool Init(AppContext& ctx) = 0;

    /** Update the application state.
     * @param ctx The application context.
     * @param dt Time delta since last update.
     */
    virtual void Update(AppContext& ctx, double dt) = 0;

    /** Render the application.
     * @param ctx The application context.
     */
    virtual void Render(AppContext& ctx) = 0;

    // Optional input handlers
    std::function<void(int, int)> onKeyPressed;
    std::function<void(int, int, int)> onMouseButton;
    std::function<void(double, double)> onMouseMove;
    std::function<void(double, double)> onScroll;
    std::function<void(double, double)> onWindowSize;
};

}  // namespace gen
