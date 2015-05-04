/*
 * header.h
 *
 *  Created on: Apr 26, 2015
 *      Author: dmitry
 */

#ifndef HEADER_H_
#define HEADER_H_

#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#include <syslog.h>
#include <stdlib.h>
#include <dirent.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <sstream>
#include <vector>
#include <fstream>
#include <iostream>
#include <string>
using namespace std;

#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <err.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>

using namespace std;

#define CONFIG_PATH "/tmp/config.txt"

class ToString {
protected:
	std::stringstream m_stream;
public:
	template<class T>
	ToString& operator<<(const T& arg) {
		m_stream << arg;
		return *this;
	}
	operator std::string() const {
		return m_stream.str();
	}
};

void my_print(int type, const std::string &data);

#endif /* HEADER_H_ */
