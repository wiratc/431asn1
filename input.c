#include <stdio.h>
#include <string.h>

#include "input.h"

char shop[MAX_SHOP][MAX_NAME];
char wizard[MAX_WIZARD][MAX_NAME];

int wizard_type[MAX_WIZARD];
int wizard_shop[MAX_WIZARD][MAX_SHOP];
int wizard_num_shop[MAX_WIZARD];

int num_line;
int num_shop;

//find a shop index in the array by shop name
int input_shop_index(const char *shop_name){
	int i = 0;
	
	for(i = 0; i < num_shop; i++){
		if(strcmp(shop_name, shop[i]) == 0){
			return i;
		}
	}
	
	strcpy(shop[num_shop], shop_name);
	num_shop++;
	
	return num_shop - 1; 

}

//for debugging
void input_print_info(){
	int i, j;
	
	for(i = 0; i < num_line; i++){
		printf("name: %s\n", wizard[i]);
		//printf("type: %d\n", wizard_type[i]);
		printf("type: %s\n", wizard_type[i] == AUROR ? "auror" : "death");
		printf("shop to visit\n");
		for(j = 0; j < wizard_num_shop[i]; j++){
			printf("%d: %s\n", j + 1, shop[wizard_shop[i][j]]);
		}
		printf("\n");
	} 
	
}

//parsing in putdile
void input_init(char* file_name){
	
	int i = 0;
	char line_buf[MAX_LINE];
	char *str_buf;
	char *wizard_ptr;
	char *new_line;
	char token[] = " ";
	FILE *fp = fopen(file_name ,"r");
	
	if(fp == NULL){
		//error
	}

	//reading line by line
	while(fgets(line_buf, MAX_LINE, fp) != NULL){
		if(line_buf[0] == '\n'){
			continue;
		}
		printf("%s", line_buf);
	
		num_line++;
		
		//remove '\n' from the end of line
		new_line = strchr(line_buf, '\n');
		*new_line = '\0';
		
		//split by " "
		str_buf = strtok(line_buf, token);
		//wizard name
		strcpy(wizard[num_line -1], str_buf);


		//printf("%s\n", str_buf);
		
		//wizard type
		str_buf = strtok(NULL, token);
		//printf("%s\n", str_buf);
		
		//converting type to int
		if(strcmp("DeathEater", str_buf) == 0){
			wizard_type[num_line - 1] = DEATH;
			//str_buf = strtok(NULL, token);
		}else{
			wizard_type[num_line - 1] = AUROR;
		}

		//start getting shop name		
		str_buf = strtok(NULL, token);

		i = 0;
		while(str_buf != NULL){
			//printf("%s %d\n", str_buf, input_shop_index(str_buf));
			wizard_shop[num_line -1][i] = input_shop_index(str_buf);
			i++;
			str_buf = strtok(NULL, token);
		}
		
		wizard_num_shop[num_line -1] = i;
	}
	
	//printf("num_line: %d\n", num_line);
	
	
	return;

}



