/*
All good with mutexes but what if we forgot to unlock the mutex at the end of the function? or what if we lock multiple times?
We have to remember to unlock() each and every code path (including exceptions) out of function or else we might run into an undesirable situation.

std::lock_guard implements Resource Acquisition Is Initialization (RAII describes the concept that resource will be held only for object lifetime) for mutex
It is usually called mutex wrapper that provides the RAII style mechanism meaning the mutex will only be owned for the duration of the scoped block.

When std::lock_guard is instantiated, it attempts to take ownership of the mutex it is given (as if lock was called ON mutex). The destructor of lock_guard will be called 
releasing the mutex as soon as the lock_guard object goes out of scope (same as unlock() is called)
*/

#include <thread>
#include <iostream>
#include <mutex>

class RaceCondition{
  public:
    std::mutex my_mutex;
    int n;
    RaceCondition():n(0){}
    void Increment(){
      std::lock_guard<std::mutex> my_lock(my_mutex);
      // We pass a mutex to the constructor of std::lock_guard<> to aquire the lock and mutex will be unlocked in the destructor removing the need to call unlock() explicitly
      ++n;
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

*/
