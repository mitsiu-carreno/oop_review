/*
Mutual Exclusion (Mutex)
MUTual EXclusion refers to the idea of preventing simultaneous execution of a critical section (a piece of code that accesses a shared resource) by multiple threads.
C++ provides std::mutex class for synchronization that is used to protect shared data from concurrent access by more than one thread of execution. 
It works with locking and unlocking mechanism. Once the mutex is locked, the concurrent executing thread will own the resource until it is unlocked; meaning that no other thread can 
execute any instructions until mutex unlocks the block of code surrounded by it.
*/

#include <thread>
#include <iostream>
#include <mutex>


//--------------------------------------
class RaceCondition{
  public:
    // Create mutex by instantiating/invoking std::mutex 
    std::mutex my_mutex;
    int n;
    RaceCondition():n(0){}
    void Increment(){
      my_mutex.lock();
      // The lock makes sure that once one thread finishes the modification, ony then another thread modifies the data. 
      ++n;
      my_mutex.unlock();
      // 
    }
};

int main(){
  RaceCondition racer;
  std::thread t1([&racer]{
    for(int i=0; i<10000; ++i){
      racer.Increment();
    }
  });

  std::thread t2([&racer]{
    for(int i =0; i<10000; ++i){
      racer.Increment();
    } 
  });

  t1.join();
  t2.join();

  std::cout << racer.n << "\n";

  return 0;
}


/*
   COULDN'T TRIGGER
If there's some error exception, system_error is thrown while error codes should be as follow:
-resource_deadlock_would_occur
-resource_unavailable_try_again
-operation_not_permitted
-device_or_resource_busy
-invalid_argument

   OPTIONAL
There are several mutex classes in the library
-std::mutex             Provides basic locking
-std::recursive_mutex   Same thread can do repeated locking without deadlocking
-std::timed_mutex       Provides timeout when trying to acquire the lock i.e. thread can do something else when wating for a thread to finish
-std::recursive_timed_mutex  Same as above
-std::shared_mutex      Allows more than one thread to own the mutex
-std::call_once(std::once_flag flag, fuction) std::call_once is matched to std::once_flag and function would be called only once no matter how many threads are launched


*/
