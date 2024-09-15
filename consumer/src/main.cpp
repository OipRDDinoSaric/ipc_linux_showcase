#include <thread>

#include <consume/Consumer.hpp>

#include <fmt/format.h>

int
main(int, char*[])
{
    fmt::println("Consumer started.");

    // Note that jthread is not supported by clang I used.
    std::thread receiveThread {Consume::Consumer::receiveTask};
    std::thread acknowledgeSendThread {Consume::Consumer::acknowledgeSendTask};

    receiveThread.join();
    acknowledgeSendThread.join();

    return 0;
}
