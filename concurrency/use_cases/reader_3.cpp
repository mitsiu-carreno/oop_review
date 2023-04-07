#include <fstream>
#include <iostream>
#include <thread>
#include <mutex>

std::mutex m;

void CountA(int min_line, int max_line){
}

int main(){
  std::ifstream read_stream;
  read_stream.open("sample_1");

  if(read_stream.is_open()){
    read_stream.seekg(0, read_stream.end);
    int len = read_stream.tellg();
    std::cout << len << "\n";
  }

  return 0;
}
