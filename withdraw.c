#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include"bank.h"


int withdraw_amount(int argc,char *argv[]){

    int fd,n,amount,accno;
    struct account *customer;
    struct stat buf;
    char password[21];
    int attempt=0;

    fd = open("bankdata",O_RDWR,0600);

    if(fd==-1){
	printf("\nrecord cannot open\n");
	exit(1);
    }

   /* if(argc != 3){
	printf("\nusage : withdraw  <accno> <amount>\n");
	close(fd);
	exit(2);
    }

    n=atoi(argv[1]);
    amount=atoi(argv[2]);*/

   printf("Enter the Account number \n");
   scanf("%d",&n);
   printf("Enter the Amount \n");
   scanf("%d",&amount);

    if(amount<0){
	printf("\ncannot withdraw negetive amount\n");
	exit(3);
    }

    fstat(fd,&buf);
    accno = buf.st_size/(sizeof(struct account));

    if(n>accno){
	printf("\nacc no does not exists\n");
	exit(4);
    }

    customer=(struct account *)malloc(sizeof(struct account));

    lseek(fd,(n-1)*sizeof(struct account),SEEK_SET);

    lockf(fd,F_LOCK,sizeof(struct account));

    if((read(fd,(struct account *)customer,sizeof(struct account)))==-1){
	printf("cannot read\n");
	lockf(fd,F_ULOCK,-sizeof(struct account));
	close(fd);
	exit(5);
    }

back:
    attempt++;
    if(attempt >=5){
	printf("\nyou attempted maximum times\n\n");
	exit(6);
    }
    printf("\nenter the password :");

    system("stty -echo");

    scanf("%s",password);

    system("stty sane");

    if(strcmp(customer->password,password) != 0){
	printf("\nplease enter correct password\n");
	goto back;
    }



    if(customer->balance < amount){
	printf("\ninsufficient balance\n");
	lockf(fd,F_ULOCK,-sizeof(struct account));
	close(fd);
	exit(7);
    }

    customer->balance = (customer->balance) - amount;
    lseek(fd,-sizeof(struct account),SEEK_CUR);

    if((write(fd,(struct account *)customer,sizeof(struct account)))==-1){
	printf("\ncannot update writing fails\n");
	lockf(fd,F_ULOCK,-sizeof(struct account));
	close(fd);
	exit(8);
    }

    lockf(fd,F_ULOCK,-sizeof(struct account));
    close(fd);

    printf("\nhello, %s\n",customer->name);
    printf("you withdrawed %ld Rs\n",amount);
    printf("your balance is %ld Rs\n ",customer->balance);

}
