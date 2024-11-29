#include "PluginManager.h"

std::vector<Plugin> PluginManager::loadPlugins(const std::string& directory) {
    std::vector<Plugin> plugins;

    for (const auto& entry : std::filesystem::directory_iterator(directory)) {
        if (entry.path().extension() == ".dll") {
            HMODULE lib = LoadLibraryA(entry.path().string().c_str());
            if (lib) {
                PluginFunction func = (PluginFunction)GetProcAddress(lib, "pluginFunc");
                if (func) {
                    plugins.push_back({ lib, func });
                }
                else {
                    std::cerr << "Ошибка: функция pluginFunc не найдена в " << entry.path().string() << std::endl;
                    FreeLibrary(lib);
                }
            }
            else {
                std::cerr << "Ошибка загрузки DLL: " << entry.path().string() << std::endl;
            }
        }
    }
    return plugins;
}

void PluginManager::unloadPlugins(std::vector<Plugin>& plugins) {
    for (auto& plugin : plugins) {
        FreeLibrary(plugin.library);
    }
}
