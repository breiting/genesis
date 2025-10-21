#include <genesis/core/Math.hpp>
#include <genesis/core/Runner.hpp>
#include <genesis/model/Agent.hpp>
#include <genesis/model/Genome.hpp>
#include <genesis/model/MovementBehavior.hpp>
#include <genesis/model/NeuralNetwork.hpp>
#include <genesis/model/SimpleEmbodiment.hpp>
#include <iostream>

#include "EvolutionApp.hpp"

using namespace gen;
using namespace std;

int main() {
    auto app = std::make_unique<EvolutionApp>();
    Runner runner;
    try {
        runner.Init(std::move(app), 1280, 720, "Genesis Simulation", 4);
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }
    runner.Run();
    return 0;
}
