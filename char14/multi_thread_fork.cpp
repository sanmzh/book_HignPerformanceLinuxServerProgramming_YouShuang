#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <wait.h>

pthread_mutex_t mutex;

void*
another ( void* arg ) {
	printf ( "In child thread, lock the mutex\n");
	pthread_mutex_lock ( &mutex );
	sleep ( 5 );
	pthread_mutex_unlock ( &mutex );
}

int
main ( ) {
	pthread_mutex_init ( &mutex, NULL );
	pthread_t id;
	pthread_create ( &id, NULL, another, NULL );
	
	sleep ( 1 );
	int pid = fork ( );
	if ( pid < 0 ) {
		pthread_join ( id, NULL );
		pthread_mutex_destroy ( &mutex );
		return 1;
	}
	else if ( pid == 0 ) {
		printf ( "I am in the child, want to get the lock\n");
		pthread_mutex_lock ( &mutex );
		printf ( "I can not run to here, opp...\n" );
		pthread_mutex_unlock( &mutex );
		exit ( 0 );
	}
	else {
		wait ( NULL );
	}
	pthread_join ( id, NULL );
	pthread_mutex_destroy ( &mutex );
	
	return 0;
}
