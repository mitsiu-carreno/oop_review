/*
When you create a thread, you need to tell the compiler what the relationship with it is going to be
*/

#include <thread>
#include <iostream>

// -------------------------------------------
/*
void func(){
  for(int i=0; i<50; ++i){
    std::cout << "In thread loop\n";
  }
}

int main(){
  std::thread t1(func);
  for(int i=0; i<50; ++i){
    std::cout << "Main" << i << "\n";
  }

  std::cout << "ENDING\n";
  // ABORTED as thread had to be terminated
  return 0;
}
*/


/*
Destructor for std::thread class checks whether it still has any OS thread attached to it and will abort the program if it has. 
To avoid abortion, you must JOIN a thread or DETACH from it
-Join waits for the thread to finish and blocks until the thread finishes execution
-Detach leaves the thread to execute on its own and no further involvement is required from you 

You can check if a thread is joinable or not by simply calling bool joinable() member function.
(A thread cannot be joined more than once or if its already detached)

Be carefull using detach as it can create a thread to run in the background (deamon thread). Also you don't know when a detach
thread is going to finish so extra care when using pointers and references as things can go out of scope

*/

// -----------------------------------------
void func(){
  for(int i=0; i<50; ++i){
    std::cout << "In thread loop\n";
  }
}

int main(){
  std::thread t1(func);
  if(t1.joinable()){
    t1.join();
  }
  if(t1.joinable()){
    t1.join();
  }else{
    std::cout << "Thread is no longer joinable\n";
  }
  return 0;
}

