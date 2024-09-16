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

Consumer::Consumer(int fromGeneratorReadDesc, int toProducerWriteDesc) :
        fromGeneratorReadDesc {fromGeneratorReadDesc},
        toProducerWriteDesc {toProducerWriteDesc},
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
        consumer.receiveLoop();
    }
}

[[noreturn]] void
Consumer::acknowledgeSendTask(Consumer& consumer)
{
    while (true)
    {
        consumer.acknowledgeSendLoop();
    }
}

Consumer::Package
Consumer::receiveGeneratedNumber() const
{
    Package package {};

    ssize_t errorCode {read(fromGeneratorReadDesc, &package, sizeof(package))};

    if (sizeof(package) != errorCode)
    {
        if (-1 == errorCode)
        {
            fmt::println(stderr, "Error while reading from the pipe: {}", strerror(errno));
        }
        else
        {
            fmt::println(stderr, "Error while reading from the pipe: Size mismatch.");
        }
        exit(1);
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

void
Consumer::receiveLoop()
{
    const auto package {receiveGeneratedNumber()};
    fmt::println("Consumer: ID {} = number {}.", package.id, package.generatedNum);

    sum += package.generatedNum;
    fmt::println("Consumer: Sum = {}.", sum);

    sendToAcknowledgeSend(package.id);
}

void
Consumer::acknowledgeSendLoop()
{
    int id {};
    {
        {
            std::unique_lock lock {mtxReceiveToAcknowledge};
            cvReceiveToAcknowledge.wait(lock,
                                        [&]()
                                        {
                                            return hasNewMessageForAcknowledge;
                                        });
            id                          = idReceiveToAcknowledge;
            hasNewMessageForAcknowledge = false;
        }
        cvReceiveToAcknowledge.notify_one();
    }

    fmt::println("Consumer: Acknowledging ID {}.", id);

    ssize_t errorCode {write(toProducerWriteDesc, &id, sizeof(id))};

    if (sizeof(id) != errorCode)
    {
        if (-1 == errorCode)
        {
            fmt::println(stderr, "Error while writing to the pipe: {}", strerror(errno));
        }
        else
        {
            fmt::println(stderr, "Error while writing to the pipe: Size mismatch.");
        }
        exit(1);
    }
}

}  // namespace Consume
