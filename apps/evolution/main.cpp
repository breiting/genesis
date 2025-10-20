#include <genesis/core/Math.hpp>
#include <genesis/model/Agent.hpp>
#include <genesis/model/Genome.hpp>
#include <genesis/model/NeuralNetwork.hpp>
#include <iostream>

using namespace gen;
using namespace std;

int main() {
    std::vector<int> layout = {4, 8, 2};
    auto nnTemplate = std::make_shared<NeuralNetwork>(layout);
    size_t weightCount = nnTemplate->GetWeightsAndBiases().size();
    cout << weightCount << endl;

    Agent agent("0");

    return 0;
}
