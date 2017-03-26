#include "helper.h"

int createNewSocket() {
	int sockDesrcr = socket(AF_INET, SOCK_STREAM, 0);
	if (sockDesrcr == -1)
		throw "createNewSocket";

	return sockDesrcr;
}

void bundSocket(int socketDescriptor, u_short port) {
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(port);
	if (bind(socketDescriptor, (const sockaddr *) &addr, sizeof(addr)) == -1)
		throw "bundSocket";
	listen(socketDescriptor, 2);
}

timeval setTimeout(int seconds) {
	timeval timeout;
	timeout.tv_sec = seconds;
	timeout.tv_usec = 0;
	return timeout;
}

std::vector<std::string> &split(const std::string &s, char delim,
		std::vector<std::string> &elems) {
	std::stringstream ss(s);
	std::string item;
	while (std::getline(ss, item, delim)) {
		elems.push_back(item);
	}
	return elems;
}

std::vector<std::string> split(const std::string &s, char delim) {
	std::vector<std::string> elems;
	split(s, delim, elems);
	return elems;
}

const std::string dataFromFile(const std::string &path) {
	std::string line;
	std::stringstream stream;
	std::ifstream myfile(path.c_str());
	while (std::getline(myfile, line)) {
		stream << line;
	}

	myfile.close();

	return stream.str();
}

void srv_print(const std::string &str, int type) {

#ifdef DAEMON_MODE
	syslog(type, str.c_str());
#else
	switch (type) {
		case LOG_ERR:
		std::cerr << str;
		break;
		case LOG_INFO:
		default:
		std::cout << str;
		break;
	}
#endif

}
