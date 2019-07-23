#ifndef _PUSH_PULL_H
#define _PUSH_PULL_H

#include <stdlib.h>
#include <string>

static int push_to_origin( const std::string &origin )
{
	std::string command = "rsync -au --delete .eng/ \"" + origin + "\"";
	system(command.c_str());
}

static int pull_from_origin( const std::string &origin )
{
	std::string command = "rsync -au --delete \"" + origin + "/\" .eng";
	system(command.c_str());
}

#endif