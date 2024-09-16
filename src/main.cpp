#include <thread>

#include <consume/Consumer.hpp>
#include <produce/Producer.hpp>

#include <fmt/format.h>

int
main(int, char*[])
{
    fmt::println("Consumer started.");

    // TODO fork()

    // Note that jthread is not supported by clang I used.
    std::thread receiveThread {Consume::Consumer::receiveTask};
    std::thread acknowledgeSendThread {Consume::Consumer::acknowledgeSendTask};

    receiveThread.join();
    acknowledgeSendThread.join();

    fmt::println("Producer started.");

    // Note that jthread is not supported by clang I used.
    std::thread generatorThread {Produce::Producer::generateTask};
    std::thread acknowledgeReceiveThread {Produce::Producer::acknowledgeReceiveTask};

    generatorThread.join();
    acknowledgeReceiveThread.join();

    return 0;
}
