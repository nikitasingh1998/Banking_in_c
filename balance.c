# include <stdio.h>
# include <stdlib.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <fcntl.h>
# include "bank.h"

int main ( int argc, char ** argv ) {
	
	struct record rec;
	int dbfd;
	int size;
	int ac_no;
	int balance = 0;
	int amount;
	struct flock fl = { F_WRLCK,SEEK_SET, 0, 0, 0 };
	fl.l_pid = getpid();
	if ( argc != 2 ) {
		fprintf ( stderr, "usage: balance acc_no\n" );
		exit ( 1 );
	}
	char *p = argv [ 1 ];
	while ( *p != '\0') {
		if ( *p < '0' && *p > '9') {
			fprintf (stderr, "Account num is digit\n");
			exit ( 2 );
		}
//		printf("%c",*p);
		p++;
	}
	if ( (dbfd = open ( "sb_acc", O_RDWR, 0664 )) == -1 ) {
		fprintf (stderr, "Database open error\n");
		exit ( 3 );
	}
	if ( ( size = lseek ( dbfd, 0, SEEK_END )) == -1 ) {
		fprintf (stderr, "Seek error\n");
		exit (4);
	}
	ac_no = atoi(argv[1]);
	if ( ac_no < 1 || ac_no > size/sizeof(rec)) {
		fprintf (stderr, "wrong account number\n");
		exit ( 5 );
	}
	if ( ( size = lseek ( dbfd, 0, SEEK_SET )) == -1 ) {
		fprintf (stderr, "Seek error\n");
		exit (8);
	}
	if (ac_no != 1) {
		if ( ( size = lseek ( dbfd, ( ac_no - 1 ) * sizeof(rec) , SEEK_SET )) == -1 ) {
			fprintf (stderr, "Seek error\n");
			exit (8);
		}
		fl.l_start = size;
	}
	fl.l_len = sizeof(rec);
	if ( fcntl(dbfd, F_SETLKW, &fl) == -1) {
		fprintf(stderr, "Unable to lock\n");
		exit(12);
	}
	if ( (size = read(dbfd,&rec,sizeof(rec))) != sizeof(rec) ) {
		fprintf ( stderr, "Read error\n" );
		exit ( 9 );
	}
	fl.l_type =F_UNLCK;
	if ( fcntl(dbfd, F_SETLK, &fl) == -1) {
		fprintf(stderr, "Unable to relese lock\n");
		exit(12);
	}

	printf("\n\nAccount_no\tBalance\n");
	printf("%d\t\t%d\n\n",ac_no,rec.balance);
}
