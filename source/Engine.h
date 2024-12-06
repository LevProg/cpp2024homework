#pragma once
#include "Wrapper.h"
#include <unordered_map>
#include <string>

class Engine {
public:
    void register_command(const std::string& name, const Wrapper& wrapper);
    void execute(const std::string& name, Wrapper::ArgsMap args, FileWrapper& fileWrapper) const;
private:
    std::unordered_map<std::string, Wrapper> commands_;
};