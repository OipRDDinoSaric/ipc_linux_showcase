/**
 * @file
 *
 */

#include <cstdint>

namespace Produce
{
class Producer
{
public:
    explicit Producer(int pipeWriteDescriptor);

    static void
    generateTask(Producer& producer);

    static void
    acknowledgeReceiveTask();

private:
    struct Package
    {
        int           id;
        std::uint32_t generatedNum;
    };

    void
    sendToConsumer(Package package) const;

    int pipeWriteDescriptor;
};
}  // namespace Produce