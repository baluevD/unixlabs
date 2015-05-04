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
public:
	static std::string _input_path;
	static int _time;
public:
	ConfigReader();
	static void read(const std::string& path);
};

#endif /* CONFIGREADER_H_ */
