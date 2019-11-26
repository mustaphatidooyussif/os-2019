#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <pthread.h>
#include <ctype.h>

#define NTHREADS 10
#define COMMANDS 900


//creating a global array to keep track of the commands that 
//are being provided by the user
char *commands[900];
char *directories[900];
int num_of_commands = 0;
int num_elements_command = 0;
int num_of_paths = 0;
int num_paarsed_arguments = 0;
char *redirected_file_name =  NULL;
char error_message[40] = "The one and only error message.\n";
int num_parallel_cmds = 0;
int redirect_fd; 
char *parallel_commands_arr[900];

//the commandstruct structure is meant to contain the number of commands 
//and the array of commands
typedef struct __mycommands{

	  int num_of_builtin_cmd; 
	  char *builtin_commands[100];

	} __mycommands;


__mycommands builtins;

void initialize_builtin_commands(__mycommands * builtins, int num);
char *concat(char *s1,char *s2);
void path();
void cd(char *d);
char *strremove(char *str, const char *sub);
int execute_file();
int is_builtin_command(char *s);
void *execute_builtin_cmd(char *s);
void *execute_sys_cmd(char *arg[]);
char *read_input(FILE *f);
char *trim_space(char *str);
void *execute_command(void *arg);
int check_file(char *c);
int parallel_check_file(char *c);
int check_redirect(char *s);
void *redirection(char *s);
int check_parallel(char *s);
int split_input(char *s);
int parallel_commands();
void print_array(char *arr[]);
void reset_variables(void);
char *search_path(char *arr[]);
void signal_handler(int signal_code);
void current_directory();

//the main function of the wish command is going to simulate a shell program.
int main(int argc,char *argv[]){

	initialize_builtin_commands(&builtins, 3);

	directories[0] = "/bin/";  //initial shell path

	if (argc == 1){
		/*Shell is in interactive mode*/
		execute_command(NULL);

	}else if (argc == 2){
		/*Shell is in batch mode*/
		execute_command(argv[1]);

	}else{
		/*Error when more than one parameter passed  */

		fprintf(stderr, "program takes zero argument or one argument.\n\nUsage: program [optional]\n");
		exit(1);
	}
	
	return 0;
	}

//initialize_builtin_commands() initializes the builtin commands
void initialize_builtin_commands(__mycommands * builtins, int num){
	builtins->num_of_builtin_cmd = num;
	builtins->builtin_commands[0] = "exit";
	builtins->builtin_commands[1] = "cd";
	builtins->builtin_commands[2] = "path";
}

//concat() function is meant to concatenate 2 strings(i.e path and filename) into 1 string.
char *concat(char *s1,char *s2){
	strcat(s1, s2);
	return s1;
	}

 
//path() provides different paths that a file can be located in and executed.
void path(){
	int i  = 1;
	while (commands[i] != NULL){
		directories[i-1] = malloc(strlen(commands[i])+1);  
		strcpy(directories[i-1], commands[i]);
		i ++;
	}

	commands[i] = NULL; 
	}

//cd() changes the currennt directory of the wish bash shell
void cd(char *d){
	if(commands[2] != NULL ){  //more than one path specified
		fprintf(stderr, "Too many arguments\n");

	}else if(commands[1] == NULL){  //No path is passed.
		printf("cd takes a path as argument\n");
	}else{
		if(chdir(commands[1]) == 0){
			printf("\n");
		}else{
			fprintf(stderr, "wish: cd: %s : No such file or directory\n" , commands[1]);
		}
	}
	
    }
	
	
//strremove() takes out all of the newline characters from the
// inputs given by the user
char *strremove(char *str, const char *sub) {
      if(sub)  //if  sub is not only a newline, copy to str
		  strcpy(str, sub);

	  str[strlen(str)-1] = '\0'; //replace newline
	  return str;
	}



//execute_file() is used to execute the commands provided by the user
int execute_file(){
	
	}


//This function checks if the  command is a builtin or not
int is_builtin_command(char *s){

	int is_builtin = 0; 
	
	if(strcmp(s, builtins.builtin_commands[0])==0){ //exit
		is_builtin = 1;
	}else if(strcmp(s, builtins.builtin_commands[1])==0){ //cd
		is_builtin = 1;
	}else if(strcmp(s, builtins.builtin_commands[2])==0){ //path
		is_builtin =  1;
	}

	return is_builtin;
}

//execute_builtin_cmd() executes my built in commands.
void *execute_builtin_cmd(char *s){
	if(strcmp(s, builtins.builtin_commands[0]) == 0){ //exit
		exit(0);
	}else if(strcmp(s, builtins.builtin_commands[1]) == 0){ //cd
		cd(s);
	}else if(strcmp(s, builtins.builtin_commands[2]) == 0){ //path
		path();
	}

	return NULL;
}

//this searches the paths the user passed for the executable command. 
char *search_path(char *arr[]){
	int i = 0;
	char *exec_file  = NULL;
	while (arr[i] != NULL){

		if(check_file(arr[i]) == 1) { //if the file is there and is executable. 
			exec_file =  arr[i];
			return exec_file;
		}

		i++; 
	}

	return exec_file; 
}

//execute_sys_cmd() executes my system commands.
void *execute_sys_cmd(char *arg[]){
   pid_t pid;
   char *path;
   char *exec_file;
   char slash = '/';

   char *environment_path[900];
   int index =  0;

   while(directories[index] != NULL){
	   path = directories[index];

	   //allocate more space. 
	   exec_file = malloc(strlen(path)+ strlen(arg[0]) + 1);  //path + command e.g /bin/ls
	   strcpy(exec_file, path); 
	   
	   char last_char = exec_file[strlen(exec_file)-1];

		//if path has no slash at the end. 
	   if(last_char != slash){
	   		strncat(exec_file, &slash, 1); //add ending slash
	   }

	   //join path and command
	   exec_file = concat(exec_file, arg[0]);

       //add to executable paths
	   environment_path[index] = exec_file;
	   index ++; 
   }

	environment_path[index] = NULL;
	
	char *executable_path = search_path(environment_path); 

	if(executable_path != NULL){
		pid = fork();

		   if(pid < 0){
			    /*child process*/-
				fprintf(stderr, "Fork failed");
		   }else if(pid == 0){
				char **array = malloc(10 * sizeof(char*));

		   		array[0] = executable_path; 
		        
		   		for(int i=0; i < num_paarsed_arguments; i ++){
			   		array[i+1] = arg[i+1];
		   		}

				//redirect output or error to file, if redirect is set. 
				if(redirected_file_name != NULL){
					redirect_fd = open(redirected_file_name, O_WRONLY|O_CREAT, 0666);
					
					if(dup2(redirect_fd, fileno(stdout)) == -1){
						fprintf(stderr, "connot redirect std output");

					}
					if(dup2(redirect_fd, fileno(stderr)) == -1){
						fprintf(stderr, "connot redirect std err");
					}
				}

				//execute system commands
		   		if(execv(executable_path, array) < 0){
			   		fprintf(stderr, "Cannot execute command\n");
			   			//exit(1);
		   		}
					
				free(array);

		   }else {
				wait(NULL);
				//close file here if redirection is set
				if(redirect_fd){
					fflush(stdout); 
				}
		   }
	}/*else{
		printf("No executable\n"); //Change error message
		printf("Remember to set path before running system commands.\n");
	}*/

	if(exec_file != NULL){
		free(exec_file); 
	}
	return NULL;
}

//read_input() reads the input provided by the user. 
char *read_input(FILE *f){
	char *input_buffer = NULL;
    size_t input_buffer_size = 0;
    ssize_t characters = getline(&input_buffer, &input_buffer_size, f);
	return input_buffer;
}

//trim_space() removes leadning and trailing spaces from string. 
char *trim_space(char *str){
	//char *start_str = strdup(str); 
	int str_len = 0;

	//remove leading space. 
	while(isspace((unsigned char)str[0])){ //if leading space
		str ++;
	}

	//remove trainling space
	str_len = strlen(str);

	if(str_len == 0){  
		return str;
	}

	char *end_str = strdup(str);
	while(isspace((unsigned char)end_str[str_len-1])){
		str_len --;
	}

	end_str[str_len] = '\0';  //replace newline
	return end_str;
}

//the execute_command() function is responsble for executing the command 
//and the parameters passed into the function.
void *execute_command(void *arg){

	if (arg){ //if file is passed, execyte batch mode. 
		char *filename = (char *)arg;

		FILE *batch_file = fopen(filename, "r");

		if(batch_file == NULL){
			fprintf(stderr, "Cannot open file");
			exit(EXIT_FAILURE);
		}

		char *command = NULL;
		size_t input_buffer_size = 0;
		char *str = NULL;
		char *parsed_cmd =  NULL;

		while (getline(&command, &input_buffer_size, batch_file) !=-1){ //TODO: Refactor this to use read_input fxn. 
			str = malloc( strlen( command ? command : "\n"));
		    
			if( str == NULL){
				fprintf(stderr, "Out of memory\n");
				exit(EXIT_FAILURE);
			}

			//check if > exist in string. 
			if(check_redirect(command) == 1){ 
				command = redirection(command);
			}

			//check if & exist (check for parallel commands)
			num_parallel_cmds = check_parallel(command);
			if(num_parallel_cmds > 1){
				//execute in parallel mode.
				printf("EXecute in parallel\n");
				parallel_commands();

			}else{
				parsed_cmd = strremove(str, command);
				num_paarsed_arguments = split_input(trim_space(parsed_cmd));

				//execute in single mode. 
				if(is_builtin_command(commands[0]) == 1){
					execute_builtin_cmd(commands[0]);
				}else if(strlen(commands[0]) > 0){
					execute_sys_cmd(commands);
				}

			}

			if(str != NULL)
				free(str);
		}
	}else{

		signal(SIGINT, signal_handler);

		do{
			current_directory();
			char *command = read_input(stdin);

			//remove newline character, leading and trailing space  before splitting. 
			//sub has length of command string or length of new line if no command is passed. 
			char *str = NULL;
			char *parsed_cmd = NULL;

			str = malloc( strlen( command ? command : "\n")); 

			if(str == NULL){
				fprintf(stderr, "Out of memory");
				exit(EXIT_FAILURE);
			}
			
			//check if > exist in string. 
			if(check_redirect(command) == 1){ 
				command = redirection(command);
			}

			//check if the command is multiple. ( > 0 for multiple)
			num_parallel_cmds = check_parallel(command);

			if(num_parallel_cmds > 1){
				//execute in parallel mode.
				parallel_commands();

			}else{
				//execute in single mode. 
				parsed_cmd = strremove(str, command);

				num_paarsed_arguments = split_input(trim_space(parsed_cmd));

				if(is_builtin_command(commands[0]) == 1){
					execute_builtin_cmd(commands[0]);
				}else if(strlen(commands[0]) > 0){
					execute_sys_cmd(commands);
				}
			}

			if(str != NULL)
				free(str);

		    reset_variables();
		}while(1);	
	}

	}


//check_file() function checks to see if a file can be found in any of the directories and if it is executable
int check_file(char *c){
 	 int is_exec = 0;
	 if(access(c, X_OK) == 0){
		 is_exec = 1;
	 }

	 return is_exec;

	}

//parallel_check_file() is responsible for checking whether a file provided through parallel commands is executable.
int parallel_check_file(char *c){
	
	}


//the check_redirect() function checks to see whether the commands that the user provides contains '>'
int check_redirect(char *s){
	int index = 0;
	int has_redirect = 0;
	char *cmd_cpy = strdup(s);
	if(strchr(cmd_cpy, '>') != NULL){
		has_redirect = 1;
	}

	return has_redirect; 
	}

//the redirect() function performs the redirects the output of the commands provided into the given file
void * redirection(char *s){

    char*tokens[700];

    char *str_cmd;
    str_cmd = strdup(s); 

    tokens[0] = strtok(str_cmd, ">");
    int m = 0;

    while(tokens[m]  != NULL){
        m ++;
        tokens[m] = strtok(NULL, ">");

    }

	//close(1);//Close stdout
	//dup2(fd, 1);

	redirected_file_name = tokens[1]; 

	return tokens[0];
	}

//the check_parallel() function is responsible to checkking if the command provided by the user involves the execution of parallel commands
int check_parallel(char *s){

    char *str_cmd;
    str_cmd = strdup(s);

    parallel_commands_arr[0] = strtok(str_cmd, "&");
    int m = 0;

    while(parallel_commands_arr[m]  != NULL){
        m ++;
        parallel_commands_arr[m] = strtok(NULL, "&");

    }

	return m;
	}
	
//split_input() function splits the input provided by the user into tokens and places 
//them in an array
int split_input(char *s){

  int num_arguments = 0;
  char *sep = " ";
  char *input_cmd, *input_string, *redirect_char = ">";

 input_string = strdup(s);  //Assign string. This way will prevent compiler warnings. 

  while((input_cmd = strsep(&input_string, sep)) != NULL){

	  commands[num_arguments] = input_cmd;
	  num_arguments ++;
  }

  commands[num_arguments] = NULL;
  return num_arguments - 1;
}
	
//the parallel_commands() function is responsible for performing the parallel commands provided by the user
int parallel_commands(){
	pid_t pids[num_parallel_cmds];
    int i;
    int n = num_parallel_cmds;

    /* Start children. */
    for (i = 0; i < num_parallel_cmds; ++i) {
		if ((pids[i] = fork()) < 0) {
			fprintf(stderr, "Cannot for child %d", i);
		} else if (pids[i] == 0) {

			char *parsed_cmd =  NULL;
			char *str = NULL;

			str = malloc( strlen( parallel_commands_arr[i] ? parallel_commands_arr[i] : "\n"));
			
			//remove new liine, trailing and leading space before parsing. 
			parsed_cmd = strremove(str, parallel_commands_arr[i]);

			num_paarsed_arguments = split_input(trim_space(parsed_cmd));

			if(is_builtin_command(commands[0]) == 1){
				execute_builtin_cmd(commands[0]);
			}else{
				execute_sys_cmd(commands);
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
	
//helper function to print content of string array
void print_array(char *arr[]){
	int index = 0; 
	while (arr[index] != NULL){
		printf("%s\n", arr[index]);
		index ++;
	}
}

void reset_variables(void){
	num_of_commands = 0;
	num_elements_command = 0;
	num_of_paths = 0;
	num_paarsed_arguments = 0;
	redirected_file_name =  NULL;
	num_parallel_cmds = 0;
	redirect_fd = 0; 
	//environment_path = NULL; 

}

//signal_handle handles CTRL-C, CTRL-D, and CTRL-D
void signal_handler(int signal_code){
	signal(SIGINT, signal_handler);
	fflush(stdout);
}

//Prints the current directory + /wish>. 
void current_directory(){
	char _cwd[900];
	if(getcwd(_cwd, sizeof(_cwd)) != NULL){
		printf("%s", _cwd);
		printf("/wish> ");
	}
}