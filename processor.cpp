//зеализовать end +
//реализовать fork
//добавить out in в asn +
//написать проверку проверки на дохуя проверок +
//канарейки на процессор
//ifcall
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

#define POISON -1;
#define CANARY 86;
//in & out & add, arifm, push & pop
//jmp
//out последнее запушенное число
//programm counter, указывает на команды
//metka
//переменная с номером текущей команды
//массив с метками
//ja stop
//jae above below equal not
//массив привяок
// двойная компиляция для прыжков вперед
//еще один стек для call и ret это еще одни jump
typedef int type;

class Stack{
public:

  Stack(){
    size = 0;
    capacity = 1;
    array = (type*)calloc(capacity+2, sizeof(type));
    array[0] = 86;
    array[capacity+1] = 86;
    hash = 0;
    //cout << &size << " " << &capacity << " " << &hash << endl;
  };
  Stack(const int& gh){
    assert(gh > 0);
    capacity = gh;
    size = 0;
    array = (type*)calloc(capacity+2, sizeof(type));
    array[0] = CANARY;
    array[capacity+1] = CANARY;
    hash = 0;
    for(int i = 1; i < capacity; i++){
      array[i] = POISON;
    }
  };

  ~Stack(){
    Dump();
    free(array);
  }

  bool empty() const{
    return !(size == 0);
  }

  void canary() const{
    if(!(array[0] == array[capacity+1])){
        cout << "Canaries are't equal" << endl;
        Dump();
        exit(0);
    }
  } //переписать с DUMP

  void checkhash(){
    if (hash != Gethash()){
      Dump();
      exit(0);
    }
  }

  type out() const{
    return array[size];
  }

  void Dump() const{
    cout << endl;
    cout << "STACK:" << endl;
    cout << "Canaries are " << array[0] << " and " << array[capacity+1] << " .";
    if(array[0] == array[capacity+1]){
      cout << "They're equal" << endl;
    }else{
      cout << "They're not equal" << endl;
    }
    cout << capacity << " is capacity of your STACK" << endl;
    cout << size << " is size of your STACK" << endl;
    cout << hash << " is hash of your STACK" << endl;
    for(int i = 0; i < size; i++){
      cout << array[i+1] << " is String " << i << endl;
    }
    cout << "!!!!!!!!!! You're here !!!!!!!!!" << endl;
    for(int i = size; i < capacity; i++){
      cout << array[i+1] << " is String " << i << " number of POISON" << endl;
    }
    cout << endl;
  }

  void push(const type& data1){
    assert(isfinite(data1));
    canary();
    checkhash();
    if(size == capacity){
      capacity = capacity*2;
      array = (type*)realloc(array, capacity * sizeof(type));
      array[capacity+1] = CANARY;
      for(int i = size+2; i < capacity+1; i++){
        array[i] = POISON;
      }
    }
    size++;
    array[size] = data1;
    hash = Gethash();
  }

  type pop(){
    canary();
    checkhash();
    assert(empty());
    size--;
    hash = Gethash();
    return array[size+1];
  }

  int64_t Gethash() const{
    int64_t gh = 0;
    int64_t p = 3;
    for(int i = 1; i<size+1; i++){
      gh = gh ^ static_cast<int64_t>(array[i])*p;
      p = p*3;
    }
    return gh;
  }

  size_t GetCapacity(){
    return capacity;
  }
private:
  type* array;
  size_t size;
  size_t capacity;
  int64_t hash;
};

class Register{
public:
  Register(){
    ax = POISON;
    bx = POISON;
    cx = POISON;
    dx = POISON;
  }

  ~Register(){
    PrintReg();
  }

  void PrintReg() const{
    cout << endl;
    cout << "REGISTER:" << endl;
    cout << ax << " is ax register" << endl;
    cout << bx << " is bx register" << endl;
    cout << cx << " is cx register" << endl;
    cout << dx << " is dx register" << endl;
  }

  void Set(string gh, const type& num){
    assert(isfinite(num));
     if(gh == "ax") {
       ax = num;
     }else if(gh == "bx"){
       bx = num;
     }else if(gh == "cx"){
       cx = num;
     }else if(gh == "dx"){
       dx = num;
     }else{
       stringstream ss;
       ss << "Unknown register: " << gh << endl;
       throw invalid_argument(ss.str());
     }
  }

  type Get(string gh) const{
     if(gh == "ax") {
       return ax;
     }else if(gh == "bx"){
       return bx;
     }else if(gh == "cx"){
       return cx;
     }else if(gh == "dx"){
       return dx;
     }else{
       stringstream ss;
       ss << "Unknown register: " << gh << endl;
       throw invalid_argument(ss.str());
     }
  }
private:
  type ax;
  type bx;
  type cx;
  type dx;
};

class Intel{
public:
  Intel(){}

  ~Intel(){
    cout << endl << "INTEL:" << endl;
  }

  void push(const string& gh){
    try{
      work.push(first.Get(gh));
    }catch(invalid_argument& inv){
        throw inv;
      }
  }

  void push(const type& gh){
    work.push(gh);
  }

  void pop(const string& gh){
    try{
      first.Set(gh, work.pop());
    }catch(invalid_argument& inv){
      throw inv;
    }
  }

  type Get(const string& gh) const{
    try{
      type num = first.Get(gh);
      return num;
    }catch(invalid_argument& inv){
      throw inv;
    }
  }

  void mul(){
    pop("ax");
    pop("bx");
    push(Get("ax")*Get("bx"));
  }

  void add(){
    pop("ax");
    pop("bx");
    push(Get("ax")+Get("bx"));
  }

  void sub(){
    pop("ax");
    pop("bx");
    push(Get("ax")-Get("bx"));
  }

  void div(){
    pop("ax");
    pop("bx");
    push(Get("ax")/Get("bx"));
  }

  void kv(){
    pop("ax");
    push(sqrt(Get("ax")));
  }

  void in(){
    type k;
    cin >> k;
    push(k);
  }

  void out(){
    cout << "Result is " << work.out() << endl;
  }

  void Gethash(){
    work.Gethash();
  }

  void Print() const{
    cout << endl << "INTEL:" << endl;
    first.PrintReg();
    work.Dump();
    call.Dump();
  }

  void pushCall(int gh){
    call.push(gh);
  }

  type popCall(){
    return call.pop();
  }

  void copy(const string& gh){
    first.Set(gh, work.out());
  }
private:
  Stack work;
  Stack call;
  Register first;
};

int ReadCommand(string& command, char* argv){
  assert(&command != NULL);
  assert(argv != NULL);
  ifstream input(argv);
  if(!input.is_open()){
    stringstream ss;
    ss << "Failed to open file with name" << argv;
    throw invalid_argument(ss.str());
  }
  getline(input, command);
  return command.size();
}

void Execute(const string& command, Intel& core, const int& num){
  /*cout << "execute of " << num << " command" << endl;
  for(int i = 0; i < num; i++){
    cout << command[i] << endl;
  }*/
  //cout << command[18] << endl;
  for(int i = 0; i < num; i++){
    switch (command[i]) {
      case 'A':
        i++;
        switch (command[i]){//проверять что после него что нибудь обязательно есть
          case 'W':
            core.push("ax");
            break;
          case 'X':
            core.push("bx");
            break;
          case 'Y':
            core.push("cx");
            break;
          case 'Z':
            core.push("dx");
            break;
          default:
            core.push(static_cast<int>(command[i]) - 32);
            break;
        }
        break;
      case 'B':
        i++;
        switch (command[i]) {
          case 'W':
            core.pop("ax");
            break;
          case 'X':
            core.pop("bx");
            break;
          case 'Y':
            core.pop("cx");
            break;
          case 'Z':
            core.pop("dx");
            break;
        }//поставить дефолт на то что ввели не то
        break;
      case 'C':
        core.add();
        break;
      case 'D':
        core.sub();
        break;
      case 'E':
        core.div();
        break;
      case 'F':
        core.mul();
        break;
      case 'G':
        core.kv();
        break;
      case 'H':
        cout << "Command is end" << endl;
        core.Print();
        exit(0);
      case 'I':
        i = static_cast<int>(command[i+1])-33;
        break;
      case 'J':
        core.out();
        //cout << "out" << endl;
        break;
      case 'K':
        core.pushCall(i-1);
        i = static_cast<int>(command[i+1])-33;
        cout << i << " is adress" << endl;
        break;
      case 'L':
        core.in();
        break;
      case 'M':
        i = core.popCall();
        break;
      case 'N':
        i++;
        switch (command[i]) {
          case '>':{
            i++;
            /*int gh = static_cast<int>(command[i])-32;
            cout << gh << " nAAAAAA" << endl;
            gh = core.Get("dx");
            if(gh < 0){
              cout << gh << " is register" << endl;
              exit(-1);
            }
            cout << gh << " is register" << endl;*/
            if(core.Get("dx") <= (static_cast<int>(command[i])-32)){
              //cout << "PIZDEC" << endl;
              i++;
            }else{
              core.pushCall(i-3);
              //cout << i << " is old adress" << endl;
              i = static_cast<int>(command[i+1])-33;
              //cout << i << " is adress" << endl;
            }
            break;
          }
          case '<':{
            i++;
            /*int gh = static_cast<int>(command[i])-32;
            cout << gh << " nAAAAAA" << endl;
            gh = core.Get("dx");
            if(gh < 0){
              cout << gh << " is register" << endl;
              exit(-1);
            }
            cout << gh << " is register" << endl;*/
            if(core.Get("dx") >= (static_cast<int>(command[i])-32)){
              //cout << "PIZDEC" << endl;
              i++;
            }else{
              core.pushCall(i-3);
              //cout << i << " is old adress" << endl;
              i = static_cast<int>(command[i+1])-33;
              //cout << i << " is adress" << endl;
            }
            break;
          }
          default:
            cout << "Unknown operand. This is " << command[i] << endl;
            exit(-1);
        }
        break;
      case 'O':
        i++;
        switch (command[i]) {
          case 'W':
            core.copy("ax");
            break;
          case 'X':
            core.copy("bx");
            break;
          case 'Y':
            core.copy("cx");
            break;
          case 'Z':
            core.copy("dx");
            break;
          }
        break;
      default:
        cout << "HUINYA number " << i << ". Command is " << command[i] << endl;
        exit(0);
    }
  }
  cout << command[18] << endl;
}

int main(int argc, char* argv[]){
  //cout << "HI" << endl;
  Intel core;
  string command;
  int size = ReadCommand(command, argv[1]);//поменять на 2, если сделаю через форк
  /*cout << size << endl;
  for(int i = 0; i<size; i++){
    cout << command[i];
  }
  cout << endl;*/
/*  try{
    core.push("kl");
  }catch(invalid_argument& inv){
    cout << inv.what() << endl;
    return 1;
  }*/
  try{
    Execute(command, core, size);
  }catch(invalid_argument& inv){
    cout << inv.what() << endl;
    return 1;
  }
  return 0;
}
