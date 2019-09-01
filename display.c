#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include"bank.h"
int display_data(){

    int fd,n;
    int accno;
    struct account *customer;
    struct stat buf;
    int attempt=0;
    char password[21];

 /*   if(argc != 2){
	printf("\nusage : disp <accno>\n");
	exit(1);
    }*/

   printf("Enter the account number\n");
   scanf("%d",&n);

    fd=open("bankdata",O_RDONLY,0600);
    if(fd==-1){
	printf("\ncannot create acc\n");
	exit(2);
    }

    customer=(struct account *)malloc(sizeof(struct account)); 


  //  n=atoi(argv[1]);


    fstat(fd,&buf);
    accno = buf.st_size/(sizeof(struct account));

    if(n>accno){
	printf("\nacc no does not exists\n");
	exit(3);
    }

    lseek(fd,(n-1)*sizeof(struct account),SEEK_SET);

    if((read(fd,customer,sizeof(struct account)))==-1){
	printf("\ncannot read\n");
	close(fd);
	exit(4);
    }

back:
    attempt++;
    if(attempt >=5){
	printf("\nyou attempted maximum times\n\n");
	exit(5);
    }
    printf("\nenter the password :");

    system("stty -echo");

    scanf("%s",password);

    system("stty sane");

    printf("\tthe password: %s\n", customer->password);
    if(strcmp(customer->password,password) != 0){
	printf("\nplease enter correct password\n");
	goto back;
    }


    printf("\tthe name %s\n",customer->name);
    printf("\tphno is %s\n",customer->phno);

    fstat(fd,&buf);
    accno=(buf.st_size/(sizeof(struct account)));

    printf("\tyour acc no %d\n",n);
    printf("\tyour bal  %ld\n",customer->balance);

    printf("\tthank you\n");
    close(fd);

}

