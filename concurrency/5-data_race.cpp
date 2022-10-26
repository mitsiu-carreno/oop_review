/*
The attempt to run multiple tasks at the same time introduces many complications and difficulties that accompany the benefits
*/

#include <thread>
#include <iostream>

class RaceCondition{
  public:
    int n;
    RaceCondition():n(0){
    }
    void Increment(){
      ++n;
    }
};
/*
RaceCondition contains a simple method that increments n. Let's try calling this function in a multi-threaded environment 
*/

int main(){
  RaceCondition racer;
  std::thread t1([&racer]{
    for(int i=0; i<10000; ++i){
      racer.Increment();
    }
  });

  std::thread t2([&racer]{
    for(int i=0; i<10000; ++i){
      racer.Increment();
    }
  });

  t1.join();
  t2.join();

  std::cout << racer.n << "\n";

  return 0;
}

/*
This is called a race condition: an undesirable situation where the output depends on the sequence or timing.

Increment is NOT an atomic operation (atomic implies that processor can simultaneously read and write to the same memory location in the same bus operation)
Increment implies the execution of three operations
1. Read the current value of n
2. Increment the current value of n by 1
3. Write new value of n

Because of simultaneous access to shared data, many errors can appear, both threads might interleave with each other and so the result becomes impredictalbe.

Syncronization
The sequence of above operations must be maintained as read, increment and write and repeat the same sequence as many times as we may want. But when we launch multiple threads, the sequence is
disturbed as we don't know which thread might be performing which of the above functions. Note that these operations are being performed over a single object n ant the data that it holds is 
shared among all threads that we created.

In order to avoid the race condition, we need a way to protect the shared data so that it must not be concurrently accessed by more than one thread of execution. Here are some proposed solutions:
-Semapthores
-*Mutex
-*Atomic references
-Monitors
-*Condition variables
-Compare and swap
*/
