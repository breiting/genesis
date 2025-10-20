#pragma once

namespace gen {

class Window;

/** Simple interface which allows the app to interact with other components.
 * The runner typically implements this class.
 */
class AppContext {
   public:
    virtual ~AppContext() = default;

    /** @return The aspect ratio of the window. */
    virtual double Aspect() const = 0;

    /** @return The window width in pixels. */
    virtual int GetWidth() const = 0;

    /** @return The window height in pixels. */
    virtual int GetHeight() const = 0;

    /** Request the application to quit. */
    virtual void RequestQuit() = 0;

    /** @return The time delta since the last frame. */
    virtual double GetFrameDt() = 0;

    /** @return The time delta since the last update. */
    virtual double GetUpdateDt() = 0;

    /** @return The window */
    virtual Window &GetWindow() = 0;
};

}  // namespace gen
