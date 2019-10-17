#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

void Split(char *str, char *delimiters, char** words, int* count);

void GiveStringAndDelim(char *str, char *delimiters, FILE* text);

void FreeAllMemory(char *str, char* delimiters, char** words, FILE* text);

int main(){
	FILE* text = fopen("text.txt", "r");
	assert(text != 0);

	char* str = (char*)calloc(100, sizeof(*str));
	//strcpy(str, "Holy shit, when this hell will solve");
	//*str = "Holy shit, when this hell will solve";
	int count = 0;
	char *delimiters = (char*)calloc(100, sizeof(*delimiters));
	//printf("%s\n", str);
	//printf("%s\n", delimiters);
	char** words = (char**)calloc(100, sizeof(*words));
	GiveStringAndDelim(str, delimiters, text);
	Split(str, delimiters, words, &count);
	FreeAllMemory(str, delimiters, words, text);
    return 0;
}

void GiveStringAndDelim(char *str, char *delimiters, FILE* text){
	assert(str != 0);
	assert(delimiters != 0);

	fgets(str, 1000, text);
	fgets(delimiters, 1000, text);
}

void Split(char *str, char *delimiters, char** words, int* count){
    words[*count] = strtok(str, delimiters);
    while(words[*count] != NULL){
	printf("%s\n", words[*count]);
        words[++(*count)] = strtok(NULL, delimiters);
    }
    return;
}

void FreeAllMemory(char *str, char* delimiters, char** words, FILE* text){
	free(str);
	free(delimiters);
	free(words);
	fclose(text);
}
