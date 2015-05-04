/*
 * Daemon.cpp
 *
 *  Created on: May 3, 2015
 *      Author: dmitry
 */

#include "Daemon.h"
#include <string.h>

Daemon::Daemon() {
	_socket = 0;
	_sin_len = sizeof(_client_address);
	buf = new char[1024];
	_client_fd = 0;
}

void Daemon::start() {
	my_print(LOG_INFO, ToString() << "Daemon is start...");
	try {
		ConfigReader::read(CONFIG_PATH);
	} catch (char* msg) {
		my_print(LOG_ERR, ToString() << msg);
	}
	if (!checkDirExist(ConfigReader::getPath())) {
		my_print(LOG_ERR,
				ToString() << "Can't open " << ConfigReader::getPath());
		exit(-1);
	}

	_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (_socket == -1) {
		my_print(LOG_ERR,
				ToString() << "Can't init socket: " << strerror(errno));
		exit(-1);
	}

	int optval = 1;
	if (setsockopt(_socket, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(int))
			== -1) {
		my_print(LOG_ERR,
				ToString() << "Can't init socket: " << strerror(errno));
		exit(-1);
	}

	struct sockaddr_in svr_addr;
	svr_addr.sin_family = AF_INET;
	svr_addr.sin_addr.s_addr = INADDR_ANY;
	svr_addr.sin_port = htons(5050);

	if (bind(_socket, (struct sockaddr *) &svr_addr, sizeof(svr_addr)) == -1) {
		close(_socket);
		my_print(LOG_ERR,
				ToString() << "Can't bind socket: " << strerror(errno));
		exit(-1);
	}

	process();

}

void Daemon::process() {
	listen(_socket, 1);
	while (true) {
		if (haveConnect()) {
			switch (fork()) {
			case 0: {
			std::string msg = _recv();

			string method, page;
			std::stringstream q(msg);
			q >> method;
			q >> page;

			page = ConfigReader::getPath() + page;
			std::stringstream stream;
			cout << method << endl;
			cout << page << endl;

			std::string z = ConfigReader::getPath() + "/";
			if(page == z)
				page = ConfigReader::getPath() + "/index.html";

			if (method.compare("GET ") && (access(page.c_str(), F_OK) != -1)) {
				stream << "HTTP/1.1" << 404 << "error" << endl;
				stream << "Content-Type: text/html; charset=UTF-8" << endl
						<< endl;
				std::string line;
				std::ifstream myfile(page.c_str());
				while (std::getline(myfile, line)) {
					stream << line;
				}
			} else {
				stream << "HTTP/1.1" << 404 << "error" << endl;
				stream << "Content-Type: text/html; charset=UTF-8" << endl
						<< endl;
				stream
						<< "<!DOCTYPE html><html><body><h1>page not found</h1></body></html>";
			}
			write(_client_fd, stream.str().c_str(), stream.str().size());

			close(_client_fd);
			break;
			}
			case -1:
				my_print(LOG_ERR,
						ToString() << "Can't fork: " << strerror(errno));
				break;
			default:
				close(_client_fd);
			}
		}
	}
}

const std::string Daemon::_recv() {
	int bytes_read = recv(_client_fd, &*buf, 1024, 0);
	if (bytes_read == -1) {
		my_print(LOG_ERR, ToString() << "Can't recv: " << strerror(errno));
		return NULL;
	}
	return std::string(buf, bytes_read);
}

int Daemon::haveConnect() {
	_client_fd = accept(_socket, (struct sockaddr *) &_client_address,
			&_sin_len);
	if (_client_fd == -1) {
		my_print(LOG_ERR, ToString() << "Can't accept: " << strerror(errno));
		return false;
	}
	return true;
}

void Daemon::die() {
	my_print(LOG_INFO, ToString() << "Daemon is close...");
}

int Daemon::checkDirExist(const std::string& dir) {
	struct stat s;
	if (!stat(dir.c_str(), &s) && S_ISDIR(s.st_mode))
		return true;
	return false;
}
