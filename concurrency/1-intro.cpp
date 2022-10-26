/*
C++ was originally designed to support only single thread programming (the execution control would go straight to the main function (entry point for default thread),
the program would execute in a sequential manner (one computation would complete before the next starts))

Then comes the multi-processor and multi-core systems and our single-threaded, sequential programs become un-optimized for them. So, there's a need to utilize the 
computational resource efficientyl and that's exactly where concurrent programming appears.

Concurrent programming allows for the execution of multiple threads and thus, we can take advantage of any parallelism available in a computer system.

Concurrent refers to the idea of executing several tasks at the same time. This can be achieved in a time-shared manner on a single CPU core (implying 'Multitasking')
or in a parallel in case multiple CPU cores (Parallel Processing)

Concurrent program is a program that offers more tha one execution paths that run in parallel or simply a program that implements the concurrnecy. These execution paths 
are managed by means of threads that execute concurrently and work together to perform some task.

Thread describes the execution path through the code. Each thread belongs to a process (the instance of an executing program that owns resources like memory, file 
handle, etc) as it is executing the code of the process, uses its resources, and there can be multiple threads being executed at the same time inside a process. Each 
process must have at least one thread that shows the main execution path.

Concrrency vs Parallelization

Concurrency structures the program by allocation multiple control flows in it. Conceptually, these flows are executed parallel, although, in fact, they can be executed 
on a set of processors (cores) or on one processor with context switching (switching the CPU from one process or thread to another). This approach is otherwise known as 
mulit-threading. Now the true parallelism is achieved through Parallel programming that aims at speeding the execution of a certain task up by means of using multiple 
equipment items(processors, cores, or computers in a distributed system).
The task is divided into several subtasks that can execute independently and uses their own resources. Once all subtasks are complete, their results are merged.
In practice, these two approaches can be combined to complement each other. For example, allocation of independent concurrent subtasks can help both, structure a program 
and improve its performance.

Why concurrency?
-Speed
  Up until 2002, making bigger and faster processors was the trend, (Moore's law) but since then, the paradigm shifted to slower processors put in groups (multi-core processors)
  Also, GPU's (Graphics processing units) were introduced, which can range to hundreds, and even thousands of cores for a massively parallel architecture aimed at handling 
  multiple functions at the same time.
  Now if we stick to old-school programs that are not concurrent at all, only one core or thread is being used while the rest of your CPU sits idle. (Concurrent program makes 
  use of any parallelism available in a computer system) Even in case of a single CPU, concurrency prevents one activity from blocking another while waiting for I/O.

-Availability and Distribution
  There can also be some external driving forces that are being imposed by the environment. For example, many things are happening simultaneously in real-word systems, this 
  demands software to be reactive and respond to externally generated events at random times. It's simpler to partition the system into concurrent pieces that can deal individually 
  with each of these events.

-Controllability
  Concurrency also increases the controllability of the system as a function can be started, stoped or can be interrupted by other concurrent function. This functionality is 
  very hard to achieve when working with the sequential program.

*/






// THE THREADING LIBRARY
/* 
C++ did not have any standard support for concurrent programming before C++11. (You had to rely on libraries that were OS-dependent (pthread on linux or Windows API on Windows))
Though the concepts where the same, there was a significant difference in how you implement them. The need for a portable concurrent code was met by introducing the multithreading
capabilities in standard C++11 that consists of two parts
  - Memory model
  - Standard threading api

Standard threading api provides you with a portable way to work with threads using the <thread> header while memory model sets the rule that threads will follow when sharing data 
in memory.

C++14 introduced Reader-Writer locks to minimize the bottleneck.
C++17 made most of the algorithms of the Standard Template Library available in a parallel version. 
*/

// Thread is basically a lightweight sub-process. It is an independent task running inside a program that can be started, interrupted and stopped.

// <image></image>

// More than one thread can be executed concurrently within one process (which refers to the idea of multi-threading) and share the data, code, resources such as memory, address space, 
//and per-process state information

// Each C++ application has one main thread (main()) while additional can be created by creating the objects of std::thread class

// g++ -std=c++1z -o 1-intro 1-intro.cpp -lpthread -Wl,--no-as-needed
// g++ -o 1-intro 1-intro.cpp -lpthread
// clang++ -o 1-intro 1-intro.cpp -lpthread && ./1-intro
// Linker -lpthread
#include <thread>
#include <iostream>

/*
// Simple regular function
void func(){
  std::cout << "Concurrent execution in C++\n"; 
}

int main(){
  // Launch a thread that will execute our function func
  std::thread t1(&func);
  // Make sure of waiting until t1 terminates.
  t1.join();
  return 0;
}
*/

// ----------------------------
/*
void func(){
  std::cout << "a\n";
  for(int i=0; i<50; ++i){
    std::cout << "Concurrent execution of thread in C++ \n";
  }
}

// There's no strict sequence of execution rather, both threads execute at the same time, but main function will always exit at the very end

int main(){
  std::thread t1(func);
  std::cout << "Main function thread executing\n";
  for(int i=0; i<50; ++i){
    std::cout << "Main " << i << "\n";
  }
  t1.join();
  // join make's sure that t1 terminates before main function exits
  std::cout << "Main function exiting... \n";
  return 0;
}

*/


// ----------------------------  EXPERIMENTAL
// LOG $ ps -T -p <pid> || $ top -H -p <pid> || $ ls -la /proc/<pid>/task/

#include <unistd.h>

void func(){
  std::cout << "a\n";
  for(int i=0; i<100; ++i){
    sleep(1);
    //std::cout << "Concurrent execution of thread in C++ \n";
  }
}

// There's no strict sequence of execution rather, both threads execute at the same time, but main function will always exit at the very end

int main(){
  std::cout << getpid() << "\n";
  std::thread t1(func);
  std::cout << "Main function thread executing\n";
  for(int i=0; i<80; ++i){
    sleep(1);
    //std::cout << "Main " << i << "\n";
  }
  t1.detach();
  // join make's sure that t1 terminates before main function exits
  std::cout << "Main function exiting... \n";
  return 0;
}

