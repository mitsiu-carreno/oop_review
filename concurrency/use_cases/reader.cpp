#include <fstream>
#include <iostream>
#include <string>

int main(){
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

  return 0;
}
