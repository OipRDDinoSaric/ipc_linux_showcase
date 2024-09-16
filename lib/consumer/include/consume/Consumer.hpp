/**
 * @file
 *
 */

#include <cstdint>
#include <condition_variable>

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

    [[nodiscard]] Package
    receiveGeneratedNumber() const;

    int                     pipeReadDescriptor;
    std::condition_variable cvReceiveToAcknowledge;
    std::mutex              mtxReceiveToAcknowledge;
    std::uint64_t           sum;
};
}  // namespace Consume