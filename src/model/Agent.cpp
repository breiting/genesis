#include <genesis/model/Agent.hpp>

namespace gen {

Agent::Agent(const std::string& id) : m_Id(id) {
}

const std::string& Agent::GetId() const {
    return m_Id;
}

void Agent::Update(float /*dt*/) {
}

void Agent::SetEmbodiment(std::unique_ptr<IEmbodiment> emb) {
    m_Embodiment = std::move(emb);
}

IEmbodiment* Agent::GetEmbodiment() {
    if (m_Embodiment) return m_Embodiment.get();
    return nullptr;
}

void Agent::AddCapability(std::unique_ptr<Capability> capability) {
    m_Capabilities[capability->GetName()] = std::move(capability);
}

Capability* Agent::GetCapability(const std::string& name) {
    auto it = m_Capabilities.find(name);
    return it != m_Capabilities.end() ? it->second.get() : nullptr;
}

}  // namespace gen
