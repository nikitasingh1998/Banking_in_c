#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include"bank.h"


int change_details(){

    int fd,n,amount,accno;
    char choice;
    char phno[11];
    struct account *customer;
    struct stat buf;
    int attempt=0;
    char password[21];

    fd = open("bankdata",O_RDWR,0600);

    if(fd==-1){
	printf("\nrecord cannot open\n");
	exit(1);
    }
    printf("Enter the account Number\n");
    scanf("%d",&n);

 /*   if(argc != 2){
	printf("\nusage : change <accno>\n");
	close(fd);
	exit(2);
    }

    n=atoi(argv[1]);*/

    fstat(fd,&buf);
    accno = buf.st_size/(sizeof(struct account));

    if(n>accno){
	printf("\nacc no does not exists\n");
	exit(3);
    }


    customer=(struct account *)malloc(sizeof(struct account));

    lseek(fd,(n-1)*sizeof(struct account),SEEK_SET);

    lockf(fd,F_LOCK,sizeof(struct account));
   
    if((read(fd,(struct account *)customer,sizeof(struct account)))==-1){
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

    if(strcmp(customer->password,password) != 0){
	printf("\nplease enter correct password\n");
	goto back;
    }

    printf("\nyour current phno is :%s\n",customer->phno);
    printf("want to change it? y or n\n\n\n");
    fflush(stdin);
    fflush (stdout);

  //  choice=getchar();
    scanf("%c",&choice);
    if(choice == 'y'){

	printf("\nenter your ph no \n");
	if((scanf("%s",phno))!=1){
	    printf("\ndidnot entered correct\n");
	    exit(6);
	}
    }
    else
	exit(7);

    if(strlen(phno)!= 10){
	printf("\nenter 10 digit ph no\n");
	exit(8);
    }
    strcpy(customer->phno,phno);

    lseek(fd,-sizeof(struct account),SEEK_CUR);

    if((write(fd,(struct account *)customer,sizeof(struct account)))==-1){
	printf("\ncannot update writing fails\n");
	close(fd);
	exit(9);
    }

    lockf(fd,F_ULOCK,-sizeof(struct account));
    
    close(fd);

    printf("\nyour new phno is %s\n\n",phno);

}

    
