/*
 * MyServer.h
 *
 *  Created on: Mar 26, 2017
 *      Author: dmitry
 */

#ifndef MYSERVER_H_
#define MYSERVER_H_

#include "helper.h"

const uint16_t SIZE = 1024;

class MyServer {

private:
	u_short _port;
	int _socketDescriptor;
	set<int> clients;

	// --- //
	timeval timeout;

	void readSet(fd_set *set);
	void connectionsHandler(fd_set *set);
	void messageHandler(fd_set *set);
	void _select(fd_set *set);

public:
	MyServer(u_short port);
	void init();
	void run();

};

#endif /* MYSERVER_H_ */
