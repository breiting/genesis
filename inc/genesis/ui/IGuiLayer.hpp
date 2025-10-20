#pragma once

namespace gen {

class IGuiLayer {
   public:
    virtual ~IGuiLayer() = default;

    virtual void BeginFrame() = 0;
    virtual void Render() = 0;  // user-defined panels
    virtual void EndFrame() = 0;
};

}  // namespace gen
