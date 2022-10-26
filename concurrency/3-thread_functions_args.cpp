#include <iostream>
#include <thread>

// -------------------------------------
/*
#include <chrono>
void func(){
  
}

int main(){
  std::thread t1(func);
  std::thread t2(func);
  std::cout << "Main thread ID is " << std::this_thread::get_id() << "\n";
  std::cout << "First thread ID is " << t1.get_id() << "\n";
  std::cout << "Second thread ID is " << t2.get_id() << "\n";

  
  //   STD::THIS_THREAD FUNCTIONS
  //   get_id()       returns id of the current thread
  //   yield()        suspend current thread so that another may run
  //   sleep_for()    thread will sleep for the specified duration of time
  //   sleep_until()  thread will sleep until the specified point of time
  
  std::this_thread::sleep_for(std::chrono::seconds(5));

  t1.join();
  t2.join();

  return 0;
}
*/

// -------------------------------------

/*
To pass arguments to the thread's callable; you just need to pass additional arguments to the thread constructor
By default all arguments are copied into the internal storage of the new thread so that it can be accessed without any problem.
C++ provides std::ref() wrapper to pass arguments by reference to the thread
*/
void func(int arg){
  std::cout << "Received " << arg << "\n";
}

void func2(int arg, bool arg2){
  std::cout << "Received " << arg << " & " << arg2 << "\n";
}

void func3(int &ref){
  std::cout << "Received by reference " << ref << "\n";
  ref = 8;
}

int main(){
  int param = 5;
  std::thread t1(func, 3);
  std::thread t2(func2, param, true); 
  // SHOW INCORRECT CALL (HARD DEBUGGING) std::thread t2(func2, param);
  std::thread t3(func3, std::ref(param));

  t1.join();
  t2.join();
  t3.join();
  std::cout << "param value = " << param << "\n";
  return 0;
}
