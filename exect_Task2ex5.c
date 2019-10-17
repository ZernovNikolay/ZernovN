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

void Split1(char *str, char** words, int* count){ //делим строку на 3 части: время, которое должно стоять в начале; команду и флаги или иные значения
	char* g = strchr(str, '\n');
	*g = '\0';
    words[*count] = strtok(str, " ");
	words[++(*count)] = strtok(NULL, " ");
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
	g = atoi(str);    //считали количество комманд в файле
	while(fgets(str, 100, fp) != NULL){
		int count = 0;
		Split1(str, command, &count);
		g = atoi(command[0]); //время задержки
		pid_t pid = fork();
		if(pid == 0){
			sleep(g);
			execlp(command[1], command[1], command[2], NULL); //должен быть метод будить родителя, но мне не удалось это сделать с SIGALRM
		}else{
			sleep(g+5);
			kill(pid, 9);
		}
	}
	printf("OK!\n");
	free(str);
	free(command);
	fclose(fp);
	return 0;
}
