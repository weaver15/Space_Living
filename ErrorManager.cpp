#include "ErrorManager.h"
#include <ctime>
#include <chrono>
#include "pugixml\pugixml.hpp"


ErrorManager::ErrorManager()
{
}


ErrorManager::~ErrorManager()
{
}

void ErrorManager::LogError(std::string tag, std::string location, std::string message)
{
	time_t result = time(NULL);
	char str[26];
	ctime_s(str, sizeof str, &result);

	pugi::xml_document doc;
	doc.load_file("ErrorLog.xml");
	pugi::xml_node nodeChild = doc.append_child("Error");
	nodeChild.append_attribute("Time: ") = str;
	nodeChild.append_attribute("Tag: ") = tag.c_str(); 
	nodeChild.append_attribute("Location: ") = location.c_str();
	nodeChild.append_attribute("Message: ") = message.c_str();
	doc.save_file("ErrorLog.xml");
}
