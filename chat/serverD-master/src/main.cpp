/*
 * main.cpp
 *
 *  Created on: Mar 26, 2017
 *      Author: dmitry
 */

// #define DAEMON_MODE

#include "MyServer.h"
#include "helper.h"

string getOpt = "p:";

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

struct globalVars {
	u_short port;
} globalVars;

void argumentsHandler(int argc, char **argv) {
	if (argc < 2)
		throw 2;

	int rez = 0;
	while ((rez = getopt(argc, argv, getOpt.c_str())) != -1) {
		switch (rez) {
		case 'p':
			globalVars.port = atoi(optarg);
			break;
		default:
			throw 2;
		}
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
	try {
		argumentsHandler(argc, argv);
		MyServer server(globalVars.port);
		server.init();
		server.run();
	} catch (int code) {
		switch (code) {
		case 2:
			srv_print(
					MakeString()
							<< "Arguments error, -p <server_port_number> requred.",
					LOG_INFO);
			break;
		}
	}
#ifdef DAEMON_MODE
	break;
}
case -1: // error!
default: // parent
return 0;
}
#endif
}
