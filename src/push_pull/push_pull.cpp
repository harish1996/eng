#include "push_pull.h"

static int push_to_origin( const std::string &origin )
{
	std::string command = "rsync -au --exclude 'origin' .eng/ \"" + origin + "\"";
	return system(command.c_str());
}

static int pull_from_origin( const std::string &origin )
{
	std::string command = "rsync -au --exclude 'origin' \"" + origin + "/\" .eng";
	return system(command.c_str());
}

static int update_origin( const std::string &origin )
{
	std::ofstream file;
	file.open(".eng/origin");
	file << origin;
	file.close();
	return SUCCESS;
}

static int default_push()
{
	std::fstream file;
	file.open(".eng/origin");
	if(!file.is_open())
		return -ORIGIN_NOT_SET;
	std::string origin;
	getline(file, origin);
	file.close();
	return push_to_origin(origin);
}

static int default_pull()
{
	std::fstream file;
	file.open(".eng/origin");
	if(!file.is_open())
		return -ORIGIN_NOT_SET;
	std::string origin;
	getline(file, origin);
	file.close();
	return pull_from_origin(origin);
}

// #include <iostream>

// int main()
// {
// 	std::string origin = "/home/ankankumardas/Desktop/OS Project/origin";
// 	std::cout << update_origin(origin) << "\n";
// 	std::cout << default_push() << "\n";
// 	std::cout << default_pull() << "\n";
// 	return 0;
// }