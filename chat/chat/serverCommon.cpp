#include "header.h"

void asDemon() {
    pid_t pid;
    pid_t sid;

    my_printf("%s\n", "Starting as a demon...");
    pid = fork();

    if (pid == 0)
        sid = setsid();
    if ( pid != 0 ) {
        exit(EXIT_SUCCESS);
    }
    pid = fork();
    if ( pid != 0 ) {
        exit(EXIT_SUCCESS);
    }
    umask(0);

    #ifdef DEMON
    close (STDIN_FILENO ) ;
    close (STDOUT_FILENO) ;
    close (STDERR_FILENO) ;
    openlog ("Chat", LOG_CONS | LOG_PID | LOG_NDELAY, LOG_LOCAL1);
    my_printf("%s\n", "------Chat Log------");
    #endif

    signal(SIGTERM, demonControlHandeler);
    signal(SIGHUP, demonControlHandeler);
}

void demonControlHandeler(int msgID) {
    switch (msgID) {
        case SIGTERM:
            out("%s\n","Receive termination signal, close down.\n");
            exit(EXIT_SUCCESS);
        default:
            break;
    }
}

timeval setTimer(int seconds) {
    timeval timeout;
    timeout.tv_sec = seconds;
    timeout.tv_usec = 0;
    return timeout;
}
