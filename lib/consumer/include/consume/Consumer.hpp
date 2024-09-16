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
    Consumer(int fromGeneratorReadDesc, int toProducerWriteDesc);

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

    void
    sendToAcknowledgeSend(int id);

    void
    receiveLoop();
    void
    acknowledgeSendLoop();

    int fromGeneratorReadDesc;
    int toProducerWriteDesc;

    std::uint64_t sum;

    std::condition_variable cvReceiveToAcknowledge;
    std::mutex              mtxReceiveToAcknowledge;
    int                     idReceiveToAcknowledge;
    bool                    hasNewMessageForAcknowledge;
};
}  // namespace Consume