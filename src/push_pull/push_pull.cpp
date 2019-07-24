#include "push_pull.h"

int push_to_origin( const std::string &origin )
{
	std::string command = "rsync -au --exclude 'origin' .eng/ \"" + origin + "\"";
	return system(command.c_str());
}

int pull_from_origin( const std::string &origin )
{
	std::string command = "rsync -au --exclude 'origin' \"" + origin + "/\" .eng";
	return system(command.c_str());
}

int DEFAULT_UPDATE_ORIGIN( const std::string &origin )
{
	std::ofstream file;
	file.open(".eng/origin");
	file << origin;
	file.close();
	return PUSH_SUCCESS;
}

int DEFAULT_PUSH()
{
	std::fstream file;
	file.open(".eng/origin");
	if(!file.is_open())
		return -EPUSH_ORIGIN_NOT_SET;
	std::string origin;
	getline(file, origin);
	file.close();
	return push_to_origin(origin);
}

int DEFAULT_PULL()
{
	std::fstream file;
	file.open(".eng/origin");
	if(!file.is_open())
		return -EPUSH_ORIGIN_NOT_SET;
	std::string origin;
	getline(file, origin);
	file.close();
	return pull_from_origin(origin);
}

