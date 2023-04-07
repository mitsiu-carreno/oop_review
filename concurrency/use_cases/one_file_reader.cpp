#include <iostream>
#include <thread>
#include <mutex>
#include <string>
#include <fstream>

std::mutex m;
int total = 0;

void CountA(const int workload, int start, char *file){
  std::ifstream file_reader;
  std::string one_line;
  int thread_total = 0;
  int current_work = 0;

  file_reader.open(file);
  if(file_reader.is_open()){
    file_reader.seekg(start, std::ios_base::beg);  // Move cursor to start position
    while(std::getline(file_reader, one_line)){
      int chars_to_read = one_line.size() +1 <= (workload - current_work) 
        ? one_line.size() +1
        : workload - current_work;


      if(chars_to_read == 0){
        break;
      }
      current_work += chars_to_read; 

      for(int i =0; i< chars_to_read; ++i){
        if(one_line[i] == 'a'){
          ++thread_total;
        }
      }
    }
  }

  m.lock();
  total += thread_total;
  m.unlock();
  std::cout << "Start: " << start << " found: " << thread_total << "\n";
}

int main(){
  char file_name [] ="sample_1"; 
  std::ifstream file;
  file.open(file_name);
  file.seekg(0, file.end); //Mover cursor a fin de archivo
  int file_len = file.tellg();  //Contar cantidad de caracteres
  //file.seekg(0, file.beg);  Mover cursor a inicio de archivo

  const int thread_workload = file_len / 5;

  std::thread t1(&CountA, thread_workload, (0*thread_workload), file_name);
  std::thread t2(&CountA, thread_workload, (1*thread_workload), file_name);
  std::thread t3(&CountA, thread_workload, (2*thread_workload), file_name);
  std::thread t4(&CountA, thread_workload, (3*thread_workload), file_name);
  std::thread t5(&CountA, thread_workload, (4*thread_workload), file_name);

  t1.join();
  t2.join();
  t3.join();
  t4.join();
  t5.join();


  std::cout << "Total de a: " << total << "\n";

  return 0;
}
