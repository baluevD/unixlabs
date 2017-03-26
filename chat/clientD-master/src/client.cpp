#include "helper.h"

const char* address = "localhost";

vector<string> recievedMessages;

void recieveMessages(int fd) {
	char buf[1024];
	ssize_t len = recv(fd, buf, 1024, 0);
	string incoming_msg = string(buf, len);
	recievedMessages.push_back(incoming_msg);
	cout << endl;
}

void viewMessages() {
	for (auto s : recievedMessages) {
		cout << s;
	}
	recievedMessages.clear();
	cout << endl;
}

int main(int argc, char *argv[]) {
	timeval timeout = setTimeout(1200);
	int listener = createNewSocket();
	connectSocket(listener, address, atoi(argv[1]));
	string message;

	fd_set read_set;
	FD_ZERO(&read_set);
	FD_SET(listener, &read_set);
	FD_SET(0, &read_set);
	while (true) {
		if (select(listener + 1, &read_set, 0, 0, &timeout)) {
			if (FD_ISSET(0, &read_set)) {
				cin >> message;
				send(listener, message.c_str(), message.length(), 0);
			}
			if (FD_ISSET(listener, &read_set)) {
				recieveMessages(listener);
			}

			FD_SET(listener, &read_set);
			FD_SET(0, &read_set);
			viewMessages();
		}
	}
}
