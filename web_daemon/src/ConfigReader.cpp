/*
 * ConfigReader.cpp
 *
 *  Created on: May 3, 2015
 *      Author: dmitry
 */

#include "ConfigReader.h"

std::string ConfigReader::_server_path;

ConfigReader::ConfigReader() {
	// TODO Auto-generated constructor stub

}

void ConfigReader::read(const std::string& path) {
	my_print(LOG_INFO, ToString() << "Read config: " << path);
	fstream file(path);
	if(file.is_open()){
		file >> ConfigReader::_server_path;
	} else {
		std::string msg = ToString() << "Can't open " << path;
		throw msg.c_str();
	}
}

std::string& ConfigReader::getPath(){
	return ConfigReader::_server_path;
}
