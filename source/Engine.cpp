#include "Engine.h"
#include <stdexcept>

void Engine::register_command(const std::string& name, const Wrapper& wrapper) {
    commands_[name] = wrapper;
}

void Engine::execute(const std::string& name, Wrapper::ArgsMap args, FileWrapper& fileWrapper) const {
    auto it = commands_.find(name);
    if (it == commands_.end()) {
        throw std::invalid_argument("Command not found: " + name);
    }
    it->second.execute(args, fileWrapper);
}
