#include "ShellUtils.h"

#include <array>
#include <cstdlib>
#include <memory>

#include <stdio.h>

int ExecuteScriptAsync(const std::string& script) {
  return ExecuteScriptAsync(script.c_str());
}

int ExecuteScriptAsync(const char* script) {
  return WEXITSTATUS(system(script));
}

int ExecuteScript(const std::string& script) {
  return ExecuteScript(script.c_str());
}

int ExecuteScript(const std::string& script, std::string& result) {
  return ExecuteScript(script.c_str(), result);
}

int ExecuteScript(const char* script) {
  std::string result;
  return ExecuteScript(script, result);
}

int ExecuteScript(const char* script, std::string& result) {
  std::array<char, 128> buffer;

  FILE* file = popen(script, "r");
  if (!file) {
    pclose(file);
    return EXIT_FAILURE;
  }

  while (!feof(file)) {
    if (fgets(buffer.data(), buffer.size(), file) != nullptr)
      result += buffer.data();
  }

  return WEXITSTATUS(pclose(file));
}
