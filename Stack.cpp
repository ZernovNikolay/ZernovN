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
#include <cstdio>
using namespace std;

const int POISON = -2'147'483'648;
const int CANARY = 86;

typedef int type;

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
    array[0] = CANARY;
    array[capacity+1] = CANARY;
    array[1] = POISON;
    hash = 0;
    canary1 = CANARY;
    canary2 = CANARY;
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
    canary1 = CANARY;
    canary2 = CANARY;
  };

  ~Stack(){// destruction of Stack, write all information of it and free all memory
    Dump();
    free(array);
  }

  bool empty() const{//check stack for function pop
    return !(size == 0);
  }

  int CheckCanary() const{//give check of security
    if(!(array[0] == array[capacity+1])){
        cout << "Canaries of Array are't equal" << endl;
        Dump();
        return 1;
    }else{
      return 0;
    }
  }

  int checkhash() const{
    if (hash != Gethash()){
      Dump();
      return 1;
    }else{
      return 0;
    }
  }

  int CheckCanaryStruct() const{
    if(canary1 != canary2){
      cout << "Canary of Stack are't equal" << endl;
      Dump();
      return 1;
    }else{
      return 0;
    }
  }

  void CheckStack() const{
    if(CheckCanaryStruct()){
      exit(-1);
    }
    if(CheckCanary()){
      exit(-1);
    }
    if(checkhash()){
      exit(-1);
    }
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
    cout << "Canaries of Stack are " << canary1 << " and " << canary2 << " .";
    if(canary1 == canary2){
      cout << "They're equal" << endl;
    }else{
      cout << "They're not equal" << endl;
    }
    if(array == NULL){
      cout << "Canaries of Array was broken and don't exist" << endl;
    }else{
      cout << "Canaries of Array are " << array[0] << " and " << array[capacity+1] << " .";
      if(array[0] == array[capacity+1]){
        cout << "They're equal" << endl;
      }else{
        cout << "They're not equal" << endl;
      }
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
    if(array != NULL){
      for(int i = 0; i < size; i++){
        cout << array[i+1] << " is String " << i << endl;
      }
      cout << "!!!!!!!!!! You're here !!!!!!!!!" << endl;
      for(int i = size; i < capacity; i++){
        cout << array[i+1] << " is String " << i << " number of POISON" << endl;
      }
    }else{
      cout << "Array was broken and don't exist" << endl;
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
  int canary1;
  type* array;
  size_t size;
  size_t capacity;
  int64_t hash;
  int canary2;
};

int main(){
  /*Stack* one = new Stack;
  one->push(5);
  one->push(7);
  one->push(8);
  one->push(9);
  delete one;*/
  Intel core;
  core.push(5);
  core.push(6);
  core.push(7);
  core.push(8);
  core.push(5);
  core.push(6);
  core.push(7);
  core.push(8);
  core.push(5);
  core.push(6);
  core.push(7);
  core.push(8);
  core.push(5);
  core.push(6);
  core.push(7);
  core.push(8);
  return 0;
}
