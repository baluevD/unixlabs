/*
 * ConfigReader.h
 *
 *  Created on: May 3, 2015
 *      Author: dmitry
 */

#ifndef CONFIGREADER_H_
#define CONFIGREADER_H_
#include "header.h"

class ConfigReader {
	static std::string _server_path;
public:
	ConfigReader();
	static void read(const std::string& path);
	static std::string& getPath();
};

#endif /* CONFIGREADER_H_ */
