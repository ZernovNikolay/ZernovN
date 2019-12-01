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

#define PI 3.14159265

char *s = (char*)calloc(1000, sizeof(char));

float GetN(void);
float GetP(void);
float GetE(void);
float GetG(char* str);
float GetT(void);

int main(){
  char *str = (char*)calloc(1000, sizeof(char));
  scanf("%s", str);
  cout << GetG(str) << endl;
  return 0;
}

float GetN(){
  float value = 0;
  do{
    value = 10*value + *s - '0';
    s++;
  }while('0' <= *s && *s <= '9');
  return value;
}

float GetTrig(){
  //cout << *s << endl;
  while(*s == 's' || *s == 'c'){
    char operate = *s;
    float value = 0;
    cout << "HELLO" << endl;
    s = s+3;
    float value2 = GetP();
    //cout << "VALUE2 " << value2 << endl;
    if(operate == 's'){
      value = sin(value2 * PI/180);
    }else{
      value = cos(value2 * PI/180);
    }
    //cout << "VALUE " << value << endl;
    return value;
  }
  float value = GetP();
  return value;
}

float GetD(){
  float value = 0;
  float value3;
  value = GetTrig();
  while(*s == '^'){
    s++;
    value3 = 1;
    float value2 = GetTrig();
    //cout << "Value2 " << value2 << endl;
    for(float i = 0; i < value2; i++){
      value3 *= value;
    }
    value = value3;
  }
  return value;
}

float GetT(){//E call T call N
  float value = 0;
  value = GetD();
  //cout << value << " VAR2" << endl;
  while(*s == '*' || *s == '/'){
    char operate = *s;
    s++;
    float value2 = GetD();
    if(operate == '*'){
      value *= value2;
    }else{
      value /= value2;
    }
  }
  return value;
}

float GetE(){//E call T call N
  float value = 0;
  value = GetT();
  while(*s == '+' || *s == '-'){
    //cout << "E" << endl;
    char operate = *s;
    s++;
    float value2 = GetT();
    //cout << value2 << " VAR" << endl;
    if(operate == '+'){
      value += value2;
    }else{
      value -= value2;
    }
  }
  return value;
}

float GetP(){
  if(*s == '('){
    s++;
    float value = GetE();
    assert(*s == ')');
    s++;
    return value;
  }else{
    return GetN();
  }
}

float GetG(char* str){
  s = str;
  float value = GetE();
  if(*s != '#'){
    cout << "HYI " << *s << endl;
  }
  return value;
}
