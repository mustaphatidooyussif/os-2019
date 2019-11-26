#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <pthread.h>
#include <ctype.h>

int has_redirect;
char *directories[900];
char *file_name;
char *commands[900];


void exec_parallel_commands(char *s);
void exec_batch_mode(char *s);
void exec_interactive_mode(void);
char *skip_space(char *str);
void split_input(char *s);
void cd(char *commands[]);
void path(char **commands);
void exec_batch_mode(char *s);
void *system_commands(char **arg);

int main(int argc, char *argv[])
{
    directories[0] = "/bin";
    if (argc == 1 ){
        exec_interactive_mode();
    }else if(argc == 2){
        exec_batch_mode(argv[1]);
    }
    return 0;
}


void exec_parallel_commands(char *s){
    char *parallel_commands[900];
    char *str_cmd;
    str_cmd = strdup(s);

    parallel_commands[0] = strtok(str_cmd, "&");
    int m = 0;

    while(parallel_commands[m]  != NULL){
        m ++;
        parallel_commands[m] = strtok(NULL, "&");

    }

    pid_t pids[m];
    int i;
    int n = m;

    /* Start children. */
    for (i = 0; i < m; ++i) {
		if ((pids[i] = fork()) < 0) {
			fprintf(stderr, "Cannot for child %d", i);
		} else if (pids[i] == 0) {

			char *parsed_cmd =  NULL;
			char *str = NULL;

			str = malloc( strlen( parallel_commands[i] ? parallel_commands[i] : "\n"));
			
			//remove new liine, trailing and leading space before parsing. 
            parsed_cmd = parallel_commands[i];
            parsed_cmd[strlen(parsed_cmd)] = '\0';

			split_input(skip_space(parsed_cmd));

            if(strcmp(commands[0], "cd") == 0){
                //cd
                cd(commands);
            }else if(strcmp(commands[0], "path") == 0){
                //path
                path(commands);
            }else if(strcmp(commands[0], "exit") == 0){
                //exit
                exit(0);
            }else{
                //system command
                system_commands(commands);
            }
			
			exit(0);
		}
    }

    /* Wait for children to exit. */
    int status;
    pid_t pid;
    while (n > 0) {
		pid = wait(&status);
		--n;  
    }
}
//execute interactive mode commands
void exec_interactive_mode(void){

    do{
        printf("wish> ");
        char *buffer = NULL;
        size_t buffer_size = 0;
        getline(&buffer, &buffer_size, stdin);
   
        buffer[strlen(buffer)-1] = '\0';

        
	    if(strchr(buffer, '&') != NULL){
           exec_parallel_commands(buffer);

        }else{
            if(strchr(buffer, '>') != NULL){
		        has_redirect = 1;
                char*redirect_tokens[700];

                char *buffer_clone;
                buffer_clone = strdup(buffer); 

                redirect_tokens[0] = strtok(buffer_clone, ">");
                int m = 0;

                while(redirect_tokens[m]  != NULL){
                    m ++;
                    redirect_tokens[m] = strtok(NULL, ">");

                }

                file_name = redirect_tokens[1]; 

                buffer = redirect_tokens[0];

            }

            
            char *parsed_cmd;
            parsed_cmd = buffer;
            parsed_cmd[strlen(parsed_cmd)] = '\0';

            //char **commands = (char *)malloc(900 * sizeof(char *));
			split_input(skip_space(parsed_cmd));

        
            if(strcmp(commands[0], "cd") == 0){
                //cd
                cd(commands);
            }else if(strcmp(commands[0], "path") == 0){
                //path
                path(commands);
            }else if(strcmp(commands[0], "exit") == 0){
                //exit
                exit(0);
            }else{
                //system command
                system_commands(commands);
            }
        }
    }while (1);
}


void split_input(char *s){

  int num_arguments = 0;
  char *sep = " ";

  char *input_cmd, *input_string;

 input_string = strdup(s);  //Assign string. This way will prevent compiler warnings. 

  while((input_cmd = strsep(&input_string, sep)) != NULL){

	  commands[num_arguments] = input_cmd;
	  num_arguments ++;
  }

  commands[num_arguments] = NULL;
}


char *skip_space(char *str){
	//char *start_str = strdup(str); 
	int len = 0;

	//remove leading space. 
	while(isspace((unsigned char)str[0])){ //if leading space
		str ++;
	}

	//remove trainling space
	len = strlen(str);

	if(len == 0){  
		return str;
	}

	char *final = strdup(str);
	while(isspace((unsigned char)final[len-1])){
		len --;
	}

	final[len] = '\0';  //replace newline
	return final;
}

void path(char **commands){
	int i  = 1;
	while (commands[i] != NULL){
		directories[i-1] = malloc(strlen(commands[i])+1);  
		strcpy(directories[i-1], commands[i]);
		i ++;
	}

	commands[i] = NULL; 
	}


//cd() changes the currennt directory of the wish bash shell
void cd(char *commands[]){
	if(commands[2] != NULL ){  
		fprintf(stderr, "Too many arguments\n");

	}else if(commands[1] == NULL){  
		printf("cd takes a path as an argument\n");
	}else{
		if(chdir(commands[1]) == 0){
			printf("\n");
		}else{
			fprintf(stderr, "Error" , commands[1]);
		}
	}
	
    }

void *system_commands(char **arg){
   pid_t pid;
   char *path;
   char *executable;
   char slash = '/';

   int index =  0;
   while(directories[index] != NULL){
	   path = directories[index];

	   //allocate more space. 
	   executable = malloc(strlen(path)+ strlen(arg[0]) + 1);  //path + command e.g /bin/ls
	   strcpy(executable, path); 
	   
	   char last_char = executable[strlen(executable)-1];

		//if path has no slash at the end. 
	   if(last_char != slash){
	   		strncat(executable, &slash, 1); //add ending slash
	   }

	   //join path and command
       strcat(executable, arg[0]);

       if(access(executable, X_OK) == 0){
           pid = fork();

		   if(pid < 0){
			    /*child process*/-
				fprintf(stderr, "Fork failed");
		   }else if(pid == 0){
				char **array = malloc(10 * sizeof(char*));

		   		array[0] = executable; 
		        
                int num_commands =0;
                while(arg[num_commands] != NULL){
                    num_commands ++;
                }

		   		for(int i=0; i < num_commands; i ++){
			   		array[i+1] = arg[i+1];
		   		}

                if(has_redirect == 1){
                    //redirec output
                    int fd;
                    fd = open(file_name, O_WRONLY|O_CREAT, 0666);
					
					if(dup2(fd, fileno(stdout)) == -1){
						fprintf(stderr, "connot redirect std output");

					}
					if(dup2(fd, fileno(stderr)) == -1){
						fprintf(stderr, "connot redirect std err");
					}
                }

				//execute system commands
		   		execv(executable, array);
		   }else {
				wait(NULL);
		   }
       }

	   index ++; 
   }
	return NULL;
}

void exec_batch_mode(char *s){
		FILE *f = fopen(s, "r");

		if(f == NULL){
			fprintf(stderr, "Cannot open file");
			exit(EXIT_FAILURE);
		}

		char *buffer = NULL;
		size_t buffer_size = 0;
		char *str = NULL;
		char *parsed_cmd =  NULL;

		while (getline(&buffer, &buffer_size, f) !=-1){ 
            buffer[strlen(buffer)] = '\0';

            if(strchr(buffer, '&') != NULL){
                exec_parallel_commands(buffer);

            }else{

                if(strchr(buffer, '>') != NULL){
                    has_redirect = 1;
                    char*redirect_tokens[700];

                    char *buffer_clone;
                    buffer_clone = strdup(buffer); 

                    redirect_tokens[0] = strtok(buffer_clone, ">");
                    int m = 0;

                    while(redirect_tokens[m]  != NULL){
                        m ++;
                        redirect_tokens[m] = strtok(NULL, ">");

                    }

                    file_name = redirect_tokens[1]; 

                    buffer = redirect_tokens[0];

                }

                parsed_cmd = buffer;
                parsed_cmd[strlen(parsed_cmd)] = '\0';
                split_input(skip_space(parsed_cmd));

                if(strcmp(commands[0], "cd") == 0){
                    //cd
                    cd(commands);
                }else if(strcmp(commands[0], "path") == 0){
                    //path
                    path(commands);
                }else if(strcmp(commands[0], "exit") == 0){
                    //exit
                    exit(0);
                }else{
                    //system command
                    system_commands(commands);
                }
            }
		}
}