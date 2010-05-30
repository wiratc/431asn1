#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <stdlib.h>

#include <unistd.h>

#include "input.h"

#define FALSE 0
#define TRUE (!FALSE)

#define MAX_PRINT 1000

extern char shop[MAX_SHOP][MAX_NAME];
extern char wizard[MAX_WIZARD][MAX_NAME];

extern int wizard_type[MAX_WIZARD];
extern int wizard_shop[MAX_WIZARD][MAX_SHOP];
extern int wizard_num_shop[MAX_WIZARD];

extern int num_line;
extern int num_shop;

extern pthread_mutex_t shop_mutex[MAX_SHOP];
extern int shop_patron[MAX_SHOP];

extern pthread_mutex_t print_mutex;

extern time_t time_start;

void wizard_print(int wizard_id, int shop_id, char *msg){
	int i;
	time_t time_now;
	time(&time_now);
	
	/*
	a mutex is needed to prevent threads printing outputs same time.
	otherwise the output may be not readable.
	*/

	pthread_mutex_lock(&print_mutex);
	printf("tid %u: ", (unsigned int)pthread_self());
	printf("second %d: ", (int)difftime(time_now, time_start));
	printf("%s ", wizard[wizard_id]);
	printf("(%s) ", wizard_type[wizard_id] == AUROR ? "Auror" : "DeathEater");
	//printf("%d ", shop_patron[shop_id]);
	printf("%s\n", msg);
	
	/*	
	for(i = 0; i < num_shop; i++){
		printf("%20s\t%4d\n", shop[i], shop_patron[i]);
	}	
	*/

	pthread_mutex_unlock(&print_mutex);
}


int wizard_check_shop(int wizard_id, int shop_id){
	int type = wizard_type[wizard_id];
	int patron = shop_patron[shop_id];

	if(type == AUROR){
		if(patron >= 0){
			return TRUE;
		}	
	}else{
		if(patron <= 0){
			return TRUE;
		}
	}

	return FALSE;
}

int wizard_leave_shop(int wizard_id, int shop_id){
	int type = wizard_type[wizard_id];
	int *patron = &shop_patron[shop_id];

	if(type == AUROR){
		(*patron)--;
	}else{
		(*patron)++;
	}
}

int wizard_enter_shop(int wizard_id, int shop_id){
	char print_buf[MAX_PRINT];

	sprintf(print_buf,
		"enters the %s shop.",
		shop[shop_id]);
	wizard_print(wizard_id, shop_id, print_buf);

}

void wizard_reserve_shop(int wizard_id, int shop_id){
	char print_buf[MAX_PRINT];
	int *patron = &shop_patron[shop_id];
	int type = wizard_type[wizard_id];
	
	if(type == AUROR){
		(*patron)++;
	}else{
		(*patron)--;
	}
	
	sprintf(print_buf,
		"makes a reservation at %s shop.",
		shop[shop_id]);
	wizard_print(wizard_id, shop_id, print_buf);
			
}


void wizard_init(void *id){
	int i;
	int diff;
	int last_shop;
	int reserving_shop;
	int shop_available;
	int *wizard_id_ptr = (int*)id;
	int wizard_id = *wizard_id_ptr;
	int left_shop = FALSE;
	char print_buf[MAX_PRINT];
	time_t time_now;
	time_t time_reserve;	
	
	//trying to reserve the first shop
	reserving_shop = wizard_shop[wizard_id][0];
	last_shop = reserving_shop;
	while(TRUE){
		pthread_mutex_lock(&shop_mutex[reserving_shop]);
		
		if(wizard_check_shop(wizard_id, reserving_shop)){
			
			wizard_reserve_shop(wizard_id, reserving_shop);
			wizard_enter_shop(wizard_id, reserving_shop);

			pthread_mutex_unlock(&shop_mutex[reserving_shop]);
			break;
		}
		pthread_mutex_unlock(&shop_mutex[reserving_shop]);
	}
	
	sleep(1);
	
	//trying to reserve subsequent shops
	for(i = 1; i < wizard_num_shop[wizard_id]; i++){
		last_shop = wizard_shop[wizard_id][i - 1];
		reserving_shop = wizard_shop[wizard_id][i];
		time(&time_reserve);

		while(TRUE){
			
			//calculate time since the first attempt to lock a mutex
			time(&time_now);
			diff = difftime(time_now, time_reserve);
			
			//cannot enter the next shop in 3 seconds
			if(diff >= 3 && !left_shop){
				
				//leave the current shop and try to reserve the next shop
				
				left_shop = TRUE;
				
				pthread_mutex_lock(&shop_mutex[last_shop]);
				wizard_leave_shop(wizard_id, last_shop);
				sprintf(print_buf,
					"is bored talking to the salesperson. %s leaves the %s shop without a reservation for the next shop to go for a walk.",
					wizard[wizard_id], shop[last_shop]);
				wizard_print(wizard_id, last_shop, print_buf);
				pthread_mutex_unlock(&shop_mutex[last_shop]);
			}

			//pthread_mutex_lock(&shop_mutex[reserving_shop]);
			
			if(pthread_mutex_trylock(&shop_mutex[reserving_shop]) != 0){
				continue;			
			}

			//check if the next shop is occupied by the same type or is not occupied
			if(wizard_check_shop(wizard_id, reserving_shop)){
				wizard_reserve_shop(wizard_id, reserving_shop);
				
				if(!left_shop){
					pthread_mutex_lock(&shop_mutex[last_shop]);
					
					wizard_leave_shop(wizard_id, last_shop);
					sprintf(print_buf,
					"leaves the %s shop to apparate to the next destination.",
					shop[last_shop]);
					wizard_print(wizard_id, reserving_shop, print_buf);

					pthread_mutex_unlock(&shop_mutex[last_shop]);
				}
				
				wizard_enter_shop(wizard_id, reserving_shop);
				pthread_mutex_unlock(&shop_mutex[reserving_shop]);
				break;
			}
			
			pthread_mutex_unlock(&shop_mutex[reserving_shop]);	
			
		}
		
		sleep(1);
		last_shop = reserving_shop;	
	}
	
	pthread_mutex_lock(&shop_mutex[last_shop]);
	wizard_leave_shop(wizard_id, last_shop);
	sprintf(print_buf,
		"is done with shopping. %s leaves the %s shop.",
		wizard[wizard_id], shop[last_shop]);
	wizard_print(wizard_id, last_shop, print_buf);
	pthread_mutex_unlock(&shop_mutex[last_shop]);

}

