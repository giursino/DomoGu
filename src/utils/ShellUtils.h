#ifndef SHELLUTILS_H
#define SHELLUTILS_H

#include <string>

int ExecuteScriptAsync(const std::string& script);
int ExecuteScriptAsync(const char* script);

int ExecuteScript(const std::string& script);
int ExecuteScript(const std::string& script, std::string& result);

int ExecuteScript(const char* script);
int ExecuteScript(const char* script, std::string& result);

#endif
