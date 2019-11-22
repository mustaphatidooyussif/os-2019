#include <stdlib.h>
#include <stdio.h>
#include <string.h> 
#include <ctype.h>

void print_array(char *arr[]);
char *skipwhite(char *s);
char *trim_space(char *str);

int main(){

    char *string = malloc(10* sizeof(char));
    strcpy(string, "         mustapha      ");

    /*
    char slash = '/';

    //printf("%s \n", string);


    char*o_tokens[100];
    char *new_cmd = "ls -l &";

    char *str_cmd;
    str_cmd = strdup(new_cmd);

    o_tokens[0] = strtok(str_cmd, "&");
    int m = 0;

    while(o_tokens[m]  != NULL){
        m ++;
        o_tokens[m] = strtok(NULL, "&");

    }

    o_tokens[m] =  NULL;

    //printf("%s \n", strchr("ghana", '>'));
    if(o_tokens[m-1] == "\n"){
        printf("Hellow");
    }

    printf("%d", strlen(o_tokens[m-1]));

    printf("%s", o_tokens[m-1]);
    //print_array(o_tokens);
    */
    char *s = trim_space(string);

    printf("%s \n", string);
    printf("%s", s);

    return 0; 
}

void print_array(char *arr[]){
	int index = 0; 
	while (arr[index] != NULL){
		printf("%s\n", arr[index]);
		index ++;
	}
}

char *skipwhite(char *s){
    while(isspace(*s)) ++s;
    return s; 
}


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

    end_str[str_len] = '\0';
	return end_str;
}