#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define NUMBER_OF_STRINGS 1024
#define STRING_LENGTH 1023

char**  tokens [NUMBER_OF_STRINGS][STRING_LENGTH];
char ** history [NUMBER_OF_STRINGS][STRING_LENGTH];

int historyId =0;


// get line from user input
char * getlinee(void) {
    char * line = malloc(200), * linep = line;
    size_t lenmax = 200, len = lenmax;
    int c;

    if(line == NULL)
        return NULL;

    for(;;) {
        c = fgetc(stdin);
        if(c == EOF)
            break;

        if(--len == 0) {
            len = lenmax;
            char * linen = realloc(linep, lenmax *= 2);

            if(linen == NULL) {
                free(linep);
                return NULL;
            }
            line = linen + (line - linep);
            linep = linen;
        }
       

        if((*line++ = c) == '\n')
            break;
    }
    //printf("%s",linep);
    *line = '\0';
    return linep;
}


//scans line and parses it for evaluation
char** scanLineConvertToTokens(){

    char* str=malloc(100);
    
    str= getlinee();
    str[strcspn(str, "\n")] = 0;
    strcpy(history[historyId],str);
    //printf("%s",history[historyId]);
    historyId++;

    
	char delim[] = " ";

	char *ptr = strtok(str, delim);
    int i =0;

	while(ptr != NULL)
	{
		strcpy(tokens[i],ptr);
        //printf("%s", ptr);
		ptr = strtok(NULL, delim);
        
        i++;

	}
    char** com=malloc(64*sizeof(char*));
    for(int k=0;k<i;k++){
        com[k]=tokens[k];
        //printf(":%s.:",com[k]);

    }
    com[i]=NULL;

    return com;

    
}


int main(int argc, char *argcv[]){
    
    //get username from environment variables
    char *username=getenv("USER");

    // loop that runs the shell,breaks when command is exit
    while (1) {
        
        printf("%s >>> ",username);
        //arguments for the execvp function 
        char** args=malloc(64*sizeof(char*));
        
        //parsed line to tokens so that we can edit arguments to execute 
        args=scanLineConvertToTokens();

        //only command that do not use execvp, therefore it does not call runExecvp() function
        if(strcmp(args[0],"dididothat")==0){

            //it prepares searching command from the given tokens
            char * searching=malloc(100*sizeof(char));
            int iteration=1;
            while(1){
                if(args[iteration]==NULL){
                    break;
                }
                strcat(searching,args[iteration]);
                strcat(searching," ");
                iteration++;
                
                
            }

           
            //there is quotes at beginning and the end, which should not exist  
            char *lineWithoutQuotes = malloc(strlen(searching));
            int i, j=0;
            if(searching[0] != '"')
                lineWithoutQuotes[j++] = searching[0];
            for(i =  1; i < strlen(searching); i++){
                if(searching[i] == '"' )
                    continue;
                lineWithoutQuotes[j++] = searching[i];
            }
            //there is an extra whitespace at the end of the line
            lineWithoutQuotes[j-1]='\0';

            
    
            //flag for dididothat
            int flagg=0;
            for(int k=0;k<15;k++){
                if(historyId-k<0){
                    break;
                }
                if(strcmp(history[historyId-k],lineWithoutQuotes)==0){
                    flagg=1;
                }
            }

            //prints the correct output for the given line
            if(flagg){
                printf("Yes\n");
            }else{
                printf("No\n");
                }
            

        }

        //execvp commands
        else{
        
        

        // Ifs for command
       
        if(strcmp(args[0],"listdir")==0){
            
            args[0]="ls";
           

        }


        if(strcmp(args[0],"mycomputername")==0){
        
            strcpy(args[0],"hostname");
            
           

        }
        
        // hostname -I is the right command for my Xubuntu Virtual-Box
        if(strcmp(args[0],"whatsmyip")==0){
        
            strcpy(args[0],"hostname");
            args[1]="-I";
            args[2]=NULL;

        }
        // in printfile there are two options available, in one of them we print the file line by a line 
        // in another reads a file and redirect standart output to a new file
        if(strcmp(args[0],"printfile")==0){
            // this command prints line by line
            if(args[2]==NULL){
               strcpy(args[0],"more");
               args[2]=args[1];
               args[1]="-1";

            }
            else{
                
                //reads a file and redirect standart output to a new file
                if(strcmp(args[2],">")==0){
                    //in this function parameter format is different

                    char * arg2=malloc(100*sizeof(char));
                    int iteration=1;
                    strcpy(arg2,"cat");
                    strcat(arg2," ");

                    while(1){
                        if(args[iteration]==NULL){
                            break;
                        }
                        strcat(arg2,args[iteration]);
                        strcat(arg2," ");
                        iteration++;
                        
                        
                    }
                    int length=strlen(arg2);
                    //removes the whitespace at the end
                    arg2[length-1]='\0';
                    //trimTrailing(arg2);

                    args[0]="/bin/sh";
                    args[1]= "-c";
                    args[2]=arg2;
                    args[3]=NULL;
                


                }
             }
        }

        //opens the default text editor
        if(strcmp(args[0],"hellotext")==0){

            // for my Xubuntu virtual box, editor is the right command           
            //strcpy(args[0],"editor");
            
            strcpy(args[0],"gedit");
            
            args[1]=NULL;

        }
        // terminates the program
        if(strcmp(args[0],"exit")==0){
        
            
            return 0;

        }
        
        runExecvp(args);

        }
        free(args);
        
        
    }

    return 0;
}

//forks and executes the necessary commands 
void runExecvp(char ** args2){
    pid_t child_pid;
    int stat_loc;

    child_pid = fork(); 
    
        if (child_pid==0) {
            /* Never returns if the call is successful */
            execvp(args2[0], args2);
            //printf("This won't be printed if execvp is successul\n");
        } else {
            if(child_pid<0){
                printf("ERROR");
            }else{
                //printf("%d",child_pid);
                waitpid(child_pid, &stat_loc, WUNTRACED);
            }
        }

}