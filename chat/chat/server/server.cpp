#include "../header.h"

void bind_socket(int sock, u_short port) {
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(port);
    int res = bind(sock, (const sockaddr *) & addr, sizeof(addr));
    res = listen(sock, 2);
}

void process_connection(int listener, std::set<int>* clients) {
    int sock = accept(listener, 0, 0);
    fcntl(sock, F_SETFL, O_NONBLOCK);
    clients->insert(sock);
    char msg[BIF_SIZE];
    memset(msg, '\0', sizeof(msg));
    sprintf(msg, "ID: %d", sock);
    send(sock, msg, strlen(msg), 0);
}

void init_read_set(int listener, std::set<int>* clients, fd_set *read_set) {
    FD_ZERO(read_set);
    FD_SET(listener, read_set);
    for (auto client : *clients) {
        FD_SET(client, read_set);
    }
}

void process_string(const int client, std::set<int>* clients) {
    char buf[BIF_SIZE];
    memset(buf, '\0', sizeof(buf));
    ssize_t len = recv(client, buf, BIF_SIZE, 0);
    char msg[BIF_SIZE];
    memset(msg, '\0', sizeof(msg));
    sprintf(msg, "[%d: %s]", client, buf);

    if (len <= 0) {
        close(client);
        clients->erase(client);
    } else if (len > 0 && !clients->empty()) {
    	my_printf("Receive message: [%s]\n", buf);
        for (auto receiver : *clients) {
        	my_printf("Throwing it to client [%d]\n", receiver);
            send(receiver, msg, strlen(msg), 0);
        }
    }
}

int initServer() {
    asDemon();

    int listener = socket(AF_INET, SOCK_STREAM, 0);
    bind_socket(listener, PORT);
    std::set<int> clients;
    timeval timeout = setTimer(TIMEOUT);

    while(true) {
        fd_set read_set;
        init_read_set(listener, &clients, &read_set);
        int maximum = std::max(listener, *std::max_element(clients.begin(), clients.end()));
        int res = select(maximum + 1, &read_set, 0, 0, &timeout);

        if (FD_ISSET(listener, &read_set)) {
            process_connection(listener, &clients);
        }

        for (auto client : clients) {
            if (FD_ISSET(client, &read_set)) {
                process_string(client, &clients);
            }
        }
    }
}
