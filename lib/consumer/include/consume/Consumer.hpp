/**
 * @file
 *
 */

#include <cstdint>

namespace Consume
{
class Consumer
{
public:
    explicit Consumer(int pipeReadDescriptor);

    [[noreturn]] static void
    receiveTask(Consumer& consumer);

    static void
    acknowledgeSendTask();

private:
    struct Package
    {
        int           id;
        std::uint32_t generatedNum;
    };

    Package
    receiveGeneratedNumber();

    int pipeReadDescriptor;
};
}  // namespace Consume