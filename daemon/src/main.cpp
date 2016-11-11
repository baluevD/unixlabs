//============================================================================
// Name        : daemon.cpp
// Author      : Dmitry
// Version     :
// Copyright   : GPL
// Description : Hello World in C++, Ansi-style
//============================================================================
#include <ostream>
#include "Daemon.h"
//#define DAEMON

Daemon dmn;

void signal_handler(int sig) {
	switch (sig) {
	case SIGHUP:
		dmn.reup();
		break;
	case SIGTERM:
		dmn.die();
		break;
	}
}


int main(int argc, char **argv) {
#ifdef DAEMON
	int pid = fork();

	switch (pid) {
		case 0: // child
		{

			umask(0);
			setsid();

			close (STDIN_FILENO);
			close (STDOUT_FILENO);
			close (STDERR_FILENO);

			signal(SIGHUP, signal_handler);
			signal(SIGTERM, signal_handler);
			openlog("daemon", LOG_CONS | LOG_PID | LOG_NDELAY, LOG_LOCAL1);
#endif

// CODE
			dmn = Daemon();
			dmn.start();
			exit(0);

#ifdef DAEMON
	break;
}
case -1: // error!
default:// parent
return 0;
}
#endif
}
