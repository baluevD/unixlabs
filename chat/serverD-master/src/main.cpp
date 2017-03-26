/*
 * main.cpp
 *
 *  Created on: Mar 26, 2017
 *      Author: dmitry
 */

// #define DAEMON_MODE

#include "MyServer.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#include <syslog.h>
#include "helper.h"

void signal_handler(int sig) {
	switch (sig) {
	case SIGHUP:
		syslog(LOG_INFO, "Reload conf file\n");
		break;
	case SIGTERM:
		syslog(LOG_INFO, "Close daemon...\n");
		exit(0);
		break;
	}
}

int main(int argc, char **argv) {
#ifdef DAEMON_MODE
	int pid = fork();

	switch (pid) {
	case 0: // child
	{

		umask(0);
		setsid();

		close(STDIN_FILENO);
		close(STDOUT_FILENO);
		close(STDERR_FILENO);

		signal(SIGHUP, signal_handler);
		signal(SIGTERM, signal_handler);
		openlog("chat_server", LOG_CONS | LOG_PID | LOG_NDELAY, LOG_LOCAL1);
		srv_print(MakeString() << "start!", LOG_INFO);
#endif
		MyServer server(atoi(argv[1]));
		server.init();
		server.run();
#ifdef DAEMON_MODE
		break;
	}
	case -1: // error!
	default: // parent
		return 0;
	}
#endif
}
