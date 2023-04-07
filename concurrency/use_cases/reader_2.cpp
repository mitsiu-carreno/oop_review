#include <fstream>
#include <iostream>
#include <string>
#include <thread>


void CountA(int min_line, int max_line, int *counter){
  std::ifstream read_stream;
  std::string one_line;
  int total_lines = 0;

  read_stream.open("sample_1");

  if(read_stream.is_open()){
    while(std::getline(read_stream, one_line)){
      if(total_lines > max_line){
        return;
      }
      if(total_lines >= min_line){
        for(int char_pos = 0; char_pos < one_line.size(); ++char_pos){
          if(one_line[char_pos] == 'a'){
            ++*counter;
          }
        }
      
      }
      ++total_lines;
    }
  }
}

/*
void CountA(char *file){
  
  std::ifstream read_stream;
  std::string one_line;
  int total_lines = 0;
  int total_a = 0;

  read_stream.open(file);

  if(read_stream.is_open()){
    while(std::getline(read_stream, one_line)){
      ++total_lines;
      //std::cout << one_line << "\n";
      for(int char_pos = 0; char_pos < one_line.size(); ++char_pos){
        if(one_line[char_pos] == 'a'){
          ++total_a;
        }
      }
    }
  }

  std::cout << "Total lines: " << total_lines << "\n";
  std::cout << "Total a found: " << total_a << "\n";

}
*/

int main(){
  /*
  std::ifstream read_stream;
  std::string one_line;
  int total_lines = 0;
  int total_a = 0;

  read_stream.open("sample_1");

  if(read_stream.is_open()){
    while(std::getline(read_stream, one_line)){
      ++total_lines;
      //std::cout << one_line << "\n";
      for(int char_pos = 0; char_pos < one_line.size(); ++char_pos){
        if(one_line[char_pos] == 'a'){
          ++total_a;
        }
      }
    }
  }

  std::cout << "Total lines: " << total_lines << "\n";
  std::cout << "Total a found: " << total_a << "\n";
*/

  /*
  CountA((char*)"sample_1");
  CountA((char*)"sample_2");
  CountA((char*)"sample_3");
*/
  /*
  std::thread t1(CountA, (char*)"sample_1");
  std::thread t2(CountA, (char*)"sample_2");
  CountA((char*)"sample_3");
*/
int t1_total=0;
int t2_total=0;
int main_total = 0;
  //std::thread t1(CountA, 0, 1666740, &t1_total);
  //std::thread t2(CountA, 1666741, 3333480, &t2_total);
  CountA(0, 5000229, &main_total);

  //t1.join();
  //t2.join();

  std::cout << "Total a found: " << t1_total + t2_total + main_total << "\n";

  return 0;
}
