# 🧬 genesis

_A Lightweight C++ NeuroEvolution Framework for Intelligent Agents_

## Overview

**genesis** is a modular, C++ framework designed for building and evolving artificial neural agents using NeuroEvolution — the combination of neural networks and genetic algorithms.

The goal of **genesis** is to provide a clear, efficient, and extensible architecture for experimentation with autonomous agents that learn through evolution rather than gradient-based training.

**genesis** emphasizes:

- 🧠 Clarity — Simple, transparent code built on solid OOP design principles (SOLID).
- ⚙️ Efficiency — Flat memory layout and cache-friendly data structures.
- 🔬 Extensibility — Easy to plug in custom environments, activation functions, and evolution strategies.

## What is NeuroEvolution?

NeuroEvolution is the process of optimizing neural networks using evolutionary algorithms rather than back-propagation.
It treats each network’s parameters (weights, biases, or even structure) as genes in a genetic representation (the genome).

During evolution:

1. A population of neural networks is created.
2. Each network’s performance is evaluated in a specific environment (its fitness).
3. The fittest networks are selected and recombined via crossover and mutation to form the next generation.
4. Over many generations, networks evolve behaviors that solve the given task.

This approach is powerful for non-differentiable, discontinuous, or sparse-reward problems — such as navigation, control, or decision-making agents — where gradient descent fails.


## 🧭 Understanding the Search Space in NeuroEvolution

In NeuroEvolution, the search space represents the set of all possible neural network configurations — every conceivable combination of weights, biases, and potentially other structural or behavioral parameters that define an agent’s behavior.

Each genome in **genesis** encodes a single point in this multidimensional space. The Population forms a cloud of such points, exploring different regions through genetic operators like mutation and crossover. The fitness function then defines a landscape over this space — a surface where higher elevations correspond to better-performing individuals.

Rather than following gradients (as in back-propagation), NeuroEvolution performs a stochastic search across this landscape. It does not require differentiability, making it especially powerful for non-linear, discontinuous, or simulation-based problems where no analytical gradient exists.

Formally:

$\Omega = \{ \theta \in \mathbb{R}^n \mid \theta \text{ represents all evolvable parameters of the agent} \}$

In practice, this means that anything you can represent as a vector of numbers can be evolved:

- Neural weights and biases
- Activation parameters
- Network topology and connectivity
- Control variables or sensor mappings
- Even physical properties in a simulated agent (e.g., morphology, sensors, or motor strengths)

In **genesis**, the `Genome` defines the coordinates of a solution in this vast search space, the `Population` evolves its distribution over time, and the `Evaluator` shapes the landscape by assigning fitness. Together, these components turn **genesis** into a search engine for emergent intelligence — capable of discovering complex behaviors without explicit instruction or gradients.

## Project Architecture

Core concepts:

- NeuralNetwork – deterministic forward-only inference (no back-propagation).
- Genome – flat vector of floats representing weights, biases, and future genes.
- Population – evolutionary manager performing elitism, crossover, and mutation.
- Evaluator – computes fitness for each genome (e.g., XOR, Maze, or Agent simulation).

### Example: XOR Evolution

**genesis** ships with a simple example where NeuroEvolution learns the XOR logical function.

```cpp
gr::NeuralNetwork nn({2, 4, 1});
gr::Population pop(50, nn.GetWeightsAndBiases().size());
gr::XorEvaluator eval(std::make_shared<gr::NeuralNetwork>(nn));

for (int gen = 0; gen < 100; ++gen) {
    float best = 0.0f, total = 0.0f;

    for (auto& g : pop.GetGenomes()) {
        float fitness = eval.Evaluate(g);
        g.SetFitness(fitness);
        total += fitness;
        best = std::max(best, fitness);
    }

    std::cout << "Gen " << gen
              << " | Best: " << best
              << " | Avg: "  << total / pop.Size() << "\n";

    pop.Evolve(0.1f, 0.3f, 0.1f);
}
```

Over generations, the population converges toward a network that correctly computes XOR.

## Future Directions

**genesis** is built to serve as a foundation for:

- 🧭 Autonomous agent environments — maze navigation, obstacle avoidance, swarm behavior.
- 🔄 Dynamic topologies — adding/removing neurons or connections during evolution (NEAT-style).
- 🧠 Hybrid learning — combining NeuroEvolution with gradient fine-tuning.
- 📊 Visualization tools — real-time rendering of network topology and evolution progress.

## Getting Started

### Requirements

- C++17 compatible compiler (GCC 9+, Clang 10+, MSVC 2019+)
- CMake (optional, for builds)

### 🚀 Build & Run

```bash
git clone https://github.com/breiting/genesis.git
cd genesis
mkdir build && cd build
cmake ..
make
```

## 📜 License

MIT — free to use, learn, and build upon.

## Scientific References

Here are some interesting research papers.

- Stanley, K. O., & Miikkulainen, R. (2002). _Evolving Neural Networks through Augmenting Topologies (NEAT)_. Evolutionary Computation, 10(2), 99–127.
- Stanley, K. O., Clune, J., Lehman, J., & Miikkulainen, R. (2019). _Designing neural networks through neuroevolution_. Nature Machine Intelligence, 1, 24–35.
- Yao, X. (1999). _Evolving Artificial Neural Networks_. Proceedings of the IEEE, 87(9), 1423–1447.
- Floreano, D., Dürr, P., & Mattiussi, C. (2008). _Neuroevolution: From architectures to learning_. Evolutionary Intelligence, 1(1), 47–62.
- Lehman, J., & Stanley, K. O. (2011). _Abandoning objectives: Evolution through the search for novelty alone_. Evolutionary Computation, 19(2), 189–223.

## Citation

If you use **genesis** in research, please cite it as:

```
@software{genesis2025,
  author = {Bernhard Reitinger},
  title = {genesis: A Lightweight C++ Framework for NeuroEvolution},
  year = {2025},
  license = {MIT},
  url = {https://github.com/breiting/genesis}
}
```

## Author

**genesis** is created and maintained by Bernhard Reitinger (@breiting).
Contributions, discussions, and pull requests are welcome!

## 🙏 Credits

**genesis** uses the following libraries and resources.

- [GLM](https://github.com/g-truc/glm)
- [GLFW](https://github.com/glfw/glfw)
- [ImGui](https://github.com/ocornut/imgui)
- [oneTBB](https://github.com/uxlfoundation/oneTBB)
- [stb_image](https://github.com/nothings/stb)
- [Roboto](https://fonts.google.com/specimen/Roboto) font ([Apache License, Version 2.0](https://www.apache.org/licenses/LICENSE-2.0))
