#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include <iostream>

using namespace std;

/**
@mainpage Second project: Eugeniy Onegin
*/
char* ReadFile (FILE* input, int fileLength);
int LengthOfFile (FILE* input);
char** FindEndOfString (char* originaltext, int fileLength, int* numberOfString);
char** Sort (char** adress, int numberOfString);
char** CopyOfMassiv (char** adress, int numberOfString);
int ParseInt(bool bis);
void DeleteExtraSymbols(const char* str, int* Abegin, const int* i, const int* length);
bool EndSymbol(const int* Abegin, const int* Bbegin, int* flag);
int StringCmp(const void* a, const void* b);
void DeleteBackSymbols(const char* str, int* Aend, const int* i, const int* length);
int StringBackCmp(const void* a, const void* b);
void Print (char** copyText, int numberOfString, FILE* output);

int main ()
    {
    char* originaltext = NULL;
    char** adress = NULL;
    char** copyText = NULL;
    FILE* input = fopen ("Input.txt", "r");//b
    int numberOfString = 0;
    FILE* output = fopen ("output.txt", "w");

    int fileLength = LengthOfFile (input);
    originaltext = ReadFile (input, fileLength);
    //cout << "haha" << endl;
    adress = FindEndOfString (originaltext, fileLength, &numberOfString);
    //cout << "haha2" << endl;
    copyText = CopyOfMassiv (adress, numberOfString);
    //cout << numberOfString << " rjkbxtcndj" << endl;

    /*for (int i = 0; i < numberOfString; i++)
    for (;copyText[i][0] == ' ';copyText[i]++)*/
    //cout << "3" << endl;
    //Print (adress, numberOfString, output);
    //cout << "4" << endl;
    qsort(copyText, numberOfString, sizeof(char*), StringCmp);
    //cout << "Sorted" << endl;
    //cout << "4" << endl;
    Print (copyText, numberOfString, output);

    qsort(copyText, numberOfString, sizeof(char*), StringBackCmp);

    fprintf (output,"\n\nBack Sorted\n\n");
    Print (copyText, numberOfString, output);

    fprintf (output,"\n\nOriginal Text\n\n");
    Print (adress, numberOfString, output);
    cout << "OK!" << endl;

    free (originaltext);
    free (adress);
    free (copyText);

    fclose (input);
    fclose (output);
    }

int LengthOfFile (FILE* input)
    {
    assert (input != NULL);

    fseek (input, 0L, SEEK_END);//fstat
    int fileLength = ftell (input);
    fseek (input, 0L, SEEK_SET);

    assert (fileLength >= 0);

    return (fileLength);
    }

char* ReadFile (FILE* input, int fileLength)
    {
    assert (input != NULL);
    assert (fileLength >= 0);

    char* originaltext = (char*) calloc (fileLength+1, sizeof (char));
    fread (originaltext, fileLength, 1, input);

    return (originaltext);
    }

char** FindEndOfString (char* originaltext, int fileLength, int* numberOfString)
    {
    assert (originaltext != NULL);
    assert (fileLength >= 0);
    assert (numberOfString != NULL);

    originaltext[fileLength] = '\0';
    int counter = 0;
    for (int i = 0; i < fileLength; i++)
    {
      if (originaltext[i] == '\n'  && originaltext[i+1] != '\n')//was \0
      {
        counter++;
      }
    }

    char** adress = (char**) calloc (counter+2, sizeof (char*));
    *numberOfString = counter;
    //cout << counter << " counter" << endl;

    adress[0] = &originaltext[0];
    counter = 1;
    for (int i = 0; i < fileLength; i++){
          if (originaltext[i] == '\n')
              {
                while(originaltext[i+1] == '\n'){
                  originaltext[i] = '\0';
                  i++;
                }
                assert (counter <= *numberOfString);
                adress[counter++] = &originaltext[i + 1];//counter++
                originaltext[i] = '\0';
              }
        }

    //for (int i = 0; i < counter; i++)

    return (adress);
    }

char** CopyOfMassiv (char** adress, int numberOfString)
    {
    assert (adress != NULL);
    assert (numberOfString >= 0);

    char** Copy = (char**) calloc (numberOfString, sizeof(char*));
    int i = 0;
    for(i = 0; i < numberOfString; i++)
        {
        Copy[i] = adress[i];
        }
    return (Copy);
    }

int StringCmp(const void* a, const void* b)
    {
    assert(a != NULL);
    assert(b != NULL);

    int Abegin = 0;
    int Bbegin = 0;
    int flag = 0;

    const char* str1 = *(const char**)a;
    const char* str2 = *(const char**)b;
    int length = strlen(str1);
    if(strlen(str2) < length){
      length = strlen(str2);
    }

    for(int i = 0; i < length; i++){
      DeleteExtraSymbols(str1, &Abegin, &i, &length);
      DeleteExtraSymbols(str2, &Bbegin, &i, &length);
      bool bis = EndSymbol(&Abegin, &Bbegin, &flag);
      if(flag == 1){
        return ParseInt(bis);
      }else{
        if(tolower(str1[i + Abegin]) != tolower(str2[i + Bbegin])){
          return ParseInt(tolower(str1[i + Abegin]) > tolower(str2[i + Bbegin]));
        }
      }
    }
    return 0;
}

int ParseInt(bool bis){
  if(bis){
    return 1;
  }else{
    return -1;
  }
}

bool EndSymbol(const int* Abegin, const int* Bbegin, int* flag){
  if(*Abegin == -1){
    if(*Bbegin == -1){
      *flag = 1;
      return true;
    }else{
      *flag = 1;
      return false;
    }
  }else if(*Bbegin == -1){
    *flag = 1;
    return true;
  }
  return true;
}

void DeleteExtraSymbols(const char* str, int* Abegin, const int* i, const int* length){
  assert(str != NULL);
  //assert(*Abegin != )

  char gh = str[*i + *Abegin];
  //cout << gh << " is Symbol" << endl;
  while (!((gh <= 'z' && gh >= 'a') || ((gh <= 'Z') && (gh >= 'A')))) {
    (*Abegin)++;
    if(*i + *Abegin > *length-1){
      *Abegin = -1;
      return;
    }
    gh = str[*i + *Abegin];
    //int g = *Abegin;
    //cout << gh << " is 2symbol" << " " << g << endl;
  }
  return;
}

int StringBackCmp(const void* a, const void* b)
    {
    assert(a != NULL);
    assert(b != NULL);

    const char* str1 = *(const char**)a;
    const char* str2 = *(const char**)b;
    int len1 = strlen(str1);
    int len2 = strlen(str2);
    int Aend = 0;
    int Bend = 0;
    int length = 0;
    int flag = 0;

    if(len2 < length){
      length = len2;
    }else{
      length = len1;
    }
    for(int i = 0; i < length; i++){
      len1--;
      len2--;
      //cout << Aend << " ";
      DeleteBackSymbols(str1, &Aend, &len1, &length);
      //cout << Aend << endl;
      DeleteBackSymbols(str2, &Bend, &len2, &length);
      bool bis = EndSymbol(&Aend, &Bend, &flag);
      if(flag == 1){
        return ParseInt(bis);
      }else{
        if(tolower(str1[len1 - Aend]) != tolower(str2[len2 - Bend])){
          return ParseInt(tolower(str1[len1 - Aend]) > tolower(str2[len2 - Bend]));
        }
      }
    }
  }

void DeleteBackSymbols(const char* str, int* Aend, const int* i, const int* length ){
  char gh = str[*i - *Aend];
  /*cout << *i << endl;
  cout << *i << " " << *Aend << "   ";
  cout << gh << " is gh1" << endl;*/
  while(!( (gh <= 'z' && gh >= 'a') || (gh <= 'Z' && gh >= 'A') )){
    //cout << gh << " is gh2" << endl;
    (*Aend)++;
    if(*i - *Aend < 0){
      (*Aend) = -1;
      return;
    }
    gh = str[*i - *Aend];
  }
  //cout << *Aend << " JOKER" << endl;
}

void Print (char** copyText, int numberOfString, FILE* output)
    {
    assert (copyText != NULL);
    assert (numberOfString >= 0);
    assert (output != NULL);

    for (int i = 0; i < numberOfString; i++)
        {
        if (copyText[i][0] != '\0') fprintf (output, "%s\n", copyText[i]);
        }
    }
