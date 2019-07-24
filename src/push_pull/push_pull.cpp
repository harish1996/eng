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

int update_origin( const std::string &origin )
{
	std::ofstream file;
	file.open(".eng/origin");
	file << origin;
	file.close();
	return SUCCESS;
}

int DEFAULT_PUSH()
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

int DEFAULT_PULL()
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