#include <iostream>
#include <fstream>
#include <cstdint>
#include <string>
#include <cmath>
#include <cassert>
#include <cstddef>
#include <exception>
#include <stdexcept>
#include <sstream>
using namespace std;

const uint8_t N = 11;
const string ListofCommand[N] = {"push", "pop", "add", "sub", "div", "mul", "kv", "ax", "bx", "cx", "dx"};
const char ListofCode[N] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K'};

int ReadCommand(char* command, char* argv){//переделать на считывание с файла и возврат int'a на количество команд
  assert(command != NULL);
  assert(argv != NULL);
  ifstream input(argv);
  if(!input.is_open()){
    stringstream ss;
    ss << "Unknown command: " << argv << endl << "File can't be opened";
    throw invalid_argument(ss.str());
  }
  string buff;
  input >> buff;
  int count = 0;
  while(!input.eof()){
    for(int k = 0; k < N; k++){
      if(buff == ListofCommand[k]){
        command[count] = ListofCode[k];
        break;
      }
    }
    input >> buff;
    count++;
  }
  input.close();
  return count;
}

void CreateAsmFile(char* command, char* argv, int count){
  assert(argv != NULL);
  assert(isfinite(count));
  ofstream output(argv);
  if(!output.is_open()){
    stringstream ss;
    ss << "Failed to open file with name" << argv;
    throw invalid_argument(ss.str());
  }
  for(int i = 0; i < count; i++){
    output << command[i];
  }
  output << endl;
  output.close();
}

int main(int argc, char* argv[]){
  if(argc < 3){
    cout << "You have entered insufficient data" << endl;
    return 1;
  }
  char charof[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K'};
  char* command = (char*)calloc(100, sizeof(char));
  try{
    int count = ReadCommand(command, argv[1]);
    CreateAsmFile(command, argv[2], count);
  }catch(invalid_argument& inv){
    cout << inv.what() << endl;
    return 2;
  }
  return 0;
}
