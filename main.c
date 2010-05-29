#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#include "input.h"
#include "wizard.h"

extern int num_shop;
extern int num_line;

pthread_mutex_t shop_mutex[MAX_SHOP];
pthread_t wizard_thread[MAX_WIZARD];

int shop_patron[MAX_SHOP];

void init(){
	int i;
	for(i = 0; i < num_shop; i++){
		pthread_mutex_init(&shop_mutex[i], NULL);
	}
	
	for(i = 0; i < num_line; i++){
		int *wizard_id = malloc(sizeof(int));
		*(int *)wizard_id = i;
		pthread_create(&wizard_thread[i], NULL, wizard_init, (void *)wizard_id); 	
	}

	for(i = 0; i < num_line; i++){
		pthread_join(wizard_thread[i], NULL);
	}
}


int main(){
	input_init();
	input_print_info();
	init();
	return 0;
}
