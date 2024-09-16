
#include <consume/Consumer.hpp>
#include <produce/Producer.hpp>

#include <thread>
#include <iostream>

#include <fmt/format.h>

#include <unistd.h>
#include <sys/wait.h>

void
waitForChildrenProcesses()
{
    while (true)
    {
        int   status;
        pid_t done = wait(&status);
        if (done == -1)
        {
            if (errno == ECHILD)
                break;  // no more child processes
        }
        else
        {
            if ((!WIFEXITED(status)) || (WEXITSTATUS(status) != 0))
            {
                pid_t processId {done};
                fmt::println(stderr, "Process with ID {} failed.", processId);
                exit(1);
            }
        }
    }
}

int
main(int, char*[])
{
    constexpr static pid_t kChildProcessId {0};

    int generatorPipe[2];
    int errorCode {pipe(generatorPipe)};
    if (errorCode != 0)
    {
        fmt::println(stderr, "Generator pipe creation failed.");
        exit(1);
    }

    int toConsumerWriteDesc {generatorPipe[1]};
    int fromGeneratorReadDesc {generatorPipe[0]};

    int acknowledgePipe[2];
    errorCode = pipe(acknowledgePipe);

    if (errorCode != 0)
    {
        fmt::println(stderr, "Acknowledge pipe creation failed.");
        exit(1);
    }

    int toProducerWriteDesc {acknowledgePipe[1]};
    int fromAcknowledgeReadDesc {acknowledgePipe[0]};

    pid_t proceessId {fork()};

    if (proceessId < 0)
    {
        fmt::println(stderr, "Can not create a child process.");
        exit(1);
    }

    if (kChildProcessId == proceessId)
    {
        Produce::Producer producer {toConsumerWriteDesc, fromAcknowledgeReadDesc};
        fmt::println("Producer: start.");

        // Note that jthread is not supported by clang I used.
        std::thread generateThread {Produce::Producer::generateTask, std::ref(producer)};
        std::thread acknowledgeReceiveThread {Produce::Producer::acknowledgeReceiveTask,
                                              std::ref(producer)};

        generateThread.join();
        acknowledgeReceiveThread.join();
        fmt::println("Producer: end.");

        close(toConsumerWriteDesc);
        close(fromAcknowledgeReadDesc);
    }
    else
    {
        Consume::Consumer consumer {fromGeneratorReadDesc, toProducerWriteDesc};

        fmt::println("Consumer: start.");

        std::thread receiveThread {Consume::Consumer::receiveTask, std::ref(consumer)};
        std::thread acknowledgeSendThread {Consume::Consumer::acknowledgeSendTask,
                                           std::ref(consumer)};

        receiveThread.join();
        acknowledgeSendThread.join();
        fmt::println("Consumer end.");

        close(fromGeneratorReadDesc);
        close(toProducerWriteDesc);
    }

    waitForChildrenProcesses();

    return 0;
}
