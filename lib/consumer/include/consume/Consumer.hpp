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

    [[noreturn]] static void
    acknowledgeSendTask(Consumer& consumer);

private:
    struct Package
    {
        int           id;
        std::uint32_t generatedNum;
    };

    [[nodiscard]] Package
    receiveGeneratedNumber() const;

    void sendToAcknowledgeSend(int id);

    int           pipeReadDescriptor;
    std::uint64_t sum;

    std::condition_variable cvReceiveToAcknowledge;
    std::mutex              mtxReceiveToAcknowledge;
    int                     idReceiveToAcknowledge;
    bool                    hasNewMessageForAcknowledge;
};
}  // namespace Consume