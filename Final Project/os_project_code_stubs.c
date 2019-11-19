#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <pthread.h>

#define NTHREADS 10
#define COMMANDS 900


//creating a global array to keep track of the commands that 
//are being provided by the user
char *commands[900];
char *directories[900];
int num_of_commands = 0;
int num_elements_command = 0;
int num_of_paths = 0;

//the commandstruct structure is meant to contain the number of commands 
//and the array of commands
typedef struct __mycommands{

	  int num_of_builtin_cmd; 
	  char *builtin_commands[100];

	} __mycommands;


__mycommands builtins;

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
	printf("Path is called\n");

	//number of paths
    printf("%s, %s , %s %s \n", commands[0], commands[1], commands[2], commands[3]);

	/*Allocate memory for all the paths inside the directory array.*/
	for(int i=1;i <= num_of_paths; i++){
		directories[i-1] = malloc(strlen(commands[i])+1);  
		strcpy(directories[i-1], commands[i]);
	    }
	}
	
	
	
//cd() changes the currennt directory of the wish bash shell
void cd(char *d){
	printf(" Cd is called\n");

	if(commands[2] != NULL ){  //more than one path specified
		fprintf(stderr, "Too many arguments\n");

	}else if(commands[1] == NULL){  //No path is passed.
		printf("cd takes a path as argument\n");
	}else{
		if(chdir(commands[1]) == 0){
			printf("You are not dir: %s\n", commands[1]);
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


int is_builtin_command(void){

	int is_builtin = 0; 
	
	if(strcmp(commands[0], builtins.builtin_commands[0])==0){ //exit
		is_builtin = 1;
	}else if(strcmp(commands[0], builtins.builtin_commands[1])==0){ //cd
		is_builtin = 1;
	}else if(strcmp(commands[0], builtins.builtin_commands[2])==0){ //path
		is_builtin =  1;
	}

	return is_builtin;
}

//execute_builtin_cmd() executes my built in commands.
void *execute_builtin_cmd(void){
	if(strcmp(commands[0], builtins.builtin_commands[0]) == 0){ //exit
		exit(0);
	}else if(strcmp(commands[0], builtins.builtin_commands[1]) == 0){ //cd
		cd(commands[0]);
	}else if(strcmp(commands[0], builtins.builtin_commands[2]) == 0){ //path
		path();
	}

	return NULL;
}

//execute_sys_cmd() executes my system commands.
void *execute_sys_cmd(void *arg){
   printf("Exec syst cmd is called\n");

   pid_t pid;
   char *path;
   char *exec_file;
   
   printf("%d", num_of_paths);

   print_array(directories);

   for(int i = 0; i < num_of_paths; i++){
	   path = directories[i];
	   printf("%s ------------", path);
	   exec_file = malloc(strlen(path)+ strlen(commands[0]) + 1);  //path + command e.g /bin/ls
	   strcpy(exec_file, path); 
	   exec_file = concat(exec_file, commands[0]);
	   printf("%s", exec_file);


   }
	

return NULL;
}

char *read_input(void){
	char *input_buffer = NULL;
    size_t input_buffer_size = 0;
    getline(&input_buffer, &input_buffer_size, stdin);

	return input_buffer;
}

//trim_space() removes leadning and trailing spaces from string. 
char *trim_space(const char *str){
	//char *start_str = strdup(str); 
	int str_len = 0;

	//remove leading space. 
	while(isspace((unsigned char)str[0])){ //if leading space
		str ++;
	}

	//remove trainling space
	str_len = strlen(str);

	if(str_len == 0){  
		return str_len;
	}

	char *end_str = strdup(str);
	while(isspace((unsigned char)end_str[str_len-1])){
		str_len --;
	}

	return end_str;
}

//the execute_command() function is responsble for executing the command 
//and the parameters passed into the function.
void *execute_command(void *arg){
	if (arg){
		char *filename = (char *)arg;

		FILE *batch_file = fopen(filename, "r");

		if(batch_file == NULL){
			fprintf(stderr, "Cannot open file");
			exit(EXIT_FAILURE);
		}
	}else{
		do{
			int num_arguments;
			printf("wish> ");
			char *command = read_input();

			//remove newline character, leading and trailing space  before splitting. 
			//sub has length of command string or length of new line if no command is passed. 
			char *str = NULL;
			char *parsed_cmd = NULL;

			str = malloc( strlen( command ? command : "\n")); 

			if(str == NULL){
				fprintf(stderr, "Out of memory");
				exit(EXIT_FAILURE);
			}

			//trim_space()
			parsed_cmd = strremove(str, command);

			num_arguments = split_input(parsed_cmd);

			/*number of arguments are the same as paths (for the path command)*/

			if(is_builtin_command() == 1){
				execute_builtin_cmd();
			}else{
				execute_sys_cmd(NULL);
			}

			if(str != NULL)
				free(str);
		    
		}while(1);

		
	}

	}


//check_file() function checks to see if a file can be found in any of the directories and if it is executable
int check_file(char *c){
	
	}

//parallel_check_file() is responsible for checking whether a file provided through parallel commands is executable.
int parallel_check_file(char *c){
	
	}


//the check_redirect() function checks to see whether the commands that the user provides contains '>'
int check_redirect(){
	
	}

//the redirect() function performs the redirects the output of the commands provided into the given file
void redirection(){
		
	}

//the check_parallel() function is responsible to checkking if the command provided by the user involves the execution of parallel commands
int check_parallel(){
	
	}
	
//split_input() function splits the input provided by the user into tokens and places 
//them in an array
int split_input(char *s){

  int num_arguments = 0;
  char *sep = " ";
  char *input_cmd, *input_string;

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
	
	}
	
//helper function to print content of string array
void print_array(char *arr[]){
	int index = 0; 
	while (arr[index] != NULL){
		printf("%s\n", arr[index]);
		index ++;
	}
}

//the main function of the wish command is going to simulate a shell program.
int main(int argc,char *argv[]){

	initialize_builtin_commands(&builtins, 3);

	directories[0] = "/bin";  //initial shell path

	if (argc == 1){
		/*Shell is in interactive mode*/
		execute_command(NULL);

	}else if (argc == 2){
		/*Shell is in batch mode*/
		execute_command(argv[1]);

	}else{
		/*Error when more than one parameter passed  */

		fprintf(stderr, "Please type wish --help");
		exit(EXIT_FAILURE);
	}
	
	return 0;
	}
