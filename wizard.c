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
	time_t time_now;
	time(&time_now);
	
	pthread_mutex_lock(&print_mutex);
	printf("tid %u: ", (unsigned int)pthread_self());
	printf("second %d: ", (int)difftime(time_now, time_start));
	printf("%s ", wizard[wizard_id]);
	printf("(%s) ", wizard_type[wizard_id] == AUROR ? "Auror" : "DeathEater");
	printf("%d ", shop_patron[shop_id]);
	printf("%s\n", msg);
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
	int type = wizard_type[wizard_id];
	int *patron = &shop_patron[shop_id];
	char print_buf[MAX_PRINT];

	if(type == AUROR){
		(*patron)++;
	}else{
		(*patron)--;
	}
	
	sprintf(print_buf,
		"enters the %s shop.",
		shop[shop_id]);
	wizard_print(wizard_id, shop_id, print_buf);

}

void wizard_reserve_shop(int wizard_id, int shop_id){
	char print_buf[MAX_PRINT];
	sprintf(print_buf,
		"makes a reservation at %s shop. %d",
		shop[shop_id], shop_patron[shop_id]);
	wizard_print(wizard_id, shop_id, print_buf);
			
}


void wizard_init(void *id){
	int i;
	int reserving_shop;
	int shop_available;
	int *wizard_id_ptr = (int*)id;
	int wizard_id = *wizard_id_ptr;
	int left_shop = FALSE;
	char print_buf[MAX_PRINT];
	time_t time_now;
	time_t time_reserve;	
	
	reserving_shop = wizard_shop[wizard_id][0];
	
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
	
	for(i = 1; i < wizard_num_shop[wizard_id]; i++){
		/*
		reserving_shop = wizard_shop[wizard_id][0];
		time(&time_reserve);
		left_shop = FALSE;
		while(TRUE){
			time(&time_now);
			pthread_mutex_lock(&shop_mutex[reserving_shop]);
			
			if(wizard_check_shop(wizard_id, reserving_shop)){
					
			}
			
			pthread_mutex_unlock(&shop_mutex[serving_shop]);	
		}
		
		sleep(1);
		*/	
	}
	
	
	reserving_shop = wizard_shop[wizard_id][i - 2];
	pthread_mutex_lock(&shop_mutex[reserving_shop]);
	wizard_leave_shop(wizard_id, reserving_shop);
	sprintf(print_buf,
		"is done with shopping. %s leaves the %s shop.",
		wizard[wizard_id], shop[reserving_shop]);
	wizard_print(wizard_id, reserving_shop, print_buf);
	pthread_mutex_unlock(&shop_mutex[reserving_shop]);
	/*
	for(i = 0; i < wizard_num_shop[wizard_id]; i++){
		reserving_shop = wizard_shop[wizard_id][i];
		sprintf(print_buf, "reserving %s", shop[reserving_shop]);
		wizard_print(wizard_id, print_buf);
		while(1 == 1){
			if(i == 0){
				pthread_mutex_lock(&shop_mutex[reserving_shop]);
				if(wizard_type[wizard_id] == AUROR){
					sprintf(print_buf, 
						"try locking %s", shop[reserving_shop]);
					wizard_print(wizard_id, print_buf);
					if(shop_patron[reserving_shop] >= 0){
						shop_patron[reserving_shop]++;
						pthread_mutex_unlock(&shop_mutex[reserving_shop]);
						break;
					}
				}else{
				pthread_mutex_unlock(&shop_mutex[reserving_shop]);
				break;	
					
					if(shop_patron[reserving_shop] <= 0){
						shop_patron[reserving_shop]++;
						pthread_mutex_unlock(&shop_mutex[reserving_shop]);
						break;
					}
					
				}
				pthread_mutex_unlock(&shop_mutex[reserving_shop]);
				continue;
			}
			
			break;
		}	
			sprintf(print_buf, "here %s", shop[reserving_shop]);
			wizard_print(wizard_id, print_buf);
	}
	*/	

}


