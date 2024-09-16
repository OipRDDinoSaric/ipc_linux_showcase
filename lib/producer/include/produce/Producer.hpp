/**
 * @file
 *
 */

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
    struct Package
    {
        int           id;
        std::uint32_t generatedNum;
    };

    void
    sendToConsumer(Package package) const;

    void generateImpl();
    void acknowledgeReceiveLoop() const;

    int toConsumerWriteDesc;
    int fromAcknowledgeReadDesc;
};
}  // namespace Produce