# include <stdio.h>
# include <stdlib.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <string.h>
# include "account.h"

int main ( int argc, char ** argv ) {
	
	struct record rec;
	int dbfd;
	int size;
	int ac_no;
	int balance = 0;
	int amount;
	int count = 0;
	struct flock fl = { F_WRLCK, SEEK_SET, 0, 0, 0};
	fl.l_pid = getpid();
	if ( argc != 3 ) {
		fprintf ( stderr, "usage: change acc_no phone_no\n" );
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
	p = argv [ 2 ];
	while ( *p != '\0') {
		if ( *p < '0' && *p > '9') {
			fprintf (stderr, "Amount is digit\n");
			exit ( 6 );
		}
//		printf("%c",*p);
		p++;
		count ++;
	}
	if (count != 10) {
		printf("Enter correct phone number\n");
		exit (7);
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
	fl.l_len = sizeof ( rec );
	if ( fcntl ( dbfd, F_SETLKW, &fl ) == -1) {
		fprintf(stderr, "unable to lock\n");
		exit (12);
	}
	if ( (size = read(dbfd,&rec,sizeof(rec))) != sizeof(rec) ) {
		fprintf ( stderr, "Read error\n" );
		exit ( 9 );
	}
	strcpy ( rec.phone_no, argv [ 2 ]);
	if ( ( size = lseek ( dbfd, 0, SEEK_SET )) == -1 ) {
		fprintf (stderr, "Seek error\n");
		exit (8);
	}
	if (ac_no != 1) {
		if ( ( size = lseek ( dbfd, ( ac_no - 1 ) * sizeof(rec) , SEEK_SET )) == -1 ) {
			fprintf (stderr, "Seek error\n");
			exit (8);
		}
	}
	if ( ( write(dbfd,&rec,sizeof(rec))) != sizeof(rec)) {
		fprintf(stderr, "Write error\n");
		exit(10);
	}
	fl.l_type = F_UNLCK;
	fl.l_start = size;
	if ( fcntl ( dbfd, F_SETLK, &fl ) == -1) {
		fprintf(stderr, "unable to lock\n");
		exit (12);
	}

	printf("\n\nAccount_no\tPhone_no\n");
	printf("%d\t\t%s",ac_no,rec.phone_no);
	printf("\nUpdated.....\n\n\n");
}
