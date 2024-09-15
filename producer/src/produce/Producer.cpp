/**
 * @file
 *
 */

#include <produce/Producer.hpp>

#include <random>
#include <chrono>
#include <thread>

#include <fmt/format.h>

namespace Produce
{
void
Producer::generateTask()
{
    constexpr static std::mt19937::result_type kLowerBound {0};
    constexpr static std::mt19937::result_type kUpperBound {100};
    constexpr static int                       kNumOfGeneratedNums {10};

    std::random_device                                       device {};
    std::mt19937                                             rng {device()};
    std::uniform_int_distribution<std::mt19937::result_type> distribution {kLowerBound,
                                                                           kUpperBound};

    for (int iii {0}; iii < kNumOfGeneratedNums; iii++)
    {
        using std::chrono_literals::operator""ms;

        std::mt19937::result_type generatedNumber {distribution(rng)};
        fmt::println("Generated number {}: {}", iii, generatedNumber);

        std::this_thread::sleep_for(500ms);
    }
}

void
Producer::acknowledgeReceiveTask()
{}
}  // namespace Produce