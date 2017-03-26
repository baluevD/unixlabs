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

void connectSocket(int socketDescriptor, const char* address, int port){
	struct sockaddr_in serv_addr;
	struct hostent *server;
	server = gethostbyname(address);
	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	bcopy(server->h_addr, (char *) &serv_addr.sin_addr.s_addr, server->h_length);
	serv_addr.sin_port = htons(port);
	int err = connect(socketDescriptor, (struct sockaddr*) &serv_addr, sizeof(serv_addr));
	if (err < 0 && errno != EINPROGRESS) {
		my_print(LOG_INFO, MakeString() << "connectSocket" << strerror(errno));
		throw "connectSocket";
	}
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
void my_print(const std::string &data){
	my_print(LOG_INFO, data);
}

void my_print(int type, const std::string &data){
#ifdef DAEMON
	syslog(type, data.c_str());
#else
	switch (type) {
		case LOG_ERR:
			std::cerr << data << std::endl;
			break;
		case LOG_INFO:
		default:
			std::cout << data << std::endl;
			break;
	}
#endif
}
