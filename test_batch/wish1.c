#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

//getline reads a line a character at a time
char *allocate_buffer(void)
{
        char *buffer = NULL;
        size_t size = 0;
        getline(&buffer, &size, stdin);
        return buffer;
}



//Parses the input line into constituent pieces
int len = 0;
char **divide_buffer(char *buffer)
{

    
    int size = 16;
    char **alloc_buffer = malloc(size * sizeof(char*));

    char *assignor = " \t\r\n";
    char *alloc_buffers = strtok(buffer, assignor);

    while (alloc_buffers != NULL) {
        alloc_buffer[len] = alloc_buffers;
        len++;

        if (len>= size) {
            size = (int) (size * 1.5);
            alloc_buffer = realloc(alloc_buffers, size * sizeof(char*));
        }

        alloc_buffers = strtok(NULL, assignor);
    }
     alloc_buffer[len] = NULL;
    return alloc_buffer;
}


char *paths[10];





//Executes all commands
int exec_everything()
{
	
		paths[0] ="/bin/";
		paths[1] ="/usr/bin/";
		int countpaths = 2;
		do{
	        printf("wish> ");

	        char *buffer=allocate_buffer();
	        char **array =divide_buffer(buffer);
		    if(strcmp(array[0],"cd")==0){
				if(array[1]==NULL){
			    fprintf(stderr, "expected argument to \"cd\"\n");

			   }
			   else{
			        if(chdir(array[1]) == 0){
			                printf("%s\n", "You have entered the folder");
			        }
			   }
			}
			else if(strcmp(array[0],"exit")==0){
				exit(0);
			}
			else if(strcmp(array[0],"path")==0){
				countpaths = 0;
				for(int i=1;i<len;i++){
					paths[i-1] = malloc(strlen(array[i])+1);
					strcpy(paths[i-1], array[i]);
					countpaths++;

				}
			}
			else{
					pid_t pid;

			        for (int i=0;i<countpaths;i++){
			        	char *path = paths[i];
			        	char *path1 = malloc(strlen(path)+strlen(array[0])+1);
			        	strcat(path1, path);
			        	strcat(path1, array[0]);
			        	if(access(path1, F_OK)==0){
			        		pid=fork();
			                if(pid==0){
			                        char *a[]={path1, array[1], array[2], array[3], array[4], array[5], array[6], array[7], array[8], NULL};
			                        execv(path1, a);
			                        printf("The exec did not work\n");
				            }
				        	pid=wait(NULL);

				        }

			        }
		        
				}
				len = 0;
		        free(buffer);
		        free(array);

		}while(1);
        
}

//Reads input from a batch file
int exec_batch(char *filename){
	char *buffer = NULL;
     ssize_t size = 0;
     FILE *file;
     file = fopen(filename,"r");
     while(getline(&buffer, &size, file)!=-1){
               
     		char **array =divide_buffer(buffer);
		    if(strcmp(array[0],"cd")==0){
				if(array[1]==NULL){
			    fprintf(stderr, "expected argument to \"cd\"\n");

			   }
			   else{
			        if(chdir(array[1]) == 0){
			                printf("%s\n", "You have entered the folder");
			        }
			   }
			}
			else{
					pid_t pid;

			        char *path="/bin/";
			         char *secondpath="/usr/bin/";

			        char *path1 = malloc(strlen(path)+strlen(array[0])+1);
			        char *path2 = malloc(strlen(secondpath)+strlen(array[0])+1);
			        strcat(path1, path);
			        strcat(path1, array[0]);
			        strcat(path2, secondpath);
			        strcat(path2, array[0]);


			        if(access(path1, F_OK)==0){
			        pid=fork();
			                if(pid==0){
			                        char *a[]={path1, array[1], array[2], array[3], array[4], array[5], array[6], array[7], array[8], NULL};
			                        execv(path1, a);
			                        printf("The exec did not work\n");
			                }
			        pid=wait(NULL);

			        }else if(access(path2, F_OK)==0){
			            pid=fork();
			                if(pid==0){
			                        char *a[]={path2, array[1], array[2], array[3], array[4], array[5], array[6], array[7], array[8], NULL};
			                        execv(path2, a);
			                        printf("The exec did not work\n");


			                pid=wait(NULL);

			        }

		        
				}
				len =0;
		        free(buffer);
		        free(array);

		}
	
		
}
}

//Runs all commands
int main(int argc, char **argv)
{


int rc;
if(argc==1){
        exec_everything();

}else if(argc==2){
        exec_batch(argv[1]);
}
}
