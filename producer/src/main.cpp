#include <thread>
#include <random>
#include <chrono>

#include <fmt/format.h>

namespace Producer
{
void
generateTask()
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
acknowledgeReceiveTask()
{}
}  // namespace Producer

int
main(int, char*[])
{
    fmt::println("Producer task started.");

    // Note that jthread is not supported by clang I used.
    std::thread generatorThread {Producer::generateTask};
    std::thread acknowledgeReceiveThread {Producer::acknowledgeReceiveTask};

    generatorThread.join();
    acknowledgeReceiveThread.join();

    return 0;
}