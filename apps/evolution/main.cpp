#include <genesis/core/Math.hpp>
#include <genesis/model/Agent.hpp>
#include <genesis/model/Genome.hpp>
#include <genesis/model/MovementBehavior.hpp>
#include <genesis/model/NeuralNetwork.hpp>
#include <genesis/model/SimpleEmbodiment.hpp>
#include <iostream>

using namespace gen;
using namespace std;

int main() {
    std::vector<int> layout = {4, 8, 2};
    auto nnTemplate = std::make_shared<NeuralNetwork>(layout);
    size_t weightCount = nnTemplate->GetWeightsAndBiases().size();
    cout << weightCount << endl;

    Agent agent("0");

    auto embodiment = std::make_unique<SimpleEmbodiment>(glm::vec2(5.0, 6.0));
    agent.SetEmbodiment(std::move(embodiment));

    std::vector<int> layer{2, 4, 2};
    auto cap = std::make_unique<Capability>("movement", layer, std::make_unique<MovementBehavior>(glm::vec2(0.0, 0.0)));
    agent.AddCapability(std::move(cap));

    for (int i = 0; i < 100; i++) {
        agent.Update(i / 60.0);
        auto pos = agent.GetEmbodiment()->GetPosition();
        cout << "Position: " << pos.x << " " << pos.y << endl;
    }

    return 0;
}
