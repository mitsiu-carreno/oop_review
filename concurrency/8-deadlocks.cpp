/*
Usage of mutexes seems a piece of cake as for now because they ensure that code in a critical section can only be accessed by a single thread at any time.
Locking and unlocking mechanism provides exclusivity, but let's write a simple program to unleash the devil.
*/

#include <thread>
#include <iostream>
#include <mutex>

// -------------------------------------------
/*
void Print(){
  std::cout << "Critical Data\n";
}

int main(){
  std::mutex mutex1, mutex2;

  std::thread t1([&mutex1, &mutex2]{
    std::cout << "T1 Acquiring first mutex\n";
    mutex1.lock();
    Print();

    std::cout << "T1 Acquiring second mutex\n";
    mutex2.lock();
    Print();

    std::cout << "T1 Release first mutex\n";
    mutex1.unlock();
    std::cout << "T1 Release second mutex\n";
    mutex2.unlock();
  });

  std::thread t2([&mutex1, &mutex2]{
    std::cout << "T2 Acquiring second mutex\n";
    mutex2.lock();
    Print();

    std::cout << "T2 Acquiring first mutex\n";
    mutex1.lock();
    Print();

    std::cout << "T2 Release second mutex\n";
    mutex2.unlock();
    std::cout << "T2 Release first mutex\n";
    mutex1.unlock();
  });

  t1.join();
  t2.join();

  return 0;
}
*/

/*
The program sits there and nothing happens afterward. Such condition is known as deadlock (A state in which one process is holding onto a resource and waiting for some other resource acquired by some
other process and thus both processes end up waiting for each other to do something)
 Deadlock
    T1  T2
M1  1   X (DEADLOCK)
M2      2

Deadlock is possible if t1 acquires mutex mutex1 while t2 acquires mutex mutex2. Now, t2 would wait for mutex mutex1 (acquired by t1) and t1 will wait for mutex mutex2 thus leading to deadlock condition

The program may run just fine, no deadlocks. That's for the reason that even though deadlock is possible, it will not occur if thread t1 acquire and releases the mutex locks for m1 and m2 before
thread t2 attempts to acquire the locks. And, of course, the order in which the threads run depends on hoy they are scheduled by the CPU scheduler.

Last but not least, the above code represents the bad code example. NEVER EVER TRY TO ACQUIRE A LOCK WHILE HOLDING ONTO ANOTHER LOCK as it will most certainly lead to deadlock.
Also avoid nested locks.
*/

/*
AVOIDING DEADLOCKS 
One of the most common ways of avoiding a deadlock is to lock away the mutexes in the same order. m1 is always locked before m2 and we'll not face the deadlock anymore.

// [MODIFY EXITING EXAMPLE AND PRACTICE]

DIFICULT
But it's not that simple in all cases, such as when the mutexes are protecting some separate instance of the same class. In such a situation, it is often necessary to apply some extra C++ provided funcionality
  * Lock mutexes at the same time and then create the guard afterwards
  * Create the guards and then lock mutexes at the same time
*/

// Lock mutexes at the same time and then create the guard afterwards

void Print(){
  std::cout << "Critical Data\n";
}

int main(){
  std::mutex mutex1, mutex2, mutex3;

  std::thread t1([&mutex1, &mutex2]{
    // Lock mutexes at the same time and then create the guard afterwards
    std::lock(mutex1, mutex2);  // std::lock() uses deadlock-avoidance algorithm and locks all the objects passed as arguments
    // We acquire the locks first (avoiding deadlocks)

    // And then we create the guards to release them properly
    std::lock_guard<std::mutex> lock1(mutex1, std::adopt_lock); // std::adopt_lock assumes that the calling thread already owns the lock which we did with std::lock() and adopt the ownership of the mutex and releases it (unlock) as soon as the control goes out of scope
    std::lock_guard<std::mutex> lock2(mutex2, std::adopt_lock);
    std::cout << "T1 Acquiring first mutex\n";
    std::cout << "T1 Acquiring second mutex\n";
    Print();

    Print();

    std::cout << "T1 Release first mutex\n";
    std::cout << "T1 Release second mutex\n";
  });

  std::thread t2([&mutex1, &mutex2]{
    // First we create the lock (without acquiring them) 
    std::unique_lock<std::mutex> lock1(mutex1, std::defer_lock);  // std::defer_lock does not acquire the ownership of the mutex rather assumes that lock will be called later by the calling thread to acquire the mutex
    std::unique_lock<std::mutex> lock2(mutex2, std::defer_lock);  

    // And then acquiring them simultaneously using std::lock without the risk of a deadlock
    std::lock(lock1, lock2);  

    // One thing to note here is that you're not supposed to pass two successive calls to std::lock 
    // to acquire the mutexes individually or you'll again run into deadlock situation
    // std::lock(lock1);
    // std::lock(lock2);
   
    std::cout << "T2 Acquiring second mutex\n";
    std::cout << "T2 Acquiring first mutex\n";
    Print();

    Print();

    std::cout << "T2 Release second mutex\n";
    std::cout << "T2 Release first mutex\n";
  });

  std::thread t3([&mutex1, &mutex2, &mutex3]{
    std::scoped_lock<std::mutex, std::mutex, std::mutex> lock(mutex1, mutex2, mutex3);
    Print();
    Print();
  });

  t1.join();
  t2.join();
  t3.join();

  return 0;
}
