#pragma once

#include <genesis/model/Capability.hpp>
#include <genesis/model/IEmbodiment.hpp>
#include <string>

namespace gen {

/**
 * @brief Cognitive agent that has certain capabilities and can act based on neural policies (brains).
 */
class Agent {
   public:
    explicit Agent(const std::string& id);

    /// Update the agent in the simulation
    void Update(float dt);

    /// Embodiment
    void SetEmbodiment(std::unique_ptr<IEmbodiment> emb);
    IEmbodiment* GetEmbodiment();

    /// Capabilities
    void AddCapability(std::unique_ptr<Capability> capability);
    Capability* GetCapability(const std::string& name);

    /// Get Id
    const std::string& GetId() const;

   private:
    std::string m_Id;
    std::unique_ptr<IEmbodiment> m_Embodiment;
    std::unordered_map<std::string, std::unique_ptr<Capability>> m_Capabilities;
};

}  // namespace gen
