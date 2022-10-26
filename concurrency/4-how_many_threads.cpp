#include <iostream>
#include <thread>

int main(){
  std::cout << std::thread::hardware_concurrency() << "\n";

  return 0;
}


/*
Hypothetically you can create as many threads as amount of memory on your machine, but then the scenarios arise where your concurrent code would execute much slower 
than sequential so the right question is how many threads should you create.

Computation intensive program
If your code is number crunching and involves a lot of computations and processing huge amount of data:
No of threads <= No of cores

IO intensive program
Reading and writing a large amount of data:
No of threads <= No of cores / 1 - blocking factor
*Blocking factor : the time a task will be spending before being blocked 

Suppose our program spends
75% of time Computing
25% of time blocking (I/O)

and has 16 cores

No of threads <= 16/(1-.75)
No of threads <= 16/(.25)
No of threads <= 64

Just because you can create as many threads as you want doesn't mean you should

  Blocking means that two or more consumers of a resource are

  * Trying to access the same resource at the exact same time AND
  * All of them have equal rights to it e.i. every one of the consumers is no more important than the other AND
  * Only one instance of the resource can be used at the time for it to be used at all.

  Synchronized would mean that access to the resouce is controlled via token 

  https://www.quora.com/In-Concurrency-does-blocking-mean-the-same-thing-as-synchronized?share=1

*/
