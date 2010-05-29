#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <stdlib.h>

#include <unistd.h>

#include "input.h"

#define FALSE 0
#define TRUE (!FALSE)

extern char shop[MAX_SHOP][MAX_NAME];
extern char wizard[MAX_WIZARD][MAX_NAME];

extern int wizard_type[MAX_WIZARD];
extern int wizard_shop[MAX_WIZARD][MAX_SHOP];
extern int wizard_num_shop[MAX_WIZARD];

extern int num_line;
extern int num_shop;

extern pthread_mutex_t shop_mutex[MAX_SHOP];
extern int shop_patron[MAX_SHOP];


void *wizard_print(int wizard_id){
	time_t time_now;

	printf("tid %u: ", (unsigned int)pthread_self());
	printf("second %ld: ", clock() / CLOCKS_PER_SEC);
	printf("%s ", wizard[wizard_id]);
	printf("(%s) ", wizard_type[wizard_id] == AUROR ? "Auror" : "DeathEater");
	
}


int wizard_check_patron(int wizard_id, int shop_id){
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
	int patron = shop_patron[shop_id];

	if(type == AUROR){
		return patron - 1;
	}else{
		return patron + 1;
	}
}

int wizard_enter_shop(int wizard_id, int shop_id){
	int type = wizard_type[wizard_id];
	int patron = shop_patron[shop_id];

	if(type == AUROR){
		return patron + 1;
	}else{
		return patron - 1;
	}
}

wizard_reserve(int shop_id){
	
}


void wizard_init(void *id){
	int i;
	int reserving_shop;
	int *wizard_id_ptr = (int*)id;
	int wizard_id = *wizard_id_ptr;
	int start_time = 0;
	//printf("wizard_id %d", wizard_id);

	for(i = 0; i < wizard_num_shop[wizard_id]; i++){
		reserving_shop = wizard_shop[wizard_id][i];
		wizard_print(wizard_id);
		printf("reserving %s\n", shop[reserving_shop]);
		while(1 == 1){
			if(i == 0){
				pthread_mutex_lock(&shop_mutex[reserving_shop]);
				if(wizard_type[wizard_id] == AUROR){
			wizard_print(wizard_id);
			printf("try locking %s\n", shop[reserving_shop]);
					if(shop_patron[reserving_shop] >= 0){
						shop_patron[reserving_shop]++;
						pthread_mutex_unlock(&shop_mutex[reserving_shop]);
						break;
					}
				}else{
				pthread_mutex_unlock(&shop_mutex[reserving_shop]);
				break;	
					/*
					if(shop_patron[reserving_shop] <= 0){
						shop_patron[reserving_shop]++;
						pthread_mutex_unlock(&shop_mutex[reserving_shop]);
						break;
					}
					*/
				}
				pthread_mutex_unlock(&shop_mutex[reserving_shop]);
				continue;
			}
			
			break;
		}	
			wizard_print(wizard_id);
			printf("here %s\n", shop[reserving_shop]);
	}	

}


