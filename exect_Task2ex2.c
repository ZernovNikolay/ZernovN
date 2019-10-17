@@ -1,49 +0,0 @@
/*3
5 ls -al
3 pwd
2 echo Hello, world!*/ //пример файла

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>

void Split1(char *str, char** words, int* count){
	char* g = strchr(str, '\n');
	*g = '\0';
    words[*count] = strtok(str, " ");
    while(words[*count] != NULL){
	//printf("%s\n", words[*count]);
        words[++(*count)] = strtok(NULL, "@");
    }
    return;
}

int main(){
	char* str = (char*)calloc(102, sizeof(char));
	FILE* fp = fopen("Input.txt", "r");
	int g;
	char** command = (char**)calloc(100, sizeof(*command));
	fgets(str, 100, fp);
	g = atoi(str);
	//printf("%d\n", g);
	//char command [10][10];
	while(fgets(str, 100, fp) != NULL){
		int count = 0;
		Split1(str, command, &count);
		pid_t pid = fork();
		if(pid == 0){
			execlp(command[0], command[0], command[1], NULL);
		}else{
			sleep(5);
			kill(pid, 9);
		}
	}
	free(str);
	free(command);
	fclose(fp);
	return 0;
}
