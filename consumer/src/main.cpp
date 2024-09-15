#include <thread>

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
    std::thread receiveThread {Consumer::receiveTask};
    std::thread acknowledgeSendThread {Consumer::acknowledgeSendTask};

    receiveThread.join();
    acknowledgeSendThread.join();

    return 0;
}
