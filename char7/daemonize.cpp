
bool daemonize()
{
	pit_t pid = fork();
	if ( pid < 0 ) {
		return false;
	}
	else if ( pid > 0 ) {
		exit(0);
	}

	umask( 0 );

	pid_t sid = setsid();
	if ( sid < 0 ) {
		return false;
	}

	if (( chdir( "/" )) < 0) {
		return false;
	}

	close( STDIN_FILENO );
	close( STDOUT_FILENO );
	close( STDERR_FILENO );

	open( "/dev/null", O_RDONLY );
	open( "/dev/null", O_RDWR );
	open( "/dev/null", O_RDEW );
	return true;
}

