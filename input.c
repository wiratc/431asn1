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

void input_init(){
	
	int i = 0;
	char line_buf[MAX_LINE];
	char *str_buf;
	char *wizard_ptr;
	char *new_line;
	char token[] = " ";
	FILE *fp = fopen("input.txt" ,"r");
	
	if(fp == NULL){
		//error
	}

	while(fgets(line_buf, MAX_LINE, fp) != NULL){
		//printf("%s", line_buf);
		if(line_buf[0] == '\n'){
			continue;
		}
		
		num_line++;
		
		new_line = strchr(line_buf, '\n');
		*new_line = '\0';
		
		str_buf = strtok(line_buf, token);
		strcpy(wizard[num_line -1], str_buf);


		//printf("%s\n", str_buf);
		str_buf = strtok(NULL, token);
		//printf("%s\n", str_buf);
	
		if(strcmp("DeathEater", str_buf) == 0){
			wizard_type[num_line - 1] = DEATH;
			//str_buf = strtok(NULL, token);
		}else{
			wizard_type[num_line - 1] = AUROR;
		}

				
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



