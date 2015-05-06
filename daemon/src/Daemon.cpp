/*
 * Daemon.cpp
 *
 *  Created on: May 3, 2015
 *      Author: dmitry
 */

#include "Daemon.h"
#include "ConfigReader.h"
#define EVENT struct inotify_event *
std::vector<std::string> Daemon::dir;
std::map<int, std::string> Daemon::_in_dir;

int ftw_handler(const char *name, const struct stat *status, int type) {
	if (type == FTW_NS)
		return 0;

	if (type == FTW_D)
		Daemon::dir.push_back(name);

	return 0;
}

Daemon::Daemon() {
	_inotify_discriptor = 0;
	_is_running = true;
	_is_need_reup = true;
}

void Daemon::start() {
	my_print(LOG_INFO, ToString() << "Daemon is start...");
	process();
}

void Daemon::process() {
	while (_is_running) {
		if (_is_need_reup) {
			clearData();
			if (init() < 0)
				return;
			_is_need_reup = false;
		}
		if (haveEvent()) {
			char buffer[256];
			read(this->_inotify_discriptor, buffer, 256);
			EVENT event = (EVENT)&buffer[(size_t) 0];
			dispalyEvent(event);
		}
	}
	close(this->_inotify_discriptor);
	my_print(LOG_INFO, ToString() << "Daemon is close...");
}

int Daemon::init() {
	try {
		ConfigReader::read(CONFIG_PATH);
	} catch (const char* msg) {
		this->_is_running = false;
		my_print(LOG_ERR, ToString() << msg);
		return -1;
	}

	ftw(ConfigReader::_input_path.c_str(), ftw_handler, 1);

	if(Daemon::dir.empty()){
		my_print(LOG_ERR, ToString() << "Directory list is empty");
		return -1;
	}

	_inotify_discriptor = inotify_init();
	if (_inotify_discriptor < 0) {
		my_print(LOG_ERR,
				ToString() << "initify init error" << strerror(errno));
		this->_is_running = false;
		return -1;
	}
	for (std::size_t i = 0; i < Daemon::dir.size(); i++) {
		std::string temp = Daemon::dir.at(i);
		setWatcher(temp);
	}
	cout << Daemon::_in_dir.size() << endl;
	return 0;
}

void Daemon::setWatcher(const std::string &cur_dir) {
	int watch_dir_index = inotify_add_watch(_inotify_discriptor,
			cur_dir.c_str(), IN_ALL_EVENTS);
	if (watch_dir_index < 0) {
		my_print(LOG_ERR,
				ToString() << "add watcher to " << cur_dir << " error: "
						<< strerror(errno));
	} else {
		Daemon::_in_dir[watch_dir_index] = cur_dir;
		my_print(LOG_INFO, ToString() << "add watcher to " << cur_dir);
	}
}

void Daemon::dispalyEvent(struct inotify_event *event) {
	string name;
	string type;

	if (event->len) {
		name = event->name;
	}
	if (event->mask & IN_ISDIR) {
		type = "Dir";
	} else {
		type = "File";
	}

	std::stringstream stream;
	std::stringstream abs_pwd;
	abs_pwd << Daemon::_in_dir[event->wd] << "/" << name;

	stream << " name:" << abs_pwd.str() << "; type:" << type << "; event:";

	switch (event->mask) {
	case IN_ACCESS:
		stream << "IN_ACCESS" << endl;
		return;
		break;
	case IN_MODIFY:
		stream << "IN_MODIFY" << endl;
		break;
	case IN_ATTRIB:
		stream << "IN_ATTRIB" << endl;
		break;
	case IN_CLOSE_WRITE:
		stream << "IN_CLOSE_WRITE" << endl;
		return;
		break;
	case IN_CLOSE_NOWRITE:
		stream << "IN_CLOSE_NOWRITE" << endl;
		return;
		break;
	case IN_CLOSE:
		stream << "IN_CLOSE" << endl;
		return;
		break;
	case IN_OPEN:
		stream << "IN_OPEN" << endl;
		return;
		break;
	case IN_MOVED_FROM:
		stream << "IN_MOVED_FROM" << endl;
		return;
		break;
	case IN_MOVED_TO:
		stream << "IN_MOVED_TO" << endl;
		return;
		break;
	case IN_MOVE:
		stream << "IN_MOVE" << endl;
		return;
		break;
	case IN_CREATE:
		stream << "IN_CREATE" << endl;
		break;
	case IN_DELETE:
		stream << "IN_DELETE" << endl;
		break;
	case IN_DELETE_SELF:
		stream << "IN_DELETE_SELF" << endl;
		break;
	case IN_MOVE_SELF:
		stream << "IN_MOVE_SELF" << endl;
		return;
		break;
	case IN_CREATE | IN_ISDIR:
		stream << "IN_CREATE | IN_ISDIR" << endl;
		setWatcher(abs_pwd.str());
		break;
	case IN_DELETE | IN_ISDIR:
		stream << "IN_DELETE | IN_ISDIR" << endl;
		break;
	default:
		stream << "ENOTHER_EVENT" << endl;
		return;
		break;
	}
	my_print(LOG_INFO, stream.str());
}

void Daemon::clearData() {
	Daemon::_in_dir.clear();
	Daemon::dir.clear();
}
