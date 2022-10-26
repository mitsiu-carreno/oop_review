/*
One of the most common ways to handle potential errors is via return codes 
*/


// ---------------------------------------------
#include <iostream>
/*
#include <vector>

int FindPosition(std::vector<int> arr, int lookfor){
  for(int i=0; i< arr.size(); ++i){
    if(arr.at(i) == lookfor){
      return i;
    }
  }
  return -1;
}

void PrintPosition(int lookfor, int position){
  if(position >= 0){
    std::cout << "Number "<< lookfor << " is at position " << position << "\n";
  }else{
    std::cout << "Number " << lookfor << " wasn't found\n";
  }
}

int main(){
  std::vector<int> numeros {12, 30, 28, 1991};
  int position;
  int num;

  num=28;
  position = FindPosition(numeros, num);
  PrintPosition(num, position);

  num=15;
  position = FindPosition(numeros, num);
  PrintPosition(num, position);
  return 0;
}
*/

// Return codes can be cryptic, is it trying to indicate an error, or is that an actual valid return value?
// Second functions can only return one value 

double Divide(double a, double b){
  return a/b;
}

int main(){
  std::cout << "5/2 = " << Divide(5,2) << "\n";
  std::cout << "10/8 = " << Divide(10,8) << "\n"; 

  std::cout << "10/0 = " << Divide(10, 0) << "\n";
  if(Divide(10,0) == inf){
    std::cout << "Catched\n"; 
  }

  return 0;
}

// Third sequuences of code where many things can go wrong, code have to be checked constantly
/*

std::ifstream setupIni { "setup.ini" }; // open setup.ini for reading
// If the file couldn't be opened (e.g. because it was missing) return some error enum
if (!setupIni)
    return ERROR_OPENING_FILE;

// Now read a bunch of values from a file
if (!readIntegerFromFile(setupIni, m_firstParameter)) // try to read an integer from the file
    return ERROR_READING_VALUE; // Return enum value indicating value couldn't be read

if (!readDoubleFromFile(setupIni, m_secondParameter)) // try to read a double from the file
    return ERROR_READING_VALUE;

if (!readFloatFromFile(setupIni, m_thirdParameter)) // try to read a float from the file
    return ERROR_READING_VALUE;

*/


/*
Exception handling provides a mechanism to decouple handling of errors or other exceptional circumstances from the typical control flow of your code. This allows more freedom to handle 
errors when and how ever is most useful given for a situation, alleviating most (if not all) of the messiness that return code causes
*/
