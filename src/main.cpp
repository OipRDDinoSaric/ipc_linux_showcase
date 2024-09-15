#include <thread>
#include <random>
#include <chrono>

#include <fmt/format.h>

namespace Producer
{
void
generateTask()
{}

void
acknowledgeReceiveTask()
{}
}  // namespace Producer

namespace Consumer
{
void
receiveTask()
{}

void
acknowledgeSendTask()
{}
}  // namespace Consumer

int
main(int, char*[])
{
    // Note that jthread is not supported by clang I used.
    std::thread generatorThread {Producer::generateTask};
    std::thread acknowledgeReceiveThread {Producer::acknowledgeReceiveTask};

    std::thread receiveThread {Consumer::receiveTask};
    std::thread acknowledgeSendThread {Consumer::acknowledgeSendTask};

    generatorThread.join();
    acknowledgeReceiveThread.join();

    receiveThread.join();
    acknowledgeSendThread.join();

    return 0;
}