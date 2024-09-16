
#include <consume/Consumer.hpp>
#include <produce/Producer.hpp>

#include <thread>
#include <iostream>

#include <fmt/format.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

void waitForChildrenProcesses()
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
    pid_t                  proceessId {fork()};

    if (proceessId < 0)
    {
        fmt::println(stderr, "Can not create a child process.");
        exit(1);
    }

    if (kChildProcessId == proceessId)
    {
        fmt::println("Producer started.");

        // Note that jthread is not supported by clang I used.
        std::thread generatorThread {Produce::Producer::generateTask};
        std::thread acknowledgeReceiveThread {Produce::Producer::acknowledgeReceiveTask};

        generatorThread.join();
        acknowledgeReceiveThread.join();
        fmt::println("Producer ended.");
    }
    else
    {
        fmt::println("Consumer started.");

        std::thread receiveThread {Consume::Consumer::receiveTask};
        std::thread acknowledgeSendThread {Consume::Consumer::acknowledgeSendTask};

        receiveThread.join();
        acknowledgeSendThread.join();
        fmt::println("Consumer ended.");

    }

    waitForChildrenProcesses();

    return 0;
}
