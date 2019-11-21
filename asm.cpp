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

//переебать asm на вывод инта

const uint8_t N = 22;
const string ListofCommand[N] = {"push",   "pop", "add", "sub", "div", "mul", "kv", "end", "jmp",
"out", "call", "in", "ret", "jn", "copy", "jb", "jm", "je", "ax", "bx", "cx", "dx"};
const char ListofCode[N] =      {  'A',     'B',   'C',   'D',   'E',   'F',   'G',  'H',   'I',
'J',    'K',   'L',   'M',  'N',    'O',   'P',  'R',  'S', 'W',  'X',  'Y',  'Z'};

int ReadLabel(int* label, string* NameOfLabel, char* argv){
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
  int count1 = 0;
  while(!input.eof()){
    //cout << buff << endl;
    if(buff[0] == '!'){
      NameOfLabel[count1] = buff.erase(0,1);
      label[count1] = count;
      //cout << count << endl;
      count1++;
    }
    count++;
    input >> buff;
  }
  input.close();
  return count1;
}

int ReadCommand(char* command, int* label, string* NameOfLabel, int sizeLabel, char* argv){
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
  while(!input.eof()){//добавить проверку на неверную комманду
    for(int k = 0; k < N; k++){
      if(buff == ListofCommand[k]){
        command[count] = ListofCode[k];
        count++;
        if((k == 8) || (k == 10)){
          input >> buff;
          for(int i = 0; i < sizeLabel; i++){
            if(buff == NameOfLabel[i]){
              command[count] = static_cast<char>(label[i] + 32);//если label за пределами 256 или даже за 224, то не работает
              //cout << command[count] << " is command" << endl;
              //cout << label[i] << " is number" << endl;
              count++;
              break;
            }
          }
        }else if(k == 13 || k == 15 || k == 16 || k == 17){
          input >> buff;
          command[count] = (char)(stol(buff)+32);
          count++;
          input >> buff;
          for(int i = 0; i < sizeLabel; i++){
            if(buff == NameOfLabel[i]){
              command[count] = static_cast<char>(label[i] + 32);//если label за пределами 256 или даже за 224, то не работает
              //cout << command[count] << " is command" << endl;
              //cout << label[i] << " is number" << endl;
              count++;
              break;
            }
          }
        }else if(k == 0){
          input >> buff;
          if(buff == "ax"){
            command[count] = 'W';
          }else if(buff == "bx"){
            command[count] = 'X';
          }else if(buff == "cx"){
            command[count] = 'Y';
          }else if(buff == "dx"){
            command[count] = 'Z';
          }else if(buff[0] == '[' && buff[buff.size()-1] == ']'){
            command[count-1] = 'a';
            buff.erase(0,1);
            buff.erase(buff.size()-1, 1);//????? передать 4 бита
            command[count] = (char)(stol(buff) + 32);
          //buff равен "5"; "dx"; "-1"
          //если кастануть dx то нервам пизда
          //cout << command[count] << " is number" << endl;
          }else{
            command[count] = static_cast<char>(stol(buff) + 32);
          }
            count++;
        }else if(k == 13){
          input >> buff;
          command[count] = buff[0];
          count++;
          input >> buff;
          command[count] = static_cast<char>(stol(buff) + 32);//0+32 выведет stol!!!!!!!
          //buff равен "0" и после stol получаю 0
          //вместо 32 поставить 33 и не выводит
          count++;
          input >> buff;
          for(int i = 0; i < sizeLabel; i++){
            if(buff == NameOfLabel[i]){
              command[count] = static_cast<char>(label[i] + 32);
              //cout << command[count] << " is command" << endl;
              //cout << label[i] << " is number" << endl;
              count++;
              break;
            }
          }
        }else if(k == 1){
          input >> buff;
          if(buff == "ax"){
            command[count] = 'W';
          }else if(buff == "bx"){
            command[count] = 'X';
          }else if(buff == "cx"){
            command[count] = 'Y';
          }else if(buff == "dx"){
            command[count] = 'Z';
          }else if(buff[0] == '[' && buff[buff.size()-1] == ']'){
            command[count-1] = 'b';
            buff.erase(0,1);
            buff.erase(buff.size() - 1, 1);
            command[count] = static_cast<char>(stol(buff) + 32);
          }
          count++;
        }
        break;
      }
    }
    input >> buff;
  }
  /*for(int i = 0; i < count1; i++){
    cout << NameOfLabel[i] << endl;
  }*/
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
    exit(-1);
  }
  char* command = (char*)calloc(100, sizeof(char));
  int* label = (int*)calloc(10, sizeof(int));
  string* NameOfLabel = (string*)calloc(10,sizeof(string));
  try{
    int sizeLabel = ReadLabel(label, NameOfLabel, argv[1]);
    /*for(int i = 0; i < sizeLabel; i++){
      cout << NameOfLabel[i] << " == " << label[i] << endl;
    }*/
    int count = ReadCommand(command, label, NameOfLabel, sizeLabel, argv[1]);
    for(int i = 0; i < count; i++){
      cout << command[i] << endl;
    }
    CreateAsmFile(command, argv[2], count);
  }catch(invalid_argument& inv){
    cout << inv.what() << endl;
    exit(-1);
  }
  return 0;
}
