/*
 * MyServer.cpp
 *
 *  Created on: Mar 26, 2017
 *      Author: dmitry
 */

#include "MyServer.h"

MyServer::MyServer(u_short port) {
	_port = port;
	timeout = setTimeout(60);
}

void MyServer::init() {
	cout << "Init new instance of MyServer" << endl;

	try {
		_socketDescriptor = createNewSocket();
		bundSocket(_socketDescriptor, _port);
	} catch (const char *msg) {
		cerr << "Smth going wrong: " << msg << endl;
		throw -1;
	}
}

void MyServer::readSet(fd_set *read_set) {
	FD_ZERO(read_set);
	FD_SET(_socketDescriptor, read_set);
	for (auto client : clients) {
		FD_SET(client, read_set);
	}
}

void MyServer::connectionsHandler(fd_set *set) {
	if (FD_ISSET(_socketDescriptor, set)) {
		int sock = accept(_socketDescriptor, 0, 0);
		fcntl(sock, F_SETFL, O_NONBLOCK);
		clients.insert(sock);
		string msg = "Socket: " + std::to_string(sock) + "\n";
		cout << msg << endl;
		send(sock, msg.c_str(), msg.length(), 0);
	}
}

void MyServer::messageHandler(fd_set *set) {
	for (auto client : clients) {
		if (FD_ISSET(client, set)) {
			char *buf = new char[SIZE];
			ssize_t len = recv(client, buf, SIZE, 0);
			srv_print(MakeString() << "len: " << len, LOG_INFO);
			string rawMsg(buf, len);
			srv_print(MakeString() << "msg: " << rawMsg, LOG_INFO);

			std::stringstream resultStream;
			resultStream << "client(" << client << "): ";

			if(rawMsg.find("exit") != std::string::npos) {
				close(client);
				clients.erase(client);
				resultStream << "has closed connection." << endl;
			} else {
				resultStream << rawMsg;
			}

			string resultMsg = resultStream.str();

			for (auto receiver : clients) {
				srv_print(MakeString() << "To client(" << receiver << "): " << resultMsg, LOG_INFO);
				int a = send(receiver, resultMsg.c_str(), resultMsg.length(), 0);
				srv_print(MakeString() << "msg:" << a, LOG_INFO);
			}
		}
	}
}

void MyServer::_select(fd_set *set) {
	int maximum = std::max(_socketDescriptor,
			*std::max_element(clients.begin(), clients.end()));
	select(maximum + 1, set, 0, 0, &timeout);
}

void MyServer::run() {
	cout << "Server has been running on:" << endl << "port:" << _port << endl
			<< "sock:" << _socketDescriptor << endl;

	while (true) {
		fd_set set;
		readSet(&set);
		_select(&set);

		connectionsHandler(&set);
		messageHandler(&set);
	}
}
