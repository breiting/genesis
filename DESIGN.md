# 🧬 Genesis Framework — Core Architecture Overview

Genesis is a modular, extensible simulation framework written in modern C++17, designed to explore emergent behavior, neuroevolution, and adaptive intelligence. It provides a clean, engine-style architecture for building, visualizing, and evolving complex systems—from simple particles to cognitive agents.

## 🌐 Architectural Philosophy

Genesis treats everything as a system of interacting agents. Each `Agent` is an autonomous entity that can sense, decide, and act within a world. Its decision logic is encapsulated in **Capabilities**—modular units that connect neural networks, genomes, and behaviors.

At its heart, Genesis unifies three domains:
1. **Simulation** (physics, world, embodiment)
2. **Evolution** (genetic algorithms, fitness evaluation)
3. **Cognition** (neural computation, adaptive behaviors)

This enables modeling emergence, intelligence, and adaptation in a visually rich, physically bounded environment.

## 🧩 Core Components

### 1. Agent System

- **Agent**: The most general base type, representing anything that can exist in a simulation (physical or abstract). It may have components like position, state, memory, or traits. It can have:
  - Own one or more Capabilities (functional modules).
  - Can have an Embodiment (spatial body, velocity, etc.).
  - Be evaluated and evolved.
  - Execute behaviors each frame via `Agent::Update(dt)`.

Agents are designed to be generic—they don’t require a position (e.g., for virtual or cyber agents), but an Embodiment can be plugged in for spatial simulations.

### 2. Embodiment Layer

- **IEmbodiment**: Interface defining position, velocity, and physical properties.
- **SimpleEmbodiment**: Default implementation for movement-based agents, providing simple 2D physics (position and velocity integration).

This separation allows the same agent logic to be reused in:
- Physical worlds (e.g., movement, pathfinding, foraging).
- Abstract spaces (e.g., financial markets, cyber defense).

### 3. Capabilities/Behaviors

A Capability represents a specific skill or competence of an agent, such as "movement," "communication," or "prediction."

Each Capability internally connects:
- A **Genome** (genetic encoding of parameters).
- A **Neural Network** (for inference).
- A **Behavior Module** (logic that translates neural network outputs into actions).

```
Agent
└── Capability("movement")
    ├── Genome         // Encoded weights and biases
    ├── NeuralNetwork  // Feed-forward inference
    └── Behavior       // Interprets NN outputs (e.g., move toward goal)
```

This "Genome–Network–Behavior" triplet defines a self-contained learning and execution unit. Agents can have multiple Capabilities, trained individually or jointly.

### 4. Evolution System

Genesis includes a lightweight evolutionary trainer:
- **Trainer**: Runs generations of agents through an environment, evaluates fitness, applies selection/mutation (and optionally crossover), and replaces low-performing genomes.
- **IFitnessEvaluator**: Abstract interface defining fitness computation. Examples:
  - **MovementEvaluator**: Measures distance to a goal.
  - **RegressionEvaluator**: Measures prediction accuracy (MSE).
  - Custom evaluators: E.g., energy efficiency, survival time, cooperation index.

Fitness is stored per agent during training and used for ranking and selection.

### 5. Neural Network Core

- **NeuralNetwork**: Compact feed-forward model with configurable layers, supporting multiple activation functions (Tanh, Sigmoid, ReLU, Linear).
  - Methods: `Forward(inputs)` for inference, `GetWeightsAndBiases()`/`SetWeightsAndBiases()` for evolutionary mutation, `Randomize()` for initial DNA generation.
- **Genome**: Serializable encoding of all weights and biases. Can be saved, loaded, mutated, or applied to other networks.

Together, these form the "neural substrate" that drives agent behaviors.

### 6. Simulation Modules

All simulations derive from a common interface:

```cpp
class ISimModule {
public:
    virtual std::string Name() const = 0;
    virtual void OnAttach() {}
    virtual void OnDetach() {}
    virtual void Update(double dt) = 0;
    virtual void RenderWorld(const glm::mat4& vp) = 0;
    virtual void RenderImGui() = 0;
};
```

This modular design allows plugging in simulations like:
- **NeuroEvolutionSim**: Agents evolve to reach a target.
- **BrownianSim**: Stochastic particle motion.
- **RegressionSim**: Data-driven evolution for regression tasks.

Each module runs independently, using shared rendering, agents, and trainers.

### 7. Visualization Layer

Genesis features a clean OpenGL rendering system built around Views:
- **AgentView**: Efficient batched rendering of agents.
- **TargetView**: Draws goal points (circles).
- **CanvasView**: Background world panel (100×100 world units).
- **EnvironmentView**: Renders bounds, shadows, rounded corners, etc.
- **Camera2D**: Smooth zoom, pan, fit-to-world, aspect correction.

The rendering pipeline uses modern OpenGL (GL 4.1 Core, GLFW + GLAD + GLSL 330/410) with a "soft dark editor aesthetic"—minimal and professional.

### 8. User Interface

A fixed left-side ImGui control panel provides:
- Simulation control (Init, Train, Observe, Reset).
- Hyperparameter tuning (mutation, elitism, speed, population).
- Visualization of stats (fitness, generations, plots).

The world, agents, and environment are rendered in a clean right-side Canvas Viewport, separated via `glViewport` and `glScissor`.

## 🚀 Example Workflow

1. **Create Agents**:
   ```cpp
   CreateAgent(startPos, targetPos);
   ```

2. **Train with Evolution**:
   ```cpp
   Trainer trainer(m_Agents, std::make_unique<MovementEvaluator>(target), "movement");
   trainer.RunGeneration(dt, steps);
   ```

3. **Observe Learned Behavior**:
   ```cpp
   for (auto& agent : m_Agents) {
       agent->Update(dt);
   }
   ```

4. **Save Best Genome**:
   ```cpp
   bestGenome.SaveBinary("best.dna");
   ```

5. **Reload and Apply**:
   ```cpp
   cap->SetFromGenome(bestGenome);
   ```

## 💡 Design Principles

- **SOLID Architecture**: Small, composable, single-responsibility classes.
- **Data-Driven**: Everything can be configured or evolved dynamically.
- **Modular Expansion**: Each simulation is an independent module.
- **Visual-First**: Everything that learns can be seen and understood.
- **Emergence over Control**: Observe what arises, not what’s imposed.

## 🧠 Vision

Genesis is more than an engine—it's a sandbox for discovery. It aims to answer fundamental questions like: "How do intelligence, order, and complexity emerge from simple rules?"

It can expand to cover:
- Multi-agent evolution.
- Ecosystem dynamics.
- Cognitive emergence.
- Pattern formation (Turing, Lenia, Cellular Automata).
- Data-driven intelligence (Regression, Prediction, Decision AI).
