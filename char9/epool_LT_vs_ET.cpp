#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/epoll.h>
#include <pthread.h>

#define MAX_EVENT_NUMBER 1024
#define BUFFER_SIZe 10

int
setnonblocking( int fd ) {
	int old_option = fcntl( fd, F_GETFL );
	int new_option = old_option | O_NONBLOCK;
	fcntl( fd, F_SETFL, new_option );
	return old_option;
}

void
addfd( int epollfd, int fd, bool enable_et ) {
	epoll_event event;
	event.data.fd = fd;
	event.envents = EPOLLIN;
	if ( enable_et ) {
		event.events |= EPOLLET;
	}
	epoll_ctl( epollfd, EPOLL_CTL_ADD, fd, &event );
	setnonblocking( fd );
}

void
lt( epoll_event* event, int number, int epollfd, int listenfd ) {
	char buf[ BUFFER_SIZE ];
	for ( int i = 0; i < number; i++ ) {
		struct sockaddr_in client_address;
		socklen_t client_addrlength = sizeof(client_address);
		int connfd = accept( listenfd, ( struct sockaddr* )&client_address, &client_addrlength );
		addfd( epollfd, connfd, false );
	}

