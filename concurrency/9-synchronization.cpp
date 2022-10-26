/*
DIFICULT
A thread might need to wait for an event to occur or a task to be completed by another thread. For example a thread notifying another thread that it has finished reading data and the other thread 
can now process the data further. So an await/notify mechanism is required to synchronize threads when accessing shared resources.

C++ standard library provides conditional variables to handle such problems.

std::condition_variable class is a synchronization primitive used to block the calling thread until notified to resume.
condition_variable::wait()        Wait on a mutex until notified
condition_variable::wait_for()    Wait for timeout or until notified
condition_variable::wait_until()  Wait until specific point of time or notified
condition_variable::notify_one()  Awakens a waiting thread
condition_variable::notify_all()  Awakens all waiting threads

*/

#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>

std::mutex mutex;
std::condition_variable cv;
bool ready = false;

void Print(){
  std::cout << "Waiting for other thread to signal ready!\n";
  // Needs to acquire std::unique_lock the cv.wait will release the mutex atomically (waiting thread will release the mutex while waiting) and suspend the execution of the thread
  // After the condition variable is notified, the thread will be awakened and the mutex is REACQUIRED
  // IT IS NOT A GOOD IDEA TO KEEP THE MUTEX LOCKED FOR LONGER THAN NECESSARY
  std::unique_lock<std::mutex> lock(mutex);
  while(!ready){
    cv.wait(lock);
  }
  std::cout << "Thread resume now\n";
}

void Execute(){
  std::cout << "Thread is ready to execute now\n";
  ready = true;
  cv.notify_all();
}

int main(){
  std::thread t1(Print);
  std::thread t2(Print);
  std::thread t3(Execute);

  t1.join();
  t2.join();
  t3.join();

  return 0;
}
