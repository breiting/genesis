#include <genesis/model/Capability.hpp>

namespace gen {

Capability::Capability(std::string name, std::vector<int> layout, std::unique_ptr<IBehavior> behavior)
    : m_Name(std::move(name)),
      m_Brain(std::make_shared<NeuralNetwork>(layout)),
      m_Genome(std::make_unique<Genome>(m_Brain->GetWeightsAndBiases().size())),
      m_Behavior(std::move(behavior)) {
}

void Capability::Randomize() {
    m_Brain->Randomize();
    m_Genome->SetDNA(m_Brain->GetWeightsAndBiases());
}

void Capability::SetFromGenome(const Genome& g) {
    m_Genome = std::make_unique<Genome>(g);
    m_Brain->SetWeightsAndBiases(g.GetDNA());
}

void Capability::SyncToGenome(Genome& g) const {
    g.SetDNA(m_Brain->GetWeightsAndBiases());
}

void Capability::Execute(Agent& agent, float dt) {
    if (!m_Behavior) return;
    m_Behavior->Execute(m_Name, agent, dt);
}

NeuralNetwork& Capability::GetBrain() {
    return *m_Brain;
}
const Genome& Capability::GetGenome() const {
    return *m_Genome;
}
IBehavior& Capability::GetBehavior() {
    return *m_Behavior;
}
const std::string& Capability::GetName() const {
    return m_Name;
}

}  // namespace gen
