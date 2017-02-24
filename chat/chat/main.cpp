#include "header.h"

int main() {
    pid_t pid;
    pid = fork();
    if (0 == pid) initServer();
    else initClient();
    return 0;
}
