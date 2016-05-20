#include <fstream>
#include <sstream>
#include <string>
#include "Config.hpp"

#include <iostream>

Config::Config(void)
{
	std::string		cfgFile("master.cfg");

	this->_loadFile(cfgFile);
}

Config::Config(const std::string &cfgFile)
{
	this->_loadFile(cfgFile);
}

Config::~Config(void)
{}

void                Config::_loadFile(const std::string &cfgFile)
{
	std::ifstream	configFile(cfgFile.c_str());
	std::string		line, name, value;
	unsigned int	pos = 0;

	while (std::getline(configFile, line))
	{
		pos = line.find("=");
		name = this->clearString(line.substr(0, pos));
		value = this->clearString(line.substr(pos + 1));
		this->_data[name] = value;
	}
}

std::string         Config::clearString(std::string str) const
{
	unsigned int	i = 0, size = str.size();
	std::string		newStr;

	for (i = 0; i < size; i++)
	{
		if (str[i] != ' ' && str[i] != '\t')
			newStr.push_back(str[i]);
	}
	return newStr;
}

std::string         Config::getValue(const std::string &name)
{
	return this->_data[name];
}

unsigned short		Config::getUShort(const std::string &name, unsigned short defValue)
{
	unsigned short									value = defValue;
	std::map<std::string, std::string>::iterator	it = this->_data.find(name);

	if (it != this->_data.end())
	{
		std::istringstream		buffer(this->_data[name]);
		buffer >> value;
	}
	return value;
}

void                Config::setUShort(const std::string &name, unsigned short value)
{
	std::stringstream		ss;

	ss << value;
	this->_data[name] = std::string(ss.str());
}

void                Config::save(void) const
{
	std::map<std::string, std::string>::const_iterator		it;
	std::map<std::string, std::string>::const_iterator		end = this->_data.end();

	for (it = this->_data.begin(); it != end; it++)
	{
		std::cout << it->first << " => " << it->second << std::endl;
	}
}
