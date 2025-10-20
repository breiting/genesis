#include <gtest/gtest.h>

#include <genesis/model/Agent.hpp>
#include <genesis/model/MovementBehavior.hpp>
#include <genesis/model/SimpleEmbodiment.hpp>
#include <memory>

using namespace gen;

TEST(TestAgent, Create) {
    Agent agent("007");
    ASSERT_EQ(agent.GetId(), "007");
}

TEST(TestAgent, Embodiment) {
    Agent agent("007");
    auto embodiment = std::make_unique<SimpleEmbodiment>(glm::vec2(5.0, 5.0));
    agent.SetEmbodiment(std::move(embodiment));

    auto ptr = agent.GetEmbodiment();

    ASSERT_NE(ptr, nullptr);
    EXPECT_FLOAT_EQ(ptr->GetPosition().x, 5.0);
}

TEST(TestAgent, Capability) {
    Agent agent("0");
    std::vector<int> layer{2, 4, 2};
    auto cap = std::make_unique<Capability>("movement", layer, std::make_unique<MovementBehavior>(glm::vec2(0.0, 0.0)));
    agent.AddCapability(std::move(cap));

    auto ptr = agent.GetCapability("movement");

    ASSERT_NE(ptr, nullptr);
    EXPECT_EQ(ptr->GetBrain().GetBiasCount(), 6);
}
