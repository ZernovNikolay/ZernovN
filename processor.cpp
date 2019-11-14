//зеализовать end +
//реализовать fork
//добавить out in в asn +
//написать проверку проверки на дохуя проверок +
//канарейки на процессор+
//ifcall как правильно писать условный переход, потеряны записи+
//бинарные файлы&
//jb >
// jn !=
//jm <
// je ==
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
#include <cerrno>
using namespace std;

const int POISON = -2'147'483'648;
const int CANARY = 86;

//ja stop
//jae above below equal not
typedef int type;

#define MEOW(x, y) x * y

class RAM{
public:
  RAM(){
    array = (type*)calloc(10, sizeof(type));
    size = 10;
    for(int i = 0; i < size; i++){
      array[i] = POISON;
    }
  };
  RAM(int gh){
    array = (type*)calloc(gh, sizeof(type));
    size = gh;
    for(int i = 0; i < size; i++){
      array[i] = POISON;
    }
  }

  ~RAM(){
    Print();
    free(array);
  }

  void Print() const{
    cout << endl << "RAM" << endl;
    for(int i = 0; i < size; i++){
      cout << array[i] << " is cell " << i << endl;
    }
  }

  void Set(type gh, const int& cell){
    array[cell] = gh;
  }

  type Get(const int& cell) const{
    return array[cell];
  }
private:
  type* array;
  size_t size;
};

/**
parameters
array
size
hash
capacity
*/

void PrintErrors(){
  if(errno != 0){
    perror("ERROR IS ");
    assert(errno == 0);
  }
}

class Stack{
public:

  Stack(){
    errno = 0;
    size = 0;
    capacity = 1;
    array = (type*)calloc(capacity+2, sizeof(type));//errno
    assert(array);
    array[0] = 86;
    array[capacity+1] = 86;
    hash = 0;
    PrintErrors();
    //cout << &size << " " << &capacity << " " << &hash << endl;
  };
  Stack(const int gh){
    assert(gh > 0);
    errno = 0;
    capacity = gh;
    size = 0;
    array = (type*)calloc(capacity+2, sizeof(type));
    assert(array);
    array[0] = CANARY;
    array[capacity+1] = CANARY;
    hash = 0;
    for(int i = 1; i < capacity+1; i++){
      array[i] = POISON;
    }
    PrintErrors();
  };

  ~Stack(){// destruction of Stack, write all information of it and free all memory
    Dump();
    free(array);
  }

  bool empty() const{//check stack for function pop
    return !(size == 0);
  }

  void canary() const{//give check of security
    if(!(array[0] == array[capacity+1])){
        cout << "Canaries are't equal" << endl;
        Dump();
        exit(0);
    }
  }

  void checkhash(){
    if (hash != Gethash()){
      Dump();
      exit(-1);
    }
  }

  void CheckStack(){
    canary();
    checkhash();
  }

  type out() const{
    return array[size];
  }

  void Dump() const{
    cout << endl;
    cout << "STACK:" << endl;
    if(errno != 0){
      perror("With ERROR ");
    }else{
      cout << "Without ERROR" << endl;
    }
    cout << "Canaries are " << array[0] << " and " << array[capacity+1] << " .";
    if(array[0] == array[capacity+1]){
      cout << "They're equal" << endl;
    }else{
      cout << "They're not equal" << endl;
    }
    cout << capacity << " is capacity of your STACK" << endl;
    if(capacity < size){
      cout << "MEMORY OF STACK WAS BROKEN, size less than capacity" << endl;
    }else if(capacity < 0){
      cout << "MEMORY OF STACK WAS BROKEN, capacity less 0" << endl;
    }
    cout << size << " is size of your STACK" << endl;
    if(size < 0){
      cout << "STACK WAS BROKEN ON SIZE" << endl;
    }
    if(hash != Gethash()){
      cout << "HASH WAS BROKEN" << endl;
    }else{
      cout << "HASH IS RIGHT" << endl;
    }
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
    CheckStack();
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
    CheckStack();
    assert(empty());
    size--;
    hash = Gethash();
    return array[size+1];
  }

  int64_t Gethash() const{
    int64_t gh = 0;
    int64_t p = 3;
    for(int i = 1; i<size+1; i++){
      gh = gh ^ (int64_t)array[i]*p;
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
  Intel(){
    canary1 = CANARY;
    canary2 = CANARY;
    work = new Stack;
    call = new Stack;
    first = new Register;
    memory = new RAM;
  }

  ~Intel(){
    cout << endl << "INTEL:" << endl;
    cout << "CANARY ARE " << canary1 << " and " << canary2;
    if(canary1 == canary2){
      cout << ". THEY ARE EQUAL" << endl;
    }else{
      cout << ". THEY ARE NOT EQUAL" << endl;
    }
    delete work;
    delete call;
    delete first;
    delete memory;
  }

  void push(const string& gh){
    Check();
    try{
      work->push(first->Get(gh));
    }catch(invalid_argument& inv){
        throw inv;
      }
  }

  void push(const type& gh){
    Check();
    work->push(gh);
  }

  void pushRAM(const int& gh){
    Check();
    work->push(memory->Get(gh));
  }

  void pop(const string& gh){
    Check();
    try{
      first->Set(gh, work->pop());
    }catch(invalid_argument& inv){
      throw inv;
    }
  }

  void popRAM(const int gh){
    Check();
    memory->Set(work->pop(), gh);
  }

  type Get(const string& gh) const{
    Check();
    try{
      type num = first->Get(gh);
      return num;
    }catch(invalid_argument& inv){
      throw inv;
    }
  }

  void mul(){
    Check();
    pop("ax");
    pop("bx");
    push(Get("ax")*Get("bx"));
  }

  void add(){
    Check();
    pop("ax");
    pop("bx");
    push(Get("ax")+Get("bx"));
  }

  void sub(){
    Check();
    pop("ax");
    pop("bx");
    push(Get("ax")-Get("bx"));
  }

  void div(){
    Check();
    pop("ax");
    pop("bx");
    push(Get("ax")/Get("bx"));
  }

  void kv(){
    Check();
    pop("ax");
    push(sqrt(Get("ax")));
  }

  void in(){
    Check();
    type k;
    cin >> k;
    push(k);
  }

  void out(){
    Check();
    cout << "Result is " << work->out() << endl;
  }

  void Gethash(){
    Check();
    work->Gethash();
  }

  void Print() const{
    cout << endl << "INTEL:" << endl;
    cout << "CANARY ARE " << canary1 << " and " << canary2;
    if(canary1 == canary2){
      cout << ". THEY ARE EQUAL" << endl;
    }else{
      cout << ". THEY ARE NOT EQUAL" << endl;
    }
    first->PrintReg();
    work->Dump();
    call->Dump();
    memory->Print();
  }

  void pushCall(int gh){
    Check();
    call->push(gh);
  }

  type popCall(){
    Check();
    return call->pop();
  }

  void copy(const string& gh){
    Check();
    first->Set(gh, work->out());
  }

  void Check() const{
    if(canary1 != canary2){
        cout << "Canaries of INTEL are't equal" << endl;
        exit(-1);
    }
  }
private:
  int canary1;
  Stack* work;
  Stack* call;
  Register* first;
  RAM* memory;
  int canary2;
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
    //cout << command[i] << " is command " << i << endl;
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
      case 'a':
        i++;
        core.pushRAM(static_cast<int>(command[i]) - 32);
        break;
      case 'B':
        i++;
        cout << "here" << endl;
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
          default:
            cout << "wtf" << endl;
            exit(-1);
            break;
        }//поставить дефолт на то что ввели не то
        break;
      case 'b':
        i++;
        core.popRAM(static_cast<int>(command[i]) - 32);
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
        if(core.Get("dx") != (static_cast<int>(command[i])-32)){
          core.pushCall(i-2);
          //cout << i << " is old adress" << endl;
          i = static_cast<int>(command[i+1])-33;
          //cout << i << " is adress" << endl;
        }else{
          //cout << "PIZDEC" << endl;
          i++;
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
      case 'P':
        i++;
        if(core.Get("dx") > (static_cast<int>(command[i])-32)){
          core.pushCall(i-2);
          //cout << i << " is old adress" << endl;
          i = static_cast<int>(command[i+1])-33;
          //cout << i << " is adress" << endl;
        }else{
          //cout << "PIZDEC" << endl;
          i++;
        }
        break;
      case 'R':
        i++;
        if(core.Get("dx") < (static_cast<int>(command[i])-32)){
          core.pushCall(i-2);
          //cout << i << " is old adress" << endl;
          i = static_cast<int>(command[i+1])-33;
          //cout << i << " is adress" << endl;
        }else{
          //cout << "PIZDEC" << endl;
          i++;
        }
        break;
      case 'S':
        i++;
        if(core.Get("dx") == (static_cast<int>(command[i])-32)){
          core.pushCall(i-2);
          //cout << i << " is old adress" << endl;
          i = static_cast<int>(command[i+1])-33;
          //cout << i << " is adress" << endl;
        }else{
          //cout << "PIZDEC" << endl;
          i++;
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
  int size = ReadCommand(command, argv[1]);//поменять на 2, если сделаю через форк*/
  /*cout << size << endl;
  for(int i = 0; i<size; i++){
    cout << command[i];
  }
  cout << endl;*/

  try{
    Execute(command, core, size);
  }catch(invalid_argument& inv){
    cout << inv.what() << endl;
    return 1;
  }
  /*errno = 0;
  Intel work;
  work.push(5);
  work.popRAM(7);
  work.pushRAM(7);*/
  return 0;
}
