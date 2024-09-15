#include <thread>
#include <random>
#include <chrono>

#include <fmt/format.h>

namespace Producer
{
void
generateTask()
{
    constexpr static std::mt19937::result_type lowerBound {0};
    constexpr static std::mt19937::result_type upperBound {100};

    std::random_device device {};
    std::mt19937 rng {device()};
    std::uniform_int_distribution<std::mt19937::result_type> distribution {lowerBound, upperBound};

    for(int iii {0}; iii < 10; iii++)
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

namespace Consumer
{
void
receiveTask()
{}

void
acknowledgeSendTask()
{}
}  // namespace Consumer

int
main(int, char*[])
{
    // Note that jthread is not supported by clang I used.
    std::thread generatorThread {Producer::generateTask};
    std::thread acknowledgeReceiveThread {Producer::acknowledgeReceiveTask};

    std::thread receiveThread {Consumer::receiveTask};
    std::thread acknowledgeSendThread {Consumer::acknowledgeSendTask};

    generatorThread.join();
    acknowledgeReceiveThread.join();

    receiveThread.join();
    acknowledgeSendThread.join();

    return 0;
}