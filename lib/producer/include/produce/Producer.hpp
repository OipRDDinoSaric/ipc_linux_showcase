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
    Producer(int pipeWriteDescriptor, int fromAcknowledgeReadDesc);

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

    int pipeWriteDescriptor;
    int fromAcknowledgeReadDesc;
};
}  // namespace Produce