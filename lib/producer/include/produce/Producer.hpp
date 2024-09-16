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
    void acknowledgeReceiveLoop();

    int pipeWriteDescriptor;
};
}  // namespace Produce