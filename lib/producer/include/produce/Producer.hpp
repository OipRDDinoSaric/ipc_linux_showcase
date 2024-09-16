/**
 * @file
 *
 */

#include <random>
#include <chrono>

#include <cstdint>

namespace Produce
{

/**
 * @brief Implements generating numbers in one thread and receiving acknowledgements in another.
 */
class Producer
{
public:
    Producer(int toConsumerWriteDesc, int fromAcknowledgeReadDesc);

    static void
    generateTask(Producer& producer);

    [[noreturn]] static void
    acknowledgeReceiveTask(Producer& producer);

private:
    constexpr static std::mt19937::result_type kRngLowerBound {0};
    constexpr static std::mt19937::result_type kRngUpperBound {100};
    constexpr static int                       kNumOfGeneratedNums {10};
    constexpr static auto kGenerateTaskLoopDelay {std::chrono::milliseconds {200}};

    struct Package
    {
        int           id;
        std::uint32_t generatedNum;
    };

    void
    sendToConsumer(Package package) const;

    void
    generateImpl();
    void
    acknowledgeReceiveLoop() const;

    int toConsumerWriteDesc;
    int fromAcknowledgeReadDesc;
};
}  // namespace Produce