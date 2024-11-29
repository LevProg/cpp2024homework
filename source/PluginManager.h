#include <vector>
#include <string>
#include <windows.h>
#include <filesystem>
#include <iostream>
#include <stdexcept>

using PluginFunction = std::pair<bool, double>(*)(const char*, double);

struct Plugin {
    HMODULE library;
    PluginFunction function;
};

class PluginManager {
public:
    static std::vector<Plugin> loadPlugins(const std::string& directory);
    static void unloadPlugins(std::vector<Plugin>& plugins);
};
