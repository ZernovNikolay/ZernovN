//переписать на файлы и на две программы
//добавить DUMP


//канарейки на процессор
#include <iostream>
#include <cstdint>
#include <string>
#include <cmath>
#include <cassert>
#include <cstddef>
using namespace std;
#define POISON -1;
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

class Stack{ //DUMP
public:

  Stack(){
    size = 0;
    capacity = 1;
    array = (type*)calloc(capacity+2, sizeof(type));
    array[0] = 86;
    array[capacity+1] = 86;
    hash = 0;
  };
  Stack(const int& gh){
    capacity = gh;
    size = 0;
    array = (type*)calloc(capacity+2, sizeof(type));
    array[0] = 86;
    array[capacity+1] = 86;
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
        Print();
        exit(0);
    }
  } //переписать с DUMP

  void checkhash(){
    size--;
    if (hash != Gethash()){
      size++;
      Dump();
      exit(0);
    }
    size++;
  }

  type out() const{
    return array[size];
  }

  void Print() const{
    cout << endl;
    cout << "STACK:" << endl;
    cout << array[0] << " = " << array[capacity+1] << endl;
    cout << capacity << " is capacity of our STACK" << endl;
    cout << size << " is size of our STACK" << endl;
    cout << hash << " is hash of our STACK" << endl;
    for(int i = 0; i < size; i++){
      cout << array[i+1] << " is String " << i << endl;
    }
    cout << endl;
  };

  void Dump() const{
    cout << endl;
    cout << "STACK:" << endl;
    cout << array[0] << " = " << array[capacity+1] << endl;
    cout << capacity << " is capacity of our STACK" << endl;
    cout << size << " is size of our STACK" << endl;
    cout << hash << " is hash of our STACK" << endl;
    for(int i = 0; i < capacity; i++){
      cout << array[i+1] << " is String " << i << endl;
    }
    cout << endl;
  }

  void push(const type& data1){
    canary();
    checkhash();
    if(size == capacity){
      capacity = capacity*2;
      array = (type*)realloc(array, capacity * sizeof(type));
      array[capacity+1] = array[0];
      for(int i = size+2; i < capacity+1; i++){
        array[i] = POISON;
      }
    }
    array[size+1] = data1;
    hash = Gethash();
    size++;
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

  size_t GetCap(){
    return capacity;
  }
private:
  type can1;
  type can2;
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
    cout << "REGISTER" << endl;
    cout << ax << " is ax register" << endl;
    cout << bx << " is bx register" << endl;
    cout << cx << " is cx register" << endl;
    cout << dx << " is dx register" << endl;
  }

  void Set(string gh, const type& num){
     if(gh == "ax") {
       ax = num;
     }else if(gh == "bx"){
       bx = num;
     }else if(gh == "cx"){
       cx = num;
     }else if(gh == "dx"){
       dx = num;
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
    cout << "INTEL" << endl;
  }

  void push(const string& gh){
    work.push(first.Get(gh));
  }

  void push(const type& gh){
    work.push(gh);
  }

  void pop(const string& gh){
    if(gh == "ax"){
        first.Set("ax", work.pop());
    }else if(gh == "bx"){
        first.Set("bx", work.pop());
    }else if(gh == "cx"){
        first.Set("cx", work.pop());
    }else if(gh == "dx"){
        first.Set("dx", work.pop());
    }
  }

  type Get(const string& gh) const{
    return first.Get(gh);
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
    cout << work.out() << endl;
  }

  void Gethash(){
    work.Gethash();
  }

  void Print() const{
    work.Print();
    first.PrintReg();
  }
private:
  Stack work;
  Register first;
};

void ReadCommand(char* command, const string* listof, const char* charof, const int& gh, const int& num){//переделать на считывание с файла и возврат int'a на количество команд
  string buff;
  char newCommand;
  string qw[num];
  for(int i = 0; i < num; i++){
    cin >> buff;
    qw[i] = buff;
    for(int k = 0; k < gh; k++){
      if(buff == listof[k]){
        command[i] = charof[k];
        break;
      }
    }
  }
}

void Execute(const char* command, Intel& core, const int& num){
  cout << "execute" << endl;
  for(int i = 0; i < num; i++){
    switch (command[i]) {
      case 'A':{
        int k = i+1;
        switch (command[k]){//проверять что после него что нибудь обязательно есть
          case 'H':
            core.push("ax");
            i++;
            break;
          case 'I':
            core.push("bx");
            i++;
            break;
          case 'J':
            core.push("cx");
            i++;
            break;
          case 'K':
            core.push("dx");
            i++;
            break;
          default:
            int gh;
            cin >> gh;
            core.push(gh);
            break;
        }
        break;
      }
      case 'B':
        i++;
        switch (command[i]) {
          case 'H':
            core.pop("ax");
            break;
          case 'I':
            core.pop("bx");
            break;
          case 'J':
            core.pop("cx");
            break;
          case 'K':
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
      default:
        cout << "HUINYA number " << i << endl;
        cout << command[i] << endl;
        exit(0);
    }
  }
}

int main(){
  /*string listof[] = {"push", "pop", "add", "sub", "div", "mul", "kv", "ax", "bx", "cx", "dx"};
  char charof[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K'};
  char* command = (char*)calloc(100, sizeof(char));*/
/*  ReadCommand(command, listof, charof, 11, num);
  Execute(command, core, num);*/
  Intel gh;
  gh.push(6);
  gh.push(8);
  gh.push(9);
  gh.push(6);
  gh.push(8);
  gh.push(9);
  gh.push(6);
  gh.push(8);
  gh.push(9);
  gh.push(6);
  gh.push(8);
  gh.push(9);
  return 0;
}
