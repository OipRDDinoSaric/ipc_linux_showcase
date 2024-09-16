/**
 * @file
 *
 */

#include <produce/Producer.hpp>

#include <random>
#include <chrono>
#include <thread>

#include <fmt/format.h>

#include <unistd.h>

namespace Produce
{

Producer::Producer(int pipeWriteDescriptor, int fromAcknowledgeReadDesc) :
        pipeWriteDescriptor {pipeWriteDescriptor},
        fromAcknowledgeReadDesc {fromAcknowledgeReadDesc}
{}

void
Producer::generateTask(Producer& producer)
{
    producer.generateImpl();
}

[[noreturn]] void
Producer::acknowledgeReceiveTask(Producer& producer)
{
    while (true)
    {
        producer.acknowledgeReceiveLoop();
    }
}

void
Producer::sendToConsumer(Package package) const
{
    ssize_t errorCode {write(pipeWriteDescriptor, &package, sizeof(package))};

    if (sizeof(package) != errorCode)
    {
        if (-1 == errorCode)
        {
            fmt::println(stderr, "Error while writing to the pipe: {}", strerror(errno));
        }
        else
        {
            fmt::println(stderr, "Error while writing to the pipe: Size mismatch.");
        }
        exit(1);
    }
}

void
Producer::generateImpl()
{
    using std::chrono_literals::operator""ms;

    constexpr static std::mt19937::result_type kLowerBound {0};
    constexpr static std::mt19937::result_type kUpperBound {100};
    constexpr static int                       kNumOfGeneratedNums {10};
    constexpr static auto                      kTaskLoopDelay {200ms};

    std::random_device                                       device {};
    std::mt19937                                             rng {device()};
    std::uniform_int_distribution<std::mt19937::result_type> distribution {kLowerBound,
                                                                           kUpperBound};

    for (int iii {0}; iii < kNumOfGeneratedNums; iii++)
    {
        std::mt19937::result_type generatedNumber {distribution(rng)};
        fmt::println("Producer: ID {} = number {}.", iii, generatedNumber);

        sendToConsumer({iii, static_cast<std::uint32_t>(generatedNumber)});

        std::this_thread::sleep_for(kTaskLoopDelay);
    }
}

void
Producer::acknowledgeReceiveLoop() const
{
    int id {};

    ssize_t errorCode {read(fromAcknowledgeReadDesc, &id, sizeof(id))};

    if (sizeof(id) != errorCode)
    {
        if (-1 == errorCode)
        {
            fmt::println(stderr, "Error while reading from the pipe: {}", strerror(errno));
        }
        else
        {
            fmt::println(stderr, "Error while reading from the pipe: Size mismatch.");
        }
        exit(1);
    }

    fmt::println("Producer: Acknowledged ID {}. ", id);
}
}  // namespace Produce
