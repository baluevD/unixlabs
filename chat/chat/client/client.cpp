#include "../header.h"
#include "client.h"

int inputListener() {
    char c;
    if (read(0, &c, sizeof(c)) > 0) {
        if (c == '\n') {
            actualMSG += '\0';
            return 1;
        }

        if (c != 8) {
            actualMSG += c;
        } else {
        	recievedMSG.pop_back();
        }
    }
    return 0;
}

void sender(int fd, const string& message) {
    send(fd, message.c_str(), message.length(), 0);
}

void getMsgFromSocket(int fd) {
    char buf[BIF_SIZE];
    recv(fd, buf, BIF_SIZE, 0);
    recievedMSG.push_back(string(buf));
}

void msgViewer() {
    for (string s : recievedMSG)
        cout << s << endl;
    recievedMSG.clear();
}

void processRecievedMessage(int listener){
	timeval timeout = setTimer(120);
    fd_set RSet;
    FD_ZERO(&RSet);
    FD_SET(listener, &RSet);
    FD_SET(0, &RSet);
    while (true)
        if (select(max(0, listener) + 1, &RSet, 0, 0, &timeout)) {
            if (FD_ISSET(0, &RSet))
                if (inputListener()) {
                    sender(listener, actualMSG);
                    actualMSG.clear();
                }
            if (FD_ISSET(listener, &RSet))
                getMsgFromSocket(listener);
            FD_SET(listener, &RSet);
            FD_SET(0, &RSet);
            msgViewer();
        }
}

void initClient() {
    cout << "Starting client..." << endl;
    sleep(1);
    cout << "Client has been started." << endl;

    struct sockaddr_in serverAddress;
    struct hostent *server;

    server = gethostbyname("localhost");

    bzero((char *) &serverAddress, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    bcopy(server->h_addr, (char *)&serverAddress.sin_addr.s_addr, server->h_length);
    serverAddress.sin_port = htons(PORT);


    int listener = socket(AF_INET, SOCK_STREAM, 0);
    fcntl(0, F_SETFL, O_NONBLOCK);

    int err = connect(listener, (struct sockaddr*)&serverAddress, sizeof(serverAddress));
    if (err < 0 && errno != EINPROGRESS) {
        cerr << "Connecting error has occurred: " << strerror(errno);
        exit(EXIT_SUCCESS);
    }

    processRecievedMessage(listener);
}
