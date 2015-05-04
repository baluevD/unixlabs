/*
 * Daemon.h
 *
 *  Created on: May 3, 2015
 *      Author: dmitry
 */

#ifndef DAEMON_H_
#define DAEMON_H_
#include "header.h"
#include "ConfigReader.h"

class Daemon {
private:
	int _socket;
	int _client_fd;
	struct sockaddr_in _client_address;
	socklen_t _sin_len;
	char *buf;

private:
	int checkDirExist(const std::string& dir);
	const std::string _recv();
	void process();
	int haveConnect();

public:
	Daemon();
	void start();
	void die();
};

#endif /* DAEMON_H_ */
