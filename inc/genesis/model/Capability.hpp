#pragma once
#include <genesis/model/Genome.hpp>
#include <genesis/model/IBehavior.hpp>
#include <genesis/model/NeuralNetwork.hpp>
#include <memory>
#include <string>

namespace gen {

/**
 * @brief Evolvable capability representing a learned functional module.
 */
class Capability {
   public:
    Capability(std::string name, std::vector<int> layout, std::unique_ptr<IBehavior> behavior);

    void Randomize();
    void SetFromGenome(const Genome& g);
    void SyncToGenome(Genome& g) const;

    void Execute(class Agent& agent, float dt);

    NeuralNetwork& GetBrain();
    const Genome& GetGenome() const;
    IBehavior& GetBehavior();
    const std::string& GetName() const;

   private:
    std::string m_Name;
    std::shared_ptr<NeuralNetwork> m_Brain;
    std::unique_ptr<Genome> m_Genome;
    std::unique_ptr<IBehavior> m_Behavior;
};

}  // namespace gen
