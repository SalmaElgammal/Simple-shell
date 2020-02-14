#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#define MAX_CHAR 256
void interactiveMode();
char **parsing(char* str);
char *cdCommand(char *start,char **line);
void handler();
void background(char **strParts);
void process(char **strParts);

FILE *f;
int main()
{

    interactiveMode();
    return 0;
}

char *cdCommand(char *start,char **line){
    if(line[1]==NULL){
        chdir(getenv("HOME"));
    }else{
        if(chdir(line[1])!=0){
                perror("no such directory\n");
        }
    }
    return start;
}
void handler(){
    fprintf(f,"Child process was terminated\n");
}
void background(char **strParts){
     pid_t pid=fork();

        if(pid==0){
            if(execvp(strParts[0],strParts)==-1){
                perror("incorrect command in background\n");
            }else{
		signal(SIGCHLD,handler);		//handle the termination of the child

            }
        }
}
void process(char **strParts){
    pid_t pid=fork();

        if(pid==0){
            if(execvp(strParts[0],strParts)==-1){
                perror("incorrect command\n");
            }
        }else{
            waitpid(pid,NULL,0);
        }
}
void interactiveMode(){

    f=fopen("x.log","a");
    char command[MAX_CHAR];
    char **strParts;
    char *start="Shell>";
    while(1){
        printf("%s",start);
        char *flag=fgets(command,MAX_CHAR,stdin); 	  //read the command
        command[strlen(command)-1]='\0';		//change the char '\n' to '\0'
        if(strcmp(command,"exit")==0 || flag==NULL){
            exit(0);
        }
	if(command[0]=='c' && command[1]=='d' ){          // "change directory" command
		strParts=parsing(command);
		start=cdCommand(start,strParts);
	}
        else if(command[strlen(command)-1]=='&'){		//background command
            command[strlen(command)-1]='\0';
            strParts=parsing(command);
            background(strParts);
        }else{
            strParts=parsing(command);
            process(strParts);
        }

    }
}
char **parsing(char* str){

    char **parts;
    parts=malloc(64*sizeof(char*));
    char *line=strtok(str," ");
    int k=0;
    while(line){
        parts[k]=line;
        line=strtok(NULL," ");
        k++;
    }
    return parts;

}


