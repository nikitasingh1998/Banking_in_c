#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include"bank.h"


int main(){
    int choice;
while(1){
    printf("enter the choice\n");
    printf(" 	   0 : create\n \
	    1 : deposit\n \
	    2 : withdraw\n \
	    3 : balance\n \
	    4 : change\n \
	    5 : Exit\n"
	  );
    scanf("%d",&choice);



    switch(choice){
	case 0:printf("create\n");
		create_account();
	       break;
	case 1:printf("deposit\n");
		deposit_amount();
	       break;
	case 2:printf("withdraw\n");
               withdraw_amount();
	       break;
	case 3:printf("balance\n");
	      display_data();
	       break;
	case 4:printf("chage\n");
		change_details();
	       break;
	case 5: printf("exit\n");
		exit(1);
    }
}
}

