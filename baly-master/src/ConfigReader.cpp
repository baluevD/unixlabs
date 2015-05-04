/*
 * ConfigReader.cpp
 *
 *  Created on: May 3, 2015
 *      Author: dmitry
 */

#include "ConfigReader.h"

std::string ConfigReader::_input_path;
int ConfigReader::_time;

ConfigReader::ConfigReader() {
	// TODO Auto-generated constructor stub

}

void ConfigReader::read(const std::string& path) {
	std::string line;
	std::ifstream file(path);
	if (file.is_open()) {
		file >> _input_path >> _time;
	} else {
		std::string msg = ToString() << "Can't config file " << path;
		throw msg.c_str();
		return;
	}
}
