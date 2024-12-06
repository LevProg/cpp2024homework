#pragma once
#include "FileWrapper.h"
#include <unordered_map>
#include <stdexcept>
#include <functional>
#include <string>

class Wrapper {
public:
    using ArgsMap = std::unordered_map<std::string, std::string>;

    Wrapper() = default;

    template <typename InstanceType, typename MethodType>
    Wrapper(InstanceType* instance, MethodType method, const ArgsMap& defaultArgs)
        : invoker_([=](ArgsMap& args, FileWrapper& fileWrapper) -> void {
        ArgsMap mergedArgs = defaultArgs;
        for (const auto& [key, value] : args) {
            mergedArgs[key] = value;
        }
        call_method(instance, method, mergedArgs, fileWrapper);
    }) {}

    void execute(ArgsMap& args, FileWrapper& fileWrapper) const {
        if (!invoker_) {
            throw std::runtime_error("Method invoker not initialized");
        }
        invoker_(args, fileWrapper);
    }

private:
    using Invoker = std::function<void(ArgsMap&, FileWrapper&)>;
    Invoker invoker_;

    template <typename InstanceType, typename MethodType>
    static void call_method(InstanceType* instance, MethodType method, ArgsMap& args, FileWrapper& fileWrapper) {
        args["text"] = fileWrapper.read();
        std::string result = (instance->*method)(args);
        fileWrapper.write(result);
    }
};