/**
 * @file
 *
 */

#include <produce/Producer.hpp>

#include <thread>

#include <fmt/format.h>

#include <unistd.h>

namespace Produce
{

/**
 * @brief Construct Producer.
 *
 * @param[in] toConsumerWriteDesc     Pipe descriptor to send generated numbers to.
 * @param[in] fromAcknowledgeReadDesc Pipe descriptor to receive acknowledgements.
 */
Producer::Producer(int toConsumerWriteDesc, int fromAcknowledgeReadDesc) :
        toConsumerWriteDesc {toConsumerWriteDesc},
        fromAcknowledgeReadDesc {fromAcknowledgeReadDesc}
{}

/**
 * @brief Task that generates numbers periodically. Use with thread.
 *
 * @param[in] producer Producer context.
 */
void
Producer::generateTask(Producer& producer)
{
    producer.generateImpl();
}

/**
 * @brief Task that loops infinitely expecting message of acknowledgement. Use with thread.
 *
 * @param[in] producer Producer context.
 */
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
    ssize_t errorCode {write(toConsumerWriteDesc, &package, sizeof(package))};

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
    std::random_device                                       device {};
    std::mt19937                                             rng {device()};
    std::uniform_int_distribution<std::mt19937::result_type> distribution {kRngLowerBound,
                                                                           kRngUpperBound};

    for (int iii {0}; iii < kNumOfGeneratedNums; iii++)
    {
        std::mt19937::result_type generatedNumber {distribution(rng)};
        fmt::println("Producer: ID {} = number {}.", iii, generatedNumber);

        sendToConsumer({iii, static_cast<std::uint32_t>(generatedNumber)});

        std::this_thread::sleep_for(kGenerateTaskLoopDelay);
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
