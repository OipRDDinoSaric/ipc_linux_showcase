
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

    int pipeDescriptor[2];
    int errorCode {pipe(pipeDescriptor)};
    int writeDescriptor {pipeDescriptor[1]};
    int readDescriptor {pipeDescriptor[0]};

    if (errorCode != 0)
    {
        fmt::println(stderr, "Pipe creation failed.");
        exit(1);
    }

    pid_t proceessId {fork()};

    if (proceessId < 0)
    {
        fmt::println(stderr, "Can not create a child process.");
        exit(1);
    }

    if (kChildProcessId == proceessId)
    {
        Produce::Producer producer {writeDescriptor};
        fmt::println("Producer started.");

        // Note that jthread is not supported by clang I used.
        std::thread generateThread {Produce::Producer::generateTask, std::ref(producer)};
        std::thread acknowledgeReceiveThread {Produce::Producer::acknowledgeReceiveTask,
                                              std::ref(producer)};

        generateThread.join();
        acknowledgeReceiveThread.join();
        fmt::println("Producer ended.");

        close(readDescriptor);
    }
    else
    {
        Consume::Consumer consumer {readDescriptor};

        fmt::println("Consumer started.");

        std::thread receiveThread {Consume::Consumer::receiveTask, std::ref(consumer)};
        std::thread acknowledgeSendThread {Consume::Consumer::acknowledgeSendTask,
                                           std::ref(consumer)};

        receiveThread.join();
        acknowledgeSendThread.join();
        fmt::println("Consumer ended.");

        close(writeDescriptor);
    }

    waitForChildrenProcesses();

    return 0;
}
