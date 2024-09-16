/**
 * @file
 *
 */

#include <consume/Consumer.hpp>

#include <unistd.h>
#include <cstdlib>

#include <fmt/format.h>

#include <mutex>
#include <thread>

namespace Consume
{

Consumer::Consumer(int pipeReadDescriptor) :
        pipeReadDescriptor {pipeReadDescriptor},
        sum {0},
        cvReceiveToAcknowledge {},
        mtxReceiveToAcknowledge {},
        idReceiveToAcknowledge {},
        hasNewMessageForAcknowledge {false}
{}

[[noreturn]] void
Consumer::receiveTask(Consumer& consumer)
{
    while (true)
    {
        const auto package {consumer.receiveGeneratedNumber()};

        fmt::println("Read message {} with value {}.", package.id, package.generatedNum);

        consumer.sum += package.generatedNum;

        fmt::println("Current sum is {}.", consumer.sum);

        consumer.sendToAcknowledgeSend(package.id);
    }
}

[[noreturn]] void
Consumer::acknowledgeSendTask(Consumer& consumer)
{
    while (true)
    {
        int id {};
        {
            {
                std::unique_lock lock {consumer.mtxReceiveToAcknowledge};
                consumer.cvReceiveToAcknowledge.wait(
                lock,
                [&]()
                {
                    return consumer.hasNewMessageForAcknowledge;
                });
                id                                   = consumer.idReceiveToAcknowledge;
                consumer.hasNewMessageForAcknowledge = false;
            }
            consumer.cvReceiveToAcknowledge.notify_one();
        }

        fmt::println("Acknowledging message with id {}.", id);


    }
}

Consumer::Package
Consumer::receiveGeneratedNumber() const
{
    Package package {};

    ssize_t errorCode {read(pipeReadDescriptor, &package, sizeof(package))};

    if (sizeof(package) != errorCode)
    {
        if (-1 == errorCode)
        {
            fmt::println(stderr, "Error while reading from the pipe: {}", strerror(errno));
            exit(1);
        }
        fmt::println(stderr, "Error while reading from the pipe: Size mismatch.");
    }

    return package;
}

void
Consumer::sendToAcknowledgeSend(int id)
{
    {
        std::unique_lock lock {mtxReceiveToAcknowledge};
        cvReceiveToAcknowledge.wait(lock,
                                    [&]()
                                    {
                                        return !hasNewMessageForAcknowledge;
                                    });

        idReceiveToAcknowledge      = id;
        hasNewMessageForAcknowledge = true;
    }

    cvReceiveToAcknowledge.notify_one();
}

}  // namespace Consume
