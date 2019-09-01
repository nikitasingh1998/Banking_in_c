
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include"bank.h"
int create_account(){

    int fd;
    unsigned long int accno;
    struct account *customer;
   // printf("%d",sizeof(struct account));
    struct stat buf;
    char password[21];
    char confirm[21];
    char name[10] = {0};
    char phone_no[11] = {0};

   /* if(argc != 3){
	printf("\nusage : create <name> <phno>\n");
	exit(1);
    }*/

   printf("Enter the name \n");
   scanf("%s",name);
   printf("enter the phone number \n");
   scanf("%s",phone_no);

    if(strlen(phone_no)!= 10){
	printf("\nenter the 10 digit ph no\n");
	exit(2);
    }

    if((fd=open("bankdata",O_CREAT | O_RDWR | O_APPEND,0600))==-1){
	printf("\ncannot create acc\n");
	exit(3);
    }

    customer=(struct account *)malloc(sizeof(struct account)); 

    customer->balance = 0;
    strcpy(customer->name,name);
    strcpy(customer->phno,phone_no);
back: printf("\nenter the password maximum 20 characters : ");

      system("stty -echo");

      scanf("%s",password);
      printf("\n%s\n",password);

      system("stty sane");

      if(strlen(password) > 20){
	  goto back;
      }
      printf("\nconfirm password :");

      system("stty -echo");
      
      scanf("%s",confirm);
      printf("\n%s\n",confirm);
      
      system("stty sane");
      
      if(strcmp(password,confirm) != 0){
	  printf("\npassword incorrect\n");
	  goto back;

      }

      strcpy(customer->password,password);

      if((write(fd,(struct account *)customer,sizeof(struct account))) == -1 ){
	  printf("\nerror on writing\n");
	  close(fd);
	  exit(5);
      }

      fstat(fd,&buf);
      accno=(buf.st_size/(sizeof(struct account)));

      close(fd);

      printf("\nthank you\n");
      printf("your acc no is %d\n\n",accno);


}
