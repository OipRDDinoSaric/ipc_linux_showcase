/**
 * @file
 *
 */

#include <consume/Consumer.hpp>

#include <unistd.h>
#include <cstdlib>

#include <fmt/format.h>

namespace Consume
{

Consumer::Consumer(int pipeReadDescriptor) : pipeReadDescriptor {pipeReadDescriptor}
{}

[[noreturn]] void
Consumer::receiveTask(Consumer& consumer)
{
    while (true)
    {
        const auto package {consumer.receiveGeneratedNumber()};

        fmt::println("Read message {} with value {}.", package.id, package.generatedNum);
    }
}

void
Consumer::acknowledgeSendTask()
{}

Consumer::Package
Consumer::receiveGeneratedNumber()
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

}  // namespace Consume
