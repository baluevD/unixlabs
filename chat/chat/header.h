#ifndef HEADER_H
#define HEADER_H

#include <iostream>
#include <cstdlib>
#include <list>
#include <set>
#include <stack>
#include <algorithm>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/syslog.h>
#include <sys/types.h>

using namespace std;

const uint16_t BIF_SIZE = 1024;
const int TIMEOUT = 1000;
const int PORT = 12333;

int initServer();
void initClient();
void asDemon();
void demonControlHandeler(int msgID);

#define DEMON

#ifdef APP
#define out(p, x) printf((p), (x))
#endif

#ifdef DEMON
#define out(p, x) syslog(LOG_INFO, (p), (x))
#endif

#define SERVER_VERBOSE
#ifdef SERVER_VERBOSE
#define my_printf(p, x) out(p, x)
#else
#define my_printf(p, x)
#endif


timeval setTimer(int seconds);


#endif //HEADER_H
