# IPC Linux showcase

Two processes of communication synchronization. For more information, see [task](./doc/Task.pdf).

## Building and running

1. Make a new directory for building the project in the project root
```shell
mkdir cmake-build-default
```

2. Go to the new directory

```shell
cd cmake-build-default
```

3. Configure CMake

```shell
cmake ../
```

4. Compile the project
```shell
cmake --build .
```

5. Run the executable

```shell
./IpcLinuxShowcase
```

Tested on aarch64 with ubuntu 22.04 LTS and macOS.

## Rationale for IPC and synchronization mechanism

Two processes and four threads must be synchronized. Each process/thread seam is covered separately.

### Producer generator task and Consumer receive task

A randomly generated number must be sent from the producer to the consumer.
As producer and consumer are separate processes, an IPC mechanism must be used.
Linux offers multiple possible ways to handle safely sending data from one process to another.

One option is sockets.
Even though sockets offer convenient and useful features, they are dismissed because they have an overhead and
complexity not needed for this simple use-case.
The overhead is deemed too expensive.

Shared memory offers the fastest transfers between multiple processes and random access to the shared memory, but it
requires manual synchronization.
Manual synchronization creates programmer time overhead not needed at this stage of the task.

Message queues feature kernel persistence, meaning shutting down one task and starting it again is straightforward and
data can be fetched in any order.
As my philosophy for IPCs is that it is the best when it is the simplest message queues will not be used as more popular
IPC exists.

Pipes offer a similar API to message queues, but are used more frequently (sometimes without a merit).
Pipes are a FIFO buffer with built-in synchronization.
Here they are used because of the small nature of the message sent, simplicity of the channel and the speed of
development.
Another advantage of pipes for this use-case is that their lifetime is as long as the processes that use them.

### Consumer receive task and send acknowledge task

To acknowledge a message, message IDs are included in every packet received.
The Seam between these two threads does not cross the process barrier and IPC mechanisms are not used.

The standard library (STL) offers us an API to many common thread synchronization primitives.
In this case, condition variable is used as it offers a transfer of data from one thread to another in a thread safe
manner and with RAII.
They also synchronize the tasks, which enables simpler data transfer at the cost of the reading speed.
Alternative would be adding a buffer between the threads.
As pipe is used in the step before, it can buffer the messages that will be synchronous here, between two threads.
Condition variable offers waiting on the notification of another task which is perfect for this seam.


### Consumer send acknowledge task and producer receive acknowledge task

The pipes are used for this use-case with the same rationale as
in [Producer generator task and Consumer receive task](#producer-generator-task-and-consumer-receive-task) section.

## Error handling

Immediate error handling is often used through the code.
This is a small executable and creating a complex, deferred error handling was dismissed over readability and code
brevity.

## Further improvements and inquiries

- Add a timeout/polling when reading from a pipe to make it interruptible with SIGINT.
  I am not sure if this is a requirement for the task, but I can add this upon request.
- The task says that the second thread of the consumer must send messages periodically to the producer.
  Does this mean the period between messages must be constant or is the current implementation good enough?
- In case faster reading is required, asynchronous transfer of data from a Consumer receive task to send an
  acknowledgment task should be used.

In case any of these modifications are necessary, feel free to contact me.

